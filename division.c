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

        if((p[0] == 255) && (p[1] == 255) && (p[2] == 255))
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
int red(GdkPixbuf *pixbuf, int y)
{
    int count = 0;
    int x = 0;
    guchar *p;
    //for(x=0; x < gdk_pixbuf_get_width(pixbuf); x++)
    //{
         p = get_pixel(pixbuf, x, y);

        if((p[0] == 255) && (p[1] == 0) && (p[2] == 0))
            {
               return 0;
            }
   //}
   return 1;

}

void line(GdkPixbuf *pixbuf)
{
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int x, y;

 //for(x=0; x < width; x++)
   // { 
        for(y=0; y < height; y++)
        {
            if(y == 0)
            {
                if((whiteline(pixbuf,y) == width)
		   &&(whiteline(pixbuf,y+1) != width) && red (pixbuf,y+1) !=
                   0)
                {
                     put_redline(pixbuf, y);
                    printf("Check  y==0 with y = %d, x = %d\n",y,x);
                     break;
                }
            }
	    else 
            { 
                if(y == height - 1)
                {
                        if((whiteline(pixbuf,y) == width)
		        &&(whiteline(pixbuf,y-1) != width) /*&& red
                        (pixbuf,y-1)!=0*/)
                            
                        {
                                put_redline(pixbuf, y);
                                printf("Check  y==h-1 with y = %d, x = %d\n",y,x);

                                break;
                        }
                }
            
	        else 
                {
                    if((whiteline(pixbuf,y) == width)
		    && (((whiteline(pixbuf,y-1) != width) && red(pixbuf,y-1)) ||
                        ((whiteline(pixbuf,y+1) != width)&&red(pixbuf,y+1))))
                    {
                        put_redline(pixbuf, y);
 printf("Check  y==y with y = %d, x = %d\n",y,x);

                    }
                }
            }
        }
   // }
}


