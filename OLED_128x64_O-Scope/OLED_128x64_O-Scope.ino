/* 
 *  Simple Oscilloscope for Arduino Micro and a 4-pin 128x64 single color 
 *  OLED panel. Pin diagram: http://pighixxx.com/micropdf.pdf
 *  SDA D2
 *  SCL D3
 *  
 *  Hook up a POT to A0
 *  ref u8glib: https://code.google.com/p/u8glib/
 *  To install u8glib on Arduino IDE: http://goo.gl/j3olBA
 *  */

// 

#include "U8glib.h"
#include <math.h>
#include <TimerOne.h>


// For Arduino Micro

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

const int WIDTH=128;
const int HEIGHT=64;
const int READ_RATE= 1000000.0 / 3.0 ; // Analog read rate, in us

const int analogInPin = A0;
int analogInValue = 0;

char buf[20];
char ftoa_buf[10];

int x;
int y[WIDTH];

void clearY(){
  for(int i=0; i<WIDTH; i++){
    y[i] = -1;
  }
}

// The default sprintf can't handle floats, 
// so this fonction converts to ascii first. 
// from http://forum.arduino.cc/index.php?topic=44262.0
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

void drawY(int aval){

  float v = (float(aval)/1024.0);
  
  sprintf(buf, "%3d%% %sV", (int) (v*100.0),ftoa(ftoa_buf, v*5.0, 2));
  
  u8g.drawStr( 3, 10, buf);
  
  u8g.drawPixel(0, y[0]);
  for(int i=1; i<WIDTH; i++){
    if(y[i]!=-1){
      u8g.drawPixel(i, y[i]);
      //u8g.drawLine(i-1, y[i-1], i, y[i]);
      //u8g.drawVLine(i, 0, y[i]);
      
    }else{
      break;
    }
  }
}

void setup(void) {
  Timer1.initialize(READ_RATE);
  Timer1.attachInterrupt(read_analog); 
  Serial.begin(115200);
  u8g.setRot180();
  u8g.setFont(u8g_font_6x10);
  x = 0;
  clearY();

}

void read_analog(void){
  analogInValue = analogRead(analogInPin);
  y[WIDTH-1] = map(analogInValue, 0, 1023, HEIGHT-1, 0);;
  
  for(int i = 0; i < WIDTH-1; i++){
    y[i] = y[i+1];
  }
}

void loop(void) {
 
  u8g.firstPage();  
  do {
    drawY(analogInValue);
  } while( u8g.nextPage() );

  Serial.println(analogInValue);

  delay(100);
  
}
