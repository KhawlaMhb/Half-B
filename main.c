#include <stdlib.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "division.h"

GtkBuilder *builder;

static void ChangeToBW(GdkPixbuf *pixbuf, int threshold);

int main (int argc, char *argv[]) 
{
  GtkWidget *window1;
  GtkImage *image;
  GdkPixbuf *myBMP;

  GError *myError = 0;

  gtk_init (&argc, &argv);

  myBMP = gdk_pixbuf_new_from_file("car.jpg",&myError); 
  // only car.jpg can be read what kind of sorcery is that
  ChangeToBW(myBMP,128);
  printf("ahhaha");
  line(myBMP);
 printf("ahhaha");

  gdk_pixbuf_save(myBMP,"BWcar.jpeg", "jpeg", &myError, NULL);
          
   builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "interfacevs1.glade", NULL) == 0) 
   {
            fprintf (stderr, "Erreur: ouverture du fichier GLADE\n") ;
            exit(1);
   }
             
      window1 = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
      image = gtk_builder_get_object(builder, "image1");
      gtk_image_set_from_file(image, "BWcar.jpeg");

      gtk_window_set_default_size(GTK_WINDOW(window1), 640, 480);

      gtk_builder_connect_signals (builder, NULL);
      gtk_widget_show (window1);
      gtk_widget_show (GTK_WIDGET(image));

      gtk_main ();
      g_object_unref (G_OBJECT (builder));
                    
      return 0;
}

static void ChangeToBW(GdkPixbuf *pixbuf, int threshold)
{
int width, height, rowstride, n_channels;
  guchar *pixels, *p;
  int x;
  int y;
  int grey;

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
      p[0]=255;
      p[1]=255;
      p[2]=255;
    }
  else
    {
      p[0]=0;
      p[1]=0;
      p[2]=0;
    }
 }
  }
}
