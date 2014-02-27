/*
=================================================================================
 Name        : Displayprn.c
 Version     : 1

Simple display statux and makibox logo
needs:
 wiringpi --> https://projects.drogon.net/raspberry-pi/wiringpi/
 PCD8544 library --> http://binerry.de/post/25787954149/pcd8544-library-for-raspberry-pi

 Description :

	 LCD pins      Raspberry Pi
	 LCD1 - GND    P06 - GND
	 LCD2 - VCC    P01 - 3.3V
	 LCD3 - CLK    P11 - GPIO0
	 LCD4 - Din    P12 - GPIO1
	 LCD5 - DC     P13 - GPIO2
	 LCD6 - CS-CE  P15 - GPIO3
	 LCD7 - RST    P16 - GPIO4
	 LCD8 - LED    P06 - GND 


free to modify free to share but please no sell it :)
================================================================================
 */
 
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "PCD8544.h"


// pin setup
int _din = 1;
int _sclk = 0;
int _dc = 2;
int _rst = 4;
int _cs = 3;

// lcd contrast  
int contrast = 50;

//use png and convert with http://www.henningkarlsen.com/electronics/t_imageconverter_mono.php
const uint8_t logoprinter[]={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08,   // 0x0010 (16) pixels
0x04, 0x02, 0x02, 0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x40, 0x80, 0x00,   // 0x0020 (32) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0030 (48) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0040 (64) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0050 (80) pixels
0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xF8,   // 0x0060 (96) pixels
0xF0, 0xF0, 0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8,   // 0x0070 (112) pixels
0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0080 (128) pixels
0x00, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0090 (144) pixels
0xFE, 0xFF, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x80, 0x00,   // 0x00A0 (160) pixels
0x00, 0x00, 0x0C, 0x9E, 0xDE, 0x9E, 0x0C, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03,   // 0x00B0 (176) pixels
0x03, 0x07, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F,   // 0x00C0 (192) pixels
0x3F, 0x1F, 0x1F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,   // 0x00D0 (208) pixels
0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xBF, 0x0F, 0x07, 0x07, 0x03, 0x07, 0x07, 0x0F, 0xBF, 0xFE, 0xFC,   // 0x00E0 (224) pixels
0xF8, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0x9F,   // 0x00F0 (240) pixels
0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0100 (256) pixels
0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,   // 0x0110 (272) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0120 (288) pixels
0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3C, 0x3C, 0x38, 0x3C, 0x3C,   // 0x0130 (304) pixels
0x1E, 0x0F, 0x3F, 0x3F, 0x1F, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01,   // 0x0140 (320) pixels
0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x00,   // 0x0150 (336) pixels
0x07, 0x0F, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0160 (352) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,   // 0x0170 (368) pixels
0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0x8E, 0x8E, 0x8E,   // 0x0180 (384) pixels
0x8E, 0x8E, 0x8E, 0xDC, 0xFC, 0xF8, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0x38, 0x1C, 0x0C, 0x0E, 0x0E,   // 0x0190 (400) pixels
0x0C, 0x1C, 0x38, 0xF8, 0xF0, 0xC0, 0x08, 0x1C, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0xC0, 0xE0, 0xF0,   // 0x01A0 (416) pixels
0xF8, 0x7C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04,   // 0x01B0 (432) pixels
0x04, 0x08, 0x10, 0x10, 0x20, 0x40, 0x40, 0x80, 0x40, 0x40, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04,   // 0x01C0 (448) pixels
0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,   // 0x01D0 (464) pixels
0xFF, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x77, 0x7F, 0x3E, 0x00, 0x00, 0x07, 0x1F, 0x3F, 0x78,   // 0x01E0 (480) pixels
0x70, 0xE0, 0xE0, 0xE0, 0xE0, 0x70, 0x78, 0x3F, 0x1F, 0x07, 0x20, 0x70, 0xF8, 0x7C, 0x3E, 0x1F,   // 0x01F0 (496) pixels
0x0F, 0x07, 0x0F, 0x1F, 0x3C, 0x78, 0xF0, 0x60, 
};

 

int main ( int argc, char *argv[] )
{

  // check wiringPi setup
  if (wiringPiSetup() == -1)
  {
	printf("wiringPi-Error\n");
    exit(1);
  }
  
  // init and clear lcd
  LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
  LCDclear();
  LCDclear();
  delay(500);

 
    if ( argc < 5 ) /* argc should be 2 for correct execution */
    { 
      LCDclear();
      LCDdrawbitmap(0, 0, logoprinter, 84, 64, BLACK);

      LCDdisplay();
      delay(100);
    
    }
    else 
    {    
         LCDdrawstring(0, 0, argv[1]);
	  LCDdrawline(0, 7, 83, 7, BLACK);
	  LCDdrawstring(0, 9, argv[2]);
	  LCDdrawstring(0, 17, argv[3]);
	  LCDdrawstring(0, 25, argv[4]);
         LCDdrawstring(0, 33, argv[5]);
	  LCDdrawstring(0, 41, argv[6]);

         LCDdisplay();
         delay(100);


    }

    
  
  return 0;
}