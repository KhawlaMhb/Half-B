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

void put_pixel (GdkPixbuf *pixbuf, int x, int y,
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

  for(x = minx ; x < maxx ; x++)
    {
      put_pixel(pixbuf, x, y, 255, 0, 0); //red pixel value
    }
}

// Goes through the image and draw a redline when needed
void line(OCR *o)
{
  int width = gdk_pixbuf_get_width(o->pixbuf);
  int height = gdk_pixbuf_get_height(o->pixbuf);
  int y;

  for(y=0; y < height; y++)
    {
      if(y == 0)
	{
	  if((whiteline(o->pixbuf,y) == width)
	     &&(whiteline(o->pixbuf,y+1) != width))
	    {
	      put_redline(o->pixbuf, y, o->minx, o->maxx);
	      o->arrLines[(o->arrLinesMax)++] = y;
	      break;
	    }
	}
      else
	{
	  if(y == height - 1)
	    {
	      if((whiteline(o->pixbuf,y) == width)
		 && (whiteline(o->pixbuf,y-1) != width))
		{
		  put_redline(o->pixbuf, y, o->minx, o->maxx);
		  o->arrLines[(o->arrLinesMax)++] = y;
		  break;
		}
	    }
	  else
	    {
	      if((whiteline(o->pixbuf,y) == width)
		 && (((whiteline(o->pixbuf,y-1) != width)) ||
		     (whiteline(o->pixbuf,y+1) != width)))
		{
		  put_redline(o->pixbuf, y, o->minx, o->maxx);
		  o->arrLines[o->arrLinesMax++] = y;
		}
	    }
	}
    }
  for(int i = 0; i<o->arrLinesMax; i++)
    {
      printf("line %d = %d\n",i,o->arrLines[i]);
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
void column(OCR *o)
{
  int width  = gdk_pixbuf_get_width(o->pixbuf);
  int height = gdk_pixbuf_get_height(o->pixbuf);
  int x;

  o->maxx = 0;
  o->minx = height;

  for(x = 0; x < width; x++)
    {
      if(x == 0)
	{
	  if ((whitecolumn(o->pixbuf,x) == height)
	      && (whitecolumn(o->pixbuf,x + 1) != height))
	    {
	      o->minx = isMIN(o->minx, x);
	      o->maxx = isMAX(o->maxx, x);
	    }
	}
      else if (x == width - 1)
	{
	  if((whitecolumn(o->pixbuf,x) == height)
	     &&(whitecolumn(o->pixbuf,x-1) != height))
	    {
	      o->minx = isMIN(o->minx, x);
	      o->maxx = isMAX(o->maxx, x);
	    }
	}
      else if ((whitecolumn(o->pixbuf,x) == height)
	       && ((whitecolumn(o->pixbuf,x-1) != height ||
		    (whitecolumn(o->pixbuf,x+1) != height ))))
	{
	  o->minx = isMIN(o->minx, x);
	  o->maxx = isMAX(o->maxx, x);
	}
    }
  put_redcolumn(o->pixbuf, o->maxx);
  put_redcolumn(o->pixbuf, o->minx);
}

// Check the white columns between two given lines
// the lines were found thanks to put_redline (saved in an array)
int whitecolumn2(GdkPixbuf *pixbuf, int x, int y1, int y2)
{
  int count = 0;
  guchar *p;
  int y;
  for(y = y1; y < y2; y++)
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


// Draws a red column between two given lines
void put_redcolumn2(GdkPixbuf *pixbuf, int x, int y1, int y2)
{
  int y = y1;

  while(y < y2)
    {
      // Color the pixel in red
      put_pixel(pixbuf, x, y, 255, 0, 0);
      y++;
    }
}

// Goes through two given lines, and puts a column
// next to a character (before and after)
// Input
//      *pixbuf  
void column2(OCR *o, int y1, int y2)
{
  int height = y2-y1;
  int x;

  printf("start column2 y1 = %d, y2 = %d\n",y1,y2);
  for(x = o->minx; x < o->maxx; x++)
    {
      if(x == o->minx)
	{
	  if ((whitecolumn2(o->pixbuf,x, y1, y2) == height)
	      && (whitecolumn2(o->pixbuf,x + 1, y1, y2) != height))
	    {
	      put_redcolumn2(o->pixbuf, x, y1, y2);
	      o->arrColumns[o->arrColumnsMax++] = x;
	    }
	}
      else if ((x == o->maxx - 1) && ((whitecolumn2(o->pixbuf,x,y1,y2)== height)
	     &&(whitecolumn2(o->pixbuf,x-1, y1, y2) != height)))
	    {
	      put_redcolumn2(o->pixbuf, x, y1, y2);
	      o->arrColumns[o->arrColumnsMax++] = x;
	    }
	
      else if ((whitecolumn2(o->pixbuf, x, y1, y2) == height)
	       && ((whitecolumn2(o->pixbuf, x-1, y1, y2) != height ||
		    (whitecolumn2(o->pixbuf, x+1, y1, y2) != height ))))
	{
	  put_redcolumn2(o->pixbuf, x, y1, y2);
	  o->arrColumns[o->arrColumnsMax++] = x;
	}
    }
  printf("rectangle\n");
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
  for (x = 0; x < width; x++)
    {
      for (y = 0; y < height; y++)
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

void process_text_lines(OCR *o)
{
  // Print coordinates of each rectangle into the console
  printf("arrlinesmax = %d\n",o->arrLinesMax);
  for(int i = 0; i < o->arrLinesMax ; i += 2)
    {
      // Detect characters
      column2(o, o->arrLines[i]+1, o->arrLines[i+1]);
    }
}

/*void process_text_columns(GdkPixbuf *pixbuf, int minx, int maxx, 
			int *arrColumns, int arrColumnsMax)
{
  // Print coordinates of each rectangle into the console
  for(int i = 0; i < arrColumnsMax ; i += 2)
    {
      // Detect characters
      // column2(pixbuf, (minx)+1, (maxx)-1, arrColumns[i]+1, arrColumns[i+1]);
    }
    }*/
