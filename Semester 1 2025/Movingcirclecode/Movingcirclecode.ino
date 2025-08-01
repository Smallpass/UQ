#include "U8glib.h" 
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9); // SCK = 13, MOSI = 11, CS = 10, A0 = 9 
int x_pos;
int y_pos;
void draw(void) { 
  // graphic commands to redraw the complete screen should be placed here   
  u8g.setFont(u8g_font_unifont); 
  //u8g.setFont(u8g_font_osb21); 
  u8g.drawCircle( x_pos,  y_pos,  5);


} 
void setup(void) { 
  x_pos = 5;
  y_pos = 22;
  // flip screen, if required 
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
  while (Serial.available() > 0) {
    // picture loop 
    Serial.println("Serial is available");
    u8g.firstPage();   
    do { 
      if (Serial.read() == 'd') {
      x_pos += 1;
      draw(); 
    }
    } while( u8g.nextPage() ); 
    // rebuild the picture after some delay 
    delay(50); 
  }
}