#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk/gdk.h>
#include "division.h"

/*
static void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);
struct pixel* get_pixel(GdkPixbuf *pixbuf, int x, int y);
*/

/*int main_(int argc, char *args[])
{
    init_gtk();
    return 0;
}*/

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

void line(GdkPixbuf *pixbuf)
{

    int x, y;

 for(x=0; x<gdk_pixbuf_get_width(pixbuf); x++)
    { 
        for(y=0; y<gdk_pixbuf_get_height(pixbuf); y++)
        {
            if(whiteline(pixbuf,y) == gdk_pixbuf_get_width(pixbuf) && (whiteline(pixbuf,y-1) != gdk_pixbuf_get_width(pixbuf)  || whiteline(pixbuf,y+1) != gdk_pixbuf_get_width(pixbuf)))
            {
              put_redline(pixbuf, y); 
            }

        }
    }
}

int whiteline(GdkPixbuf *pixbuf, int y)
{
    int x, count =0;
    for(x=0; x < gdk_pixbuf_get_width(pixbuf); x++)
    {
        if(get_pixel(pixbuf, x, y)==0)
            {
                count++;
            } 
       
   }
   return count;
}


void put_redline(GdkPixbuf *pixbuf, int y)
{
    int x=0;
    while(x<gdk_pixbuf_get_width(pixbuf))
    {    
        put_pixel(pixbuf, x, y, 120, 0, 0, 0); //pixel rouge valeur
        x++;
    }

}

