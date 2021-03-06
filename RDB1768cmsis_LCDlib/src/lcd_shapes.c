//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009 Code Red Technologies Ltd. 
//
// lcd_sahpes.c contains various routines to draw shapes the LCD display on the
// RDB1768 development board.
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 

#include "lcd.h"

#include <stdlib.h>		// to provice abs() function

// Routine to draw a filled rectangle to the LCD.
// Two corners of rectangle are at (xmin,ymin) and (xmax,ymax).
// The Rectangle is filled with the RGB565 color specified
void LCD_FilledRect(int xmin,int xmax,int ymin,int ymax,int color)
{
    int i;

    // Specify to LCD controller coordinates we are writing to...
    LCDdriver_WriteCom(DD_CASET); 	// Set the column address
    LCDdriver_WriteData(xmin);		// min address
    LCDdriver_WriteData(xmax);		// max address
    LCDdriver_WriteCom(DD_RASET);	// Set the row address
	// Screen is offset by 1 in Y axis in relation to controller RAM buffer
    LCDdriver_WriteData(ymin + 1);	// min address
    LCDdriver_WriteData(ymax + 1);	// max address
    LCDdriver_WriteCom(DD_RAMWR);	// RAM Write command

    // Plot the color data to the LCD buffer
    for(i = ((xmax - xmin + 1) * (ymax - ymin + 1)); i > 0; i--)
    {
    	LCDdriver_WriteData(color >> 8);	// top 8 bits of RGB565 color
    	LCDdriver_WriteData(color);			// bottom 8 bits of RGB565 color
    }
}

// Routine to draw an unfilled rectangle to the LCD.
// Two corners of rectangle are at (xmin,ymin) and (xmax,ymax).
// The Rectangle is drawn in the RGB565 color specified
void LCD_Rect(int xmin,int xmax,int ymin,int ymax,int color)
{
	// Draw 4 lines of rectange as 4 filled rectanges, each of 1 pixel wide
	LCD_FilledRect(xmin,xmin,ymin,ymax,color);
	LCD_FilledRect(xmax,xmax,ymin,ymax,color);
	LCD_FilledRect(xmin,xmax,ymin,ymin,color);
	LCD_FilledRect(xmin,xmax,ymax,ymax,color);
}



// Plot a point on the screen in the 6:5:6 color format
void LCD_PlotPoint(int x,int y,int color)
{
    LCDdriver_WriteCom(DD_CASET);	// Set the column address 
    LCDdriver_WriteData(x);			// min address
    LCDdriver_WriteData(x);			// max address
    LCDdriver_WriteCom(DD_RASET);	// Set the row address
	// Screen is offset by 1 in Y axis in relation to controller RAM buffer
    LCDdriver_WriteData(y + 1);		// min address
    LCDdriver_WriteData(y + 1);		// max address
    LCDdriver_WriteCom(DD_RAMWR);	// RAM Write command
    LCDdriver_WriteData(color >> 8);	// top 8 bits of RGB565 color
    LCDdriver_WriteData(color);			// top 8 bits of RGB565 color
}

// Routine to draw a filled circle to the LCD.
// The centre of the circle is at (x0,y0) and the circle has the 
// specifed radius. The circle is filled with the RGB565 color 
// The circle is drawn using the "Midpoint circle algorithm", 
// also known as "Bresenham's circle algorithm". In order to fill
// the circle, the algorithm has been modifed to draw a line between
// each two points, rather than plotting the two points individually.
void LCD_FilledCircle (int x0, int y0, int radius, int color)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
  
  LCD_FilledRect(x0, x0 ,y0 - radius,y0 + radius, color); 
  LCD_FilledRect(x0 - radius, x0 + radius ,y0,y0, color);  
  
  while(x < y)
  {
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    

    LCD_FilledRect(x0-x, x0+x ,y0 +y, y0 + y, color);    
    LCD_FilledRect(x0-x, x0+x ,y0 - y, y0 - y, color); 
    LCD_FilledRect(x0-y, x0+y ,y0 + x, y0 + x, color);         
    LCD_FilledRect(x0-y, x0+y ,y0 - x, y0 - x, color); 
  }
}

// Routine to draw an unfilled circle to the LCD.
// The centre of the circle is at (x0,y0) and the circle has the 
// specifed radius. The circle is drawn in the RGB565 color 
// The circle is drawn using the "Midpoint circle algorithm", 
// also known as "Bresenham's circle algorithm". 
void LCD_Circle (int x0, int y0, int radius, int color)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;

  LCD_PlotPoint(x0, y0 + radius, color);
  LCD_PlotPoint(x0, y0 - radius, color);
  LCD_PlotPoint(x0 + radius, y0, color);
  LCD_PlotPoint(x0 - radius, y0, color);

  while(x < y)
  {
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    LCD_PlotPoint(x0 + x, y0 + y, color);
    LCD_PlotPoint(x0 - x, y0 + y, color);
    LCD_PlotPoint(x0 + x, y0 - y, color);
    LCD_PlotPoint(x0 - x, y0 - y, color);
    LCD_PlotPoint(x0 + y, y0 + x, color);
    LCD_PlotPoint(x0 - y, y0 + x, color);
    LCD_PlotPoint(x0 + y, y0 - x, color);
    LCD_PlotPoint(x0 - y, y0 - x, color);
  }
}

// Routine to draw a line in the RGB565 color to the LCD.
// The line is drawn from (xmin,ymin) to (xmax,ymax).
// The algorithm used to draw the line is "Bresenham's line
// algorithm". 
#define SWAP(a, b)  a ^= b; b ^= a; a ^= b; 

void LCD_Line (int xmin,int xmax,int ymin,int ymax,int color)
{
   int Dx = xmax - xmin; 
   int Dy = ymax - ymin;
   int steep = (abs(Dy) >= abs(Dx));
   if (steep) {
       SWAP(xmin, ymin);
       SWAP(xmax, ymax);
       // recompute Dx, Dy after swap
       Dx = xmax - xmin;
       Dy = ymax - ymin;
   }
   int xstep = 1;
   if (Dx < 0) {
       xstep = -1;
       Dx = -Dx;
   }
   int ystep = 1;
   if (Dy < 0) {
       ystep = -1;		
       Dy = -Dy; 
   }
   int TwoDy = 2*Dy; 
   int TwoDyTwoDx = TwoDy - 2*Dx; // 2*Dy - 2*Dx
   int E = TwoDy - Dx; //2*Dy - Dx
   int y = ymin;
   int xDraw, yDraw;
   int x;
   for (x = xmin; x != xmax; x += xstep) {		
       if (steep) {			
           xDraw = y;
           yDraw = x;
       } else {			
           xDraw = x;
           yDraw = y;
       }
       // plot
       LCD_PlotPoint(xDraw, yDraw, color);
       // next
       if (E > 0) {
           E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
           y = y + ystep;
       } else {
           E += TwoDy; //E += 2*Dy;
       }
   }
}


