#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk/gdk.h>
#include "division.h"

guchar* get_pixel(GdkPixbuf *pixbuf, int x, int y)
{
  int rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  return p;
 }

static void put_pixel (GdkPixbuf *pixbuf, int x, int y, 
		       guchar red, guchar green, guchar blue, guchar alpha)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  // g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
  // g_assert (n_channels == 4);

  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
  p[3] = alpha;
}

int whiteline(GdkPixbuf *pixbuf, int y)
{
  int count = 0;
  guchar *p;
  int x;

  for(x=0; x < gdk_pixbuf_get_width(pixbuf); x++)
    {
      p = get_pixel(pixbuf, x, y);

      if(((p[0] == 255) && (p[1] == 255) && (p[2] == 255))
	 || ((p[0] == 255) && (p[1] == 0) && (p[2] == 0)))
	{
	  count++;
	}
    }
  return count;
}

void put_redline(GdkPixbuf *pixbuf, int y)
{
    int x = 0;
    int width = gdk_pixbuf_get_width(pixbuf);

    while(x < width)
    {    
        put_pixel(pixbuf, x, y, 255, 0, 0, 255); //pixel rouge valeur
        x++;
    }
}

void line(GdkPixbuf *pixbuf)
{
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int x, y;

  for(y=0; y < height; y++)
    {
      if(y == 0)
	{
	  if((whiteline(pixbuf,y) == width)
	     &&(whiteline(pixbuf,y+1) != width))
	    {
	      put_redline(pixbuf, y);
	      printf("Check  y==0 with y = %d\n",y);
	      break;
	    }
	}
      else 
	{ 
	  if(y == height - 1)
	    {
	      if((whiteline(pixbuf,y) == width)
		 && (whiteline(pixbuf,y-1) != width))
		{
		  put_redline(pixbuf, y);
		  //printf("Check y == h - 1 with y = %\n",y);
		  break;
		}
	    }
	  else 
	    {
	      if((whiteline(pixbuf,y) == width)
		 && (((whiteline(pixbuf,y-1) != width)) ||
		     (whiteline(pixbuf,y+1) != width)))
		{
		  put_redline(pixbuf, y);
		  printf("Check  y==y with y = %d\n",y);
		}
	    }
	}
    }
}

int isWhite(guchar *p)
{
    if (p[0]==255 && p[1]==255 && p[2]==255) return 1;
    if (p[0]==255 && p[1]==0 && p[2]==0) return 1;
    return 0;
}

int whitecolumn(GdkPixbuf *pixbuf, int x)
{
  int count = 0;
  guchar *p;
  int y;


  for(y = 0; y < gdk_pixbuf_get_height(pixbuf); y++)
    {
      p = get_pixel(pixbuf, x, y);

      //if the pixel is white
      if(isWhite(p))
	{
	  count++;
	}
    }
  return count;
}

void put_redcolumn(GdkPixbuf *pixbuf, int x)
{
    int y = 0;
    int height = gdk_pixbuf_get_height(pixbuf);


  printf("Red Column: %d\n",x);

    while(y < height)
    {
      // Color the pixel in red
        put_pixel(pixbuf, x, y, 255, 0, 0, 255);
        y++;
    }
}

void column(GdkPixbuf *pixbuf)
{
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int x, y;

  for(x = 0; x < width; x++)
    {

      if(x == 0)
	{
	  if((whitecolumn(pixbuf,x) == height)
	     &&(whitecolumn(pixbuf,x+1) != height))
	    {
	      put_redcolumn(pixbuf, x);
	      printf("Check  x==0 with x = %d\n",x);
	      break;
	    }
	}
      else
	{
	  if(x == width - 1)
	    {
	      if((whitecolumn(pixbuf,x) == height)
		 &&(whitecolumn(pixbuf,x-1) != height))
		{
		  put_redcolumn(pixbuf, x);
		  printf("Check  x==h-1 with x = %d\n",x);
		  break;
		}
	    }
	  else
	    {
	      if((whitecolumn(pixbuf,x) == height)
		 && ((whitecolumn(pixbuf,x-1) != height ||
		       (whitecolumn(pixbuf,x+1) != height))))
		{
		  printf("Check x==x with x = %d\n",x);
		  printf("Check count = %d\n",whitecolumn(pixbuf,x));
		  printf("Check count x-1  = %d\n",whitecolumn(pixbuf,x-1));
		  printf("Check count x+1 = %d\n",whitecolumn(pixbuf,x+1));
		  put_redcolumn(pixbuf, x);
		}
	    }
	}
    }
}
