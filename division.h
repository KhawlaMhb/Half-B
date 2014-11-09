#ifndef DIV_H
#define DIV_H

#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>

struct rectangle_struct
{
  int x1;
  int y1;
  int x2;
  int y2;
};
// Use rectangle for declaring variables instead of struct
typedef struct rectangle_struct rectangle;

struct OCR_struct
{
  GdkPixbuf *pixbuf;
  int minx;            // Left Boundary of the Text
  int maxx;            // Right Boundary of the Text
  int arrLines[100];   // Array containing the lines
  int arrLinesMax;           // Number of lines in the array arrLines
  int arrColumns[100];       // Array containing the columns
  int arrColumnsMax;         // Number of columns in the array
  rectangle TextLines[100];
  int TextLinesMax;
  rectangle CharBoxes[100];  // Character Areas
  int CharBoxesMax;          // Number of Character Areas
};
typedef struct OCR_struct OCR;

void put_pixel (GdkPixbuf *pixbuf, int x, int y, 
		       guchar red, guchar green, guchar blue);
guchar* get_pixel(GdkPixbuf *pixbuf, int x, int y);
void line (OCR *o);
void column(OCR *o);
int isWhite(guchar *p);
void ChangeToBW(GdkPixbuf *pixbuf, int threshold);
void process_text_lines(OCR *o);


#endif
