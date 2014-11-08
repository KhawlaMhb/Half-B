#ifndef DIV_H
#define DIV_H

#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>

void put_pixel (GdkPixbuf *pixbuf, int x, int y, 
		       guchar red, guchar green, guchar blue);
guchar* get_pixel(GdkPixbuf *pixbuf, int x, int y);
void line (GdkPixbuf *pixbuf, int *minx, int *maxx, 
	   int arrLines[], int *arrLinesMax);
int whiteline(GdkPixbuf *pixbuf,int y);
void column(GdkPixbuf *pixbuf, int *minx, int *maxx);
int whitecolumn(GdkPixbuf *pixbuf,int y);
void put_redcolumn(GdkPixbuf *pixbuf, int y);
int isWhite(guchar *p);
void ChangeToBW(GdkPixbuf *pixbuf, int threshold);


#endif
