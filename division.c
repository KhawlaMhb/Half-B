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
		       guchar red, guchar green, guchar blue)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  g_assert (n_channels == 3);

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
}

// Checks if every pixel on the line are white
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

// Draw a red line between two column lines (minx and maxx)
void put_redline(GdkPixbuf *pixbuf, int y, int minx, int maxx)
{
  int x;
  //int xcol=0;
  //guchar *p;
  //int width = gdk_pixbuf_get_width(pixbuf);

  //p = get_pixel(pixbuf, x, y);

  for(x = minx ; x < maxx ; x++)
    {
      put_pixel(pixbuf, x, y, 255, 0, 0); //red pixel value
      x++;
    }
}

// Goes through the image and draw a redline when needed
void line(GdkPixbuf *pixbuf, int *minx, int *maxx)
{
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int y;

  for(y=0; y < height; y++)
    {
      if(y == 0)
	{
	  if((whiteline(pixbuf,y) == width)
	     &&(whiteline(pixbuf,y+1) != width))
	    {
	      put_redline(pixbuf, y, *minx, *maxx);
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
		  put_redline(pixbuf, y, *minx, *maxx);
		  break;
		}
	    }
	  else
	    {
	      if((whiteline(pixbuf,y) == width)
		 && (((whiteline(pixbuf,y-1) != width)) ||
		     (whiteline(pixbuf,y+1) != width)))
		{
		  put_redline(pixbuf, y, *minx, *maxx);
		}
	    }
	}
    }
}

int isMIN(int minx, int x)
{
  if (minx > x) return x;
  else return minx;
}

int isMAX(int maxx, int x)
{
  if(maxx > x) return maxx;
  else return x;
}

int isWhite(guchar *p)
{
  if (p[0]==255 && p[1]==255 && p[2]==255) {
    return 1;
  }
  if (p[0]==255 && p[1]==0 && p[2]==0) {
    return 1;
  }
  return 0;
}


// Check if the whole column has only white pixels
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

// Draws a red column
void put_redcolumn(GdkPixbuf *pixbuf, int x)
{
  int y = 0;
  int height = gdk_pixbuf_get_height(pixbuf);

  while(y < height)
    {
      // Color the pixel in red
      put_pixel(pixbuf, x, y, 255, 0, 0);
      y++;
    }
}

// Goes through the image and draws a red column when needed
void column(GdkPixbuf *pixbuf, int *minx, int *maxx)
{
  int width  = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int x;

  *maxx = 0;
  *minx = height;

  for(x = 0; x < width; x++)
    {
      if(x == 0)
	{
	  if ((whitecolumn(pixbuf,x) == height)
	      && (whitecolumn(pixbuf,x + 1) != height))
	    {
	      *minx = isMIN(*minx, x);
	      *maxx = isMAX(*maxx, x);
	    }
	}
      else if (x == width - 1)
	{
	  if((whitecolumn(pixbuf,x) == height)
	     &&(whitecolumn(pixbuf,x-1) != height))
	    {
	      *minx = isMIN(*minx, x);
	      *maxx = isMAX(*maxx, x);
	    }
	}
      else if ((whitecolumn(pixbuf,x) == height)
	       && ((whitecolumn(pixbuf,x-1) != height ||
		    (whitecolumn(pixbuf,x+1) != height ))))
	{
	  *minx = isMIN(*minx, x);
	  *maxx = isMAX(*maxx, x);
	}
    }
  put_redcolumn(pixbuf, *maxx);
  put_redcolumn(pixbuf, *minx);
}

void ChangeToBW(GdkPixbuf *pixbuf, int threshold)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;
  int x;
  int y;

  // Get all image data needed for this operation
  n_channels = gdk_pixbuf_get_n_channels (pixbuf);
  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  // Loop through all pixels
  for (x=0;x<width;x++)
    {
      for (y=0;y<height;y++)
	{
	  p = pixels + y * rowstride + x * n_channels;
	  if ((p[0] + p[1] + p[2])/3 > threshold)
	    {
	      // White pixel
	      p[0]=255;
	      p[1]=255;
	      p[2]=255;
	    }
	  else
	    {
	      // Black pixel
	      p[0]=0;
	      p[1]=0;
	      p[2]=0;
	    }
	}
    }
}
