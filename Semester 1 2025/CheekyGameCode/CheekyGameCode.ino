#include "U8glib.h" 
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9); // SCK = 13, MOSI = 11, CS = 10, A0 = 9 
const int LRpin = 18; //A0
const int UDpin = 19; // A1
int LR;
int UD;
const int deadzone = 0;
int LR_neutral;
int UD_neutral;

void draw(int x_pos, int y_pos) {

  u8g.setFont(u8g_font_unifont); 
  //u8g.setFont(u8g_font_osb21); 
  u8g.drawStr(x_pos+4, y_pos-20, "o"); 
  u8g.drawStr(x_pos+3, y_pos-10, "+"); 
  u8g.drawStr(x_pos, y_pos, "/\\"); 
}

void setup(void) { 
  // flip screen, if required 
  Serial.begin(9600);
  delay(500);

  UD_neutral = analogRead(UDpin);
  LR_neutral = analogRead(LRpin);
  u8g.setRot180(); 
   // set SPI backup if required 
  //u8g.setHardwareBackup(u8g_backup_avr_spi); 
 // assign default color value 
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) { 
    u8g.setColorIndex(255);     // white 
  } 
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) { 
    u8g.setColorIndex(3);         // max intensity 
  } 
  else if ( u8g.getMode() == U8G_MODE_BW ) { 
    u8g.setColorIndex(1);         // pixel on 
  } 
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) { 
    u8g.setHiColorByRGB(255,255,255); 
  
  }} 
void loop(void) { 
  // picture loop 
  int x_int = 0;
  int y_int = 28;
  u8g.firstPage(); 
  u8g.drawStr(0, 22, "loading");
  u8g.firstPage();
  do { 
    Serial.print(UD+"\n");
    u8g.setFont(u8g_font_unifont);
  
    int add_LR = analogRead(LRpin);
    int add_UD = analogRead(UDpin);
    if (add_LR > deadzone) {
      if (add_UD > deadzone) {
        LR += add_LR;
        UD += add_UD;
        draw(LR, UD);
      }
    }
    Serial.println(LR);


     
  } while( u8g.nextPage() ); 
   // rebuild the picture after some delay 
    
   

}