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

void put_pixel (GdkPixbuf *pixbuf, int x, int y, 
		       guchar red, guchar green, guchar blue);
guchar* get_pixel(GdkPixbuf *pixbuf, int x, int y);
void line (GdkPixbuf *pixbuf, int *minx, int *maxx, 
	   int arrLines[], int *arrLinesMax);
int whiteline(GdkPixbuf *pixbuf,int y);
void column(GdkPixbuf *pixbuf, int *minx, int *maxx);
int whitecolumn(GdkPixbuf *pixbuf,int x);
void put_redcolumn(GdkPixbuf *pixbuf, int y);
int isWhite(guchar *p);
void ChangeToBW(GdkPixbuf *pixbuf, int threshold);
void process_text_lines(GdkPixbuf *pixbuf, int minx, int maxx, 
			int *arrLines, int arrLinesMax);


#endif
