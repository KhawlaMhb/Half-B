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


static void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
{
      int width, height, rowstride, n_channels;
      guchar *pixels, *p;

      n_channels = gdk_pixbuf_get_n_channels (pixbuf);

      g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
      g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
      g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
      g_assert (n_channels == 4);

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
int whiteline(GdkPixbuf *pixbuf, int x, int y)
{
    int count =0;
    guchar *p;

    for(x=0; x < gdk_pixbuf_get_width(pixbuf); x++)
    {
         p = get_pixel(pixbuf, x, y);

        if((p[0] == 255) &&( p[1] == 255) &&( p[2] == 255))
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
    printf("check1");

 for(x=0; x < width; x++)
    { 
        for(y=0; y < height; y++)
        {
            if(y == 0)
            {
                if((whiteline(pixbuf,x,y) == width)&&( whiteline(pixbuf,x,y+1) != width) )
                {
                     put_redline(pixbuf, y);
                     break;
                }

            }
             if(y == height - 1)
            {
                if((whiteline(pixbuf,x,y) == width)&&( whiteline(pixbuf,x,y - 1) != width) )
                {
                     put_redline(pixbuf, y);
                     break;
                }

            }

            if((whiteline(pixbuf,x,y) == width) && ((whiteline(pixbuf,x,y-1) != width) ||( whiteline(pixbuf,x,y+1) != width)))
            {
              put_redline(pixbuf, y); 
            }

        }
        printf("check x: %d",x);
    }
}


