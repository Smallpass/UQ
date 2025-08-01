/*
 * Elevator-Emulator.c
 *
 * Main file
 *
 * Authors: Peter Sutton, Ahmed Baig
 * Modified by <YOUR NAME HERE>
 */ 

/* Definitions */

#define F_CPU 8000000L

/* External Library Includes */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

/* Internal Library Includes */

#include "display.h"
#include "ledmatrix.h"
#include "buttons.h"
#include "serialio.h"
#include "terminalio.h"
#include "timer0.h"

/* Data Structures */

typedef enum {UNDEF_FLOOR = -1, FLOOR_0=0, FLOOR_1=4, FLOOR_2=8, FLOOR_3=12} ElevatorFloor;

/* Global Variables */
uint32_t time_since_move;
ElevatorFloor current_position;
#define QUEUE_BUFFER 100
ElevatorFloor start_pos[QUEUE_BUFFER];
ElevatorFloor end_pos[QUEUE_BUFFER];
uint32_t Queue_num = 0;
int current_Queue_num = 0;
bool traveller_onboard = false;
bool service_mode;
bool start_elevator_animation = false;




/* Internal Function Declarations */

void initialise_hardware(void);
void start_screen(void);
void start_elevator_emulator(void);
void handle_inputs(void);
void draw_elevator(void);
void draw_floors(void);
void draw_travellers();
void clear_travellers(void);
void switch_setup(void);
void seven_segment_display(char*, ElevatorFloor, bool);
void play_sound(bool);



/* Main */

int main(void) {
	// Setup hardware and call backs. This will turn on 
	// interrupts.
	initialise_hardware();
	
	// Show the splash screen message. Returns when display is complete
	start_screen();
	
	// Start elevator controller software
	start_elevator_emulator();
}

/* Internal Function Definitions */

/**
 * @brief All hardware initialisation occurs here
 * @arg none
 * @retval none
*/
void initialise_hardware(void) {
	
	ledmatrix_setup();
	init_button_interrupts();
	// Setup serial port for 19200 baud communication with no echo
	// of incoming characters
	init_serial_stdio(19200,0);
	
	init_timer0();
	
	// Turn on global interrupts
	sei();
}

/**
 * @brief Displays the "EC" start screen with elevator symbol
 * @arg none
 * @retval none
*/
void start_screen(void) {
	// Clear terminal screen and output a message
	clear_terminal();
	move_terminal_cursor(10,10);
	printf_P(PSTR("Elevator Controller\n"));
	move_terminal_cursor(10,12);
	printf_P(PSTR("CSSE2010/7201 project by Samuel Allpass\n"));
	
	// Show start screen
	start_display();
	
	// Animation variables
	uint32_t doors_frame_time = 0;
	uint32_t interval_delay = 150;
	uint8_t frame = 0;
	uint8_t doors_opening_closing = 1; // 1 => opening, 0 => closing
	
	// Wait until a button is pressed, or 's' is pressed on the terminal
	while(1) {
		
		// Don't worry about this if/else tree. Its purely for animating
		// the elevator doors on the start screen
		if (get_current_time() - doors_frame_time  > interval_delay) {
			start_display_animation(frame);
			doors_frame_time   = get_current_time(); // Reset delay until next movement update
			if (doors_opening_closing) {
				interval_delay = 150;
				frame++;
				if (frame == 1) interval_delay = 2000;
				if (frame == 3) doors_opening_closing = 0;
			} else {
				interval_delay = 150;
				frame--;
				if (frame == 2) interval_delay = 500;
				if (frame == 0) doors_opening_closing = 1;
			}
		}
	
		// First check for if a 's' is pressed
		// There are two steps to this
		// 1) collect any serial input (if available)
		// 2) check if the input is equal to the character 's'
		char serial_input = -1;
		if (serial_input_available()) {
			serial_input = fgetc(stdin);
		}
		// If the serial input is 's', then exit the start screen
		if (serial_input == 's' || serial_input == 'S') {
			break;
		}
		// Next check for any button presses
		int8_t btn = button_pushed();
		if (btn != NO_BUTTON_PUSHED) {
			break;
		}
	}
}

/**
 * @brief Initialises LED matrix and then starts infinite loop handling elevator
 * @arg none
 * @retval none
*/
void start_elevator_emulator(void) {
	
	// Clear the serial terminal
	clear_terminal();
	
	// Initialise Display
	initialise_display();
	
	// Clear a button push or serial input if any are waiting
	// (The cast to void means the return value is ignored.)
	(void)button_pushed();
	clear_serial_input_buffer();

	// Initialise local variables
	time_since_move = get_current_time();
	
	// Draw the floors and elevator
	draw_elevator();
	draw_floors();
	switch_setup();
	
	current_position = FLOOR_0;
	ElevatorFloor elevator_dest = FLOOR_0;
	char* direction;
	int old_position = FLOOR_0;
	char* old_direction = "Stationary";
	bool elevator_slow;
	int elevator_speed;
	int with_traveller_count = 0;
	int without_traveller_count = 0;
	bool updated = false;

	bool showing_floor = false;
	uint32_t animation_start_time;
	
	clear_terminal();
	move_terminal_cursor(0,0);
	printf("Direction: Stationary\n\n\n");
	printf("Floor: 0\n\n");
	printf("Number of floors moved with passenger: 0\n");
	printf("Number of floors moved without passenger: 0\n");

	while(true) {
		
		elevator_slow = !(PINA & (1 << PA6));
		
		if (elevator_slow){
			elevator_speed = 300;
		} else {
			elevator_speed = 100;
		}

		service_mode = (PIND & (1 << PD3));
		if (service_mode) {
			int joystick_value = enter_service_mode();
			if (joystick_value > 800) {
				elevator_dest = FLOOR_3;
				elevator_speed = 100;
				
			} else if (joystick_value > 530) {
				elevator_dest = FLOOR_3;
				elevator_speed = 300;

			} else if (joystick_value < 200) {
				elevator_dest = FLOOR_0;
				elevator_speed = 100;
			} else if (joystick_value < 470) {
				elevator_dest = FLOOR_0;
				elevator_speed = 300;
			} else {
				elevator_dest = current_position;
				direction = "Stationary";
			}
		}

		// Only update the elevator every 200 ms
		if (get_current_time() - time_since_move > elevator_speed && !start_elevator_animation) {

			//Check for direction of travel
			if (elevator_dest - current_position > 0) { // Move up
				direction = "Up";
			} else if (elevator_dest - current_position < 0) { // Move down
				direction = "Down";
			} else {
				direction = "Stationary";
			}

			if ((current_position != old_position || strcmp(old_direction, direction)) 
						&& !(current_position%4)) {
				print_to_terminal(direction, current_position, with_traveller_count, without_traveller_count);

			}
			old_direction = direction;
			old_position = current_position;
					
			// Adjust the elevator based on where it needs to go
			if (elevator_dest - current_position > 0) { // Move up
				current_position++;
			} else if (elevator_dest - current_position < 0) { // Move down
				current_position--;
			}


			// As we have potentially changed the elevator position, lets redraw it
			draw_elevator();
			time_since_move = get_current_time(); // Reset delay until next movement update
			updated = true;
		} else {
				updated = false;
		}	

		showing_floor = !showing_floor;
		seven_segment_display(direction, current_position, showing_floor);

		
		bool moving = strcmp(direction, "Stationary");

		if (!(current_position%4) && updated && moving) {
			if (traveller_onboard) {
				with_traveller_count++;
		
			} else {

				without_traveller_count++;
			}

		}

		handle_inputs();
		if (current_Queue_num < Queue_num && updated && !service_mode) {
			int start_level = start_pos[current_Queue_num];
			int end_level = end_pos[current_Queue_num];

			if (current_position == start_level) {
				elevator_dest = end_level;
				traveller_onboard = true;
				draw_travellers(traveller_onboard);
				start_elevator_animation = true;
				animation_start_time = get_current_time();



			} else if (!moving) {
				elevator_dest = start_level;
				traveller_onboard = false;

			}  else if (current_position == end_level && elevator_dest == end_level) {
				traveller_onboard = false;
				current_Queue_num++;
				start_elevator_animation = true;
				draw_travellers(traveller_onboard);
				animation_start_time = get_current_time();
			}

		}
		if (start_elevator_animation) {
			elevator_doors_animation(animation_start_time);
		}
	}
}


int enter_service_mode(void) {
	// Set reference voltage to AVcc (5V), select ADC0 by default
    ADMUX = (1 << REFS0);
    // Enable ADC and set prescaler to 64 (gives 125kHz ADC clock from 8MHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
    // Select ADC7
    ADMUX = (ADMUX & 0xF0) | 0x07;
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for it to complete
    while (ADCSRA & (1 << ADSC));
    // Return result
    return ADC;
}

void elevator_doors_animation(uint32_t animation_start_time) {
	int doors_closed = (0 << PA0)|(1 << PA1)|(1 << PA2)|(0 << PA3);
	int doors_open = (1 << PA0)|(0 << PA1)|(0 << PA2)|(1 << PA3);
	int reset_pins = ~((1 << PA0)|(1 << PA1) | (1 << PA2) | (1 << PA3));
	
	if (get_current_time() - animation_start_time > 1200) {
		start_elevator_animation = false;
	} else if (get_current_time() - animation_start_time > 800) {
		PORTA &= reset_pins;
		PORTA = doors_closed;
	} else if (get_current_time() - animation_start_time > 400) {
		if (get_current_time() - animation_start_time < 500) {
			play_sound(true);
		}
		PORTA &= reset_pins;
		PORTA |= doors_open;

	} else {
		PORTA &= reset_pins;
		PORTA = doors_closed;
	}
}




void play_sound(bool boarding) {
	int boarding_time = 100;
	int placed_time = 50;
	uint32_t time_since_start;
	if (boarding) {
		// Reset Timer1 counter
		TCNT1 = 0;

		// Set TOP value for 500 Hz (16,000 counts)
		ICR1 = 16000;

		// Set 50% duty cycle
		OCR1B = 8000;

		// Set Fast PWM Mode 14: WGM13:0 = 1110 (TOP = ICR1)
		TCCR1A = (1 << COM1B1);                    // Clear OC1B on compare match
		TCCR1B = (1 << WGM13) | (1 << WGM12) |     // WGM13:WGM12 = 1:1
				(1 << CS10);                     // Prescaler = 1 (no division)
		TCCR1A |= (1 << WGM11);                   // Complete WGM11 for mode 14
		time_since_start = get_current_time();

		while (get_current_time() - time_since_start < boarding_time) {
			DDRD = (DDRD & ~(1 << PD4)) | (1 << PD4);	
		}
			DDRD &= ~(1 << PD4);		

	} else {

		// Setup Timer1 for 3kHz PWM
		TCNT1 = 0;            // Reset counter
		ICR1 = 2666;          // Set TOP for 3kHz
		OCR1B = 1333;         // 50% duty

		// Fast PWM, Mode 14: WGM13:0 = 1110
		TCCR1A = (1 << COM1B1);  // Clear OC1B on compare match (non-inverting)
		TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  // Prescaler = 1
		TCCR1A |= (1 << WGM11);
		time_since_start = get_current_time();
		while (get_current_time() - time_since_start < placed_time) {
			DDRD = (DDRD & ~(1 << PD4)) | (1 << PD4);		
		}
		DDRD &= ~(1 << PD4);
	}
}

void print_to_terminal(char* direction, int current_position, int with_traveller_count, int without_traveller_count) {
	move_terminal_cursor(12, 0);
	clear_to_end_of_line();
	printf("%s", direction);
	move_terminal_cursor(8, 4);
	clear_to_end_of_line();

	printf("%d", current_position/4);
	move_terminal_cursor(40, 6);
	clear_to_end_of_line();
	printf("%d", with_traveller_count);
	move_terminal_cursor(43, 7);
	clear_to_end_of_line();
	printf("%d", without_traveller_count);
}

