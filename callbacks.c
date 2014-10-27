#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "callbacks.h"

extern GtkBuilder *builder;

char *on_file_activate ()
{
  GtkWidget *dialog;
  char *filename;
  dialog = gtk_file_chooser_dialog_new ("Open", 
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					("_Cancel"), GTK_RESPONSE_CANCEL,
					("_Open"),GTK_RESPONSE_ACCEPT, NULL);


  if (gtk_dialog_run (GTK_DIALOG(dialog))== GTK_RESPONSE_ACCEPT)
    {
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
    }
  gtk_widget_destroy (dialog);
  return filename;
}
