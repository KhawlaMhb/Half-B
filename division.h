#ifndef DIV_H
#define DIV_H

#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>

static void put_pixel (GdkPixbuf *pixbuf, int x, int y, 
		       guchar red, guchar green, guchar blue);
guchar* get_pixel(GdkPixbuf *pixbuf, int x, int y);
void line (GdkPixbuf *pixbuf);
int whiteline(GdkPixbuf *pixbuf,int y);
void put_redline(GdkPixbuf *pixbuf, int y);
void column(GdkPixbuf *pixbuf);
int whitecolumn(GdkPixbuf *pixbuf,int y);
void put_redcolumn(GdkPixbuf *pixbuf, int y);
int isWhite(guchar *p);


#endif
