#include <stdlib.h>
#include <gtk/gtk.h>
#include "callbacks.h"

GtkBuilder *builder;

int main (int argc, char *argv[]) {
      GtkWidget *window1;

        
      gtk_init (&argc, &argv);

          
      builder = gtk_builder_new ();
        if (gtk_builder_add_from_file (builder, "interfacevs1.glade", NULL) == 0) 
        {
            fprintf (stderr, "Erreur: ouverture du fichier GLADE\n") ;
            exit(1);
        }

             
      window1 = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
      gtk_window_set_default_size(GTK_WINDOW(window1),840,580);
               
      gtk_builder_connect_signals (builder, "interface.xml");
                  
      gtk_widget_show (window1);

      gtk_main ();
      g_object_unref (G_OBJECT (builder));

                        
         return 0;
}