int get_destination() {
	int msb = (PINA & (1 << PA5)) ? 1 : 0;
	int lsb = (PINA & (1 << PA4)) ? 1 : 0;
	int dest = (msb << 1) | lsb;
	return (dest)*4;
}

void seven_segment_display(char* direction, int current_position, bool showing_floor) {
    uint8_t seven_seg[4] = {63, 6, 91, 79};  // 0,1,2,3

	if (showing_floor) {
		PORTC = 0x00;             // Turn off 7-seg
		PORTD &= ~(1 << PD2);  // Indicate floor mode
		if (!strcmp(direction, "Down")) {
			PORTC = seven_seg[(current_position+3)/4];
		} else {
			PORTC = seven_seg[current_position/4];
		}
		if (current_position%4 != 0) {
			PORTC |= (1 << PC7);
		}
		_delay_ms(3);
	
	} else {
		PORTC = 0x00;             // Turn off 7-seg
		PORTD |= (1 << PD2);   // Indicate direction mode

		if (!strcmp(direction, "Up")) {
            PORTC |= (1 << PA0);
        } else if (!strcmp(direction, "Down")) {
            PORTC |= (1 << PA3);
        } else {
            PORTC |= (1 << PA6);
        }
		_delay_ms(3);
	}
}


void switch_setup() {
	// Set up switches to be on A pins
	DDRA &= ~((1 << PA4) | (1 << PA5) | (1 << PA6));
	// Set up service mode switch on C
	// Set up joystick output on PA7
	DDRA &= ~(1 << PA7);

	// Set up seven segment display on C pins
	DDRC = 0xFF;	
	DDRD |= (1 << PD2);
	// Set up L0 to L3 LED pins with A0 to A34 as outputs
	DDRA |= (1 << PA0)|(1 << PA1)|(1 << PA2)|(1 << PA3);

	// Initialise closed elevator doors
	PORTA = (0 << PA0)|(1 << PA1)|(1 << PA2)|(0 << PA3);

}

void clear_travellers() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			spi_send_byte(0x01);
			uint8_t dest = (((4+j) & 0x07) << 4) | ((14 - i*4) & 0x0F);
			spi_send_byte(dest);
			spi_send_byte(0);
		}
			
	}
}

void draw_travellers() {
	clear_travellers();
	int offset = 0;
	if (traveller_onboard) {
		offset++;
	}

	int floor0_num = 0;
	int floor1_num = 0;
	int floor2_num = 0;
	int floor3_num = 0;

	for (int i = current_Queue_num + offset; i < Queue_num; i++) {
		int start = start_pos[i];
		int x_pos = 14 - start;
		int y_pos;
		int destination = end_pos[i];
		// Find place on floor to place
		if (start == FLOOR_0) {
			y_pos = 4 + floor0_num;
			floor0_num++;
		} else if (start == FLOOR_1) {
			y_pos = 4 + floor1_num;
			floor1_num++;
		} else if (start == FLOOR_2) {
			y_pos = 4 + floor2_num;
			floor2_num++;
		} else if (start == FLOOR_3) {
			y_pos = 4 + floor3_num;
			floor3_num++;
		}

		// Find colour
		int colour;
		if (destination == FLOOR_0) {
			colour = COLOUR_LIGHT_RED;
		} else if (destination == FLOOR_1) {
			colour = COLOUR_LIGHT_GREEN;
		} else if (destination == FLOOR_2) {
			colour = COLOUR_LIGHT_YELLOW;
		} else if (destination == FLOOR_3) {
			colour = COLOUR_LIGHT_ORANGE;
		}
		if (y_pos  < 8 && y_pos > 3) {
			spi_send_byte(0x01);
			uint8_t dest = ((y_pos & 0x07) << 4) | (x_pos & 0x0F);
			spi_send_byte(dest);
			spi_send_byte(colour);
		}
	}
}


