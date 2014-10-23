#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

/*G_MODULE_EXPORT void on_window1_destroy (GtkObject *object, gpointer user_data) ;*/
void on_file_activate (GtkWidget *widget, gpointer user_data) ;
void on_image_display (GObject *object, gpointer user_data) ;


#endif /* CALLBACKS_H */
