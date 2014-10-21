#include <stdio.h>   /* sprintf */
#include <stdlib.h>  /* atoi */
#include <stdio.h>
#include <string.h>  /* strlen, strcmp */
#include <ctype.h>   /* isdigit */
#include "callbacks.h"

extern GtkBuilder *builder;

void on_file_activate (GtkWidget *widget, gpointer user_data)
{
    GtkWidget *dialog;
   
    dialog = gtk_file_chooser_dialog_new ("Open", NULL,GTK_FILE_CHOOSER_ACTION_OPEN, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"),GTK_RESPONSE_ACCEPT, NULL);


    if (gtk_dialog_run (GTK_DIALOG(dialog))== GTK_RESPONSE_ACCEPT)
          {
                  char *filename;
                  filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
                  //open_file (filename);
                  g_free (filename);
          }

    gtk_widget_destroy (dialog);

}
/*on_image_display (GtkObject *object, gpointer user_data)
{
    GtkImage *image_to_load;
    image_to_load = gtk_image_new_from_file(*filename);
    gtk_widget_show(image_to_load);

}*/
