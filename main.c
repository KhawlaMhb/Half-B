#include <stdlib.h>
#include <gtk/gtk.h>
#include "callbacks.h"

GtkBuilder *builder;

int main (int argc, char *argv[]) 
{
  GtkWidget *window1;
  GtkImage *image;
  GdkPixbuf *myBMP;

  GError *myError = 0;
  int width;

  gtk_init (&argc, &argv);

  myBMP = gdk_pixbuf_new_from_file("oiseau.bmp",&myError);

  width = gdk_pixbuf_get_width (myBMP);
  printf("myBMP->width: %d",width);

  /* 

      
          
      builder = gtk_builder_new ();
        if (gtk_builder_add_from_file (builder, "interfacevs1.glade", NULL) == 0) 
        {
            fprintf (stderr, "Erreur: ouverture du fichier GLADE\n") ;
            exit(1);
        }
             
      window1 = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
      image = gtk_builder_get_object(builder, "image1");
      gtk_image_set_from_file(image, "car.jpg");

      gtk_window_set_default_size(GTK_WINDOW(window1), 640, 480);

      gtk_builder_connect_signals (builder, NULL);
      //gtk_builder_add_callback_symbol(builder, "on_file_activate", G_CALLBACK(on_file_activate));
                  
      gtk_widget_show (window1);
      gtk_widget_show (GTK_WIDGET(image));

      gtk_main ();
      g_object_unref (G_OBJECT (builder));*/

                        
         return 0;
}