/**
 * @brief Draws 4 lines of "FLOOR" coloured pixels
 * @arg none
 * @retval none
*/
void draw_floors(void) {
	for (uint8_t i = 0; i < WIDTH; i++) {
		update_square_colour(i, FLOOR_0, FLOOR);
		update_square_colour(i, FLOOR_1, FLOOR);
		update_square_colour(i, FLOOR_2, FLOOR);
		update_square_colour(i, FLOOR_3, FLOOR);
	}
}

/**
 * @brief Draws the elevator at the current_position
 * @arg none
 * @retval none
*/
void draw_elevator(void) {
	
	// Store where it used to be with old_position
	static uint8_t old_position; // static variables maintain their value, every time the function is called
	
	int8_t y = 0; // Height position to draw elevator (i.e. y axis)
	// Clear where the elevator was
	if (old_position > current_position) { // Elevator going down - clear above
		y = old_position + 3;
		} else if (old_position < current_position) { // Elevator going up - clear below
		y = old_position + 1;
	}
	if (y % 4 != 0) { // Do not draw over the floor's LEDs
		update_square_colour(1, y, EMPTY_SQUARE);
		update_square_colour(2, y, EMPTY_SQUARE);
	}
	old_position = current_position;
	
	// Draw a 2x3 block representing the elevator
	for (uint8_t i = 1; i <= 3; i++) { // 3 is the height of the elevator sprite on the LED matrix
		y = current_position + i; // Adds current floor position to i=1->3 to draw elevator as 3-high block
		if (y % 4 != 0) { // Do not draw on the floor
			update_square_colour(1, y, ELEVATOR);
			update_square_colour(2, y, ELEVATOR); // Elevator is 2 LEDs wide so draw twice
		}
	}
}

/**
 * @brief Reads btn values and serial input and adds a traveller as appropriate
 * @arg none
 * @retval none
*/
void handle_inputs(void) {
	
	/* ******** START HERE ********
	
	 The following code handles moving the elevator using the buttons on the
	 IO Board. Add code to handle BUTTON2_PUSHED and BUTTON3_PUSHED
	 
	 Here is how the following code works:
	 1. Get btn presses (if any have occurred). Remember that this is
		all handled in the buttons.c/h library.
	 2. Use an if/else tree based on which of the buttons has been
		pressed.
	 3. Set the destination of the elevator to the FLOOR_X corresponding
		with the particular button that was pressed.
	
	*/
	
	// We need to check if any button has been pushed
	uint8_t btn = button_pushed();
	// Initialise serial_input with -1 indicating no data
	char serial_input = -1;
	if (serial_input_available()) {
		serial_input = fgetc(stdin);
	}
	if (btn*4 == get_destination()) {
		return;
	}
	else if (btn == BUTTON0_PUSHED || serial_input == '0') {
		// Move to Floor 0
		start_pos[Queue_num] = FLOOR_0;
		end_pos[Queue_num] = get_destination();
		Queue_num++;
		draw_travellers();
		play_sound(false);

	} else if (btn == BUTTON1_PUSHED || serial_input == '1') {
		// Move to Floor 1
		start_pos[Queue_num] = FLOOR_1;
		end_pos[Queue_num] = get_destination();
		Queue_num++;
		draw_travellers();
		play_sound(false);

	} else if (btn == BUTTON2_PUSHED || serial_input == '2') {
		// Move to Floor 1
		start_pos[Queue_num] = FLOOR_2;
		end_pos[Queue_num] = get_destination();
		Queue_num++;
		draw_travellers();
		play_sound(false);

	} else if (btn == BUTTON3_PUSHED || serial_input == '3') {
		// Move to Floor 1
		start_pos[Queue_num] = FLOOR_3;
		end_pos[Queue_num] = get_destination();
		Queue_num++;
		draw_travellers();
		play_sound(false);
	}
	
}

