#include <stdlib.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "division.h"

GtkBuilder *builder;

int main (int argc, char *argv[])
{
  GtkWidget *window1;
  GtkImage *image;
  GdkPixbuf *myBMP;
  GError *myError = 0;
  //int minx, maxx;
  //int arrLines[100];
  //int arrLinesMax = 0;
  //int arrColumns[100];
  //int arrColumnsMax = 0;
  OCR myOCR;

  // minx = 0;
  //maxx = 0;

  gtk_init (&argc, &argv);

  myBMP = gdk_pixbuf_new_from_file(on_file_activate(),&myError);
  ChangeToBW(myBMP,128);

  myOCR.pixbuf = myBMP;
  myOCR.minx = 0;
  myOCR.maxx = 0;
  myOCR.arrLinesMax = 0;
  myOCR.arrColumnsMax = 0;
  myOCR.TextLinesMax = 0;
  myOCR.CharBoxesMax = 0;

  column(&myOCR);
  line(&myOCR);

  printf("line1 = %d\n",myOCR.arrLines[0]);
  // Print the rectangle containing each line of text
  process_text_lines(&myOCR);

  // Saves the modifications of the file into BWcar.jpg
  if(gdk_pixbuf_get_width(myBMP)>700 && gdk_pixbuf_get_height(myBMP)>700)
  {
 myBMP = gdk_pixbuf_scale_simple (myBMP,gdk_pixbuf_get_width(myBMP)/2,
        gdk_pixbuf_get_height(myBMP)/2, GDK_INTERP_NEAREST);
  }

  gdk_pixbuf_save(myBMP,"BWcar.jpeg", "jpeg", &myError,NULL);

  // Builds the interface from the .xml file into the GtkBuilder
  builder = gtk_builder_new();

  gtk_builder_add_from_file (builder, "interfacevs1.glade", NULL);
  if(myError != NULL)
    {
      printf("Error reading interface\nMessage: %s\n",myError->message);
    }
  // Initialize the window and all the widgets recursively
  window1 = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
  gtk_window_set_default_size(GTK_WINDOW(window1), 640, 480);
  gtk_window_set_resizable (GTK_WINDOW(window1), FALSE);
  image = GTK_IMAGE(gtk_builder_get_object(builder, "image1"));
  gtk_image_set_from_file(image, "BWcar.jpeg");

  //Connect widgets with callback signals and show window
  gtk_builder_connect_signals (builder, NULL);
  gtk_widget_show (window1);
  gtk_widget_show (GTK_WIDGET(image));

  gtk_main ();
  g_object_unref (G_OBJECT (builder));

  return 0;
}
