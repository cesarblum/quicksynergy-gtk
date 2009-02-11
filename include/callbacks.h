#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

gboolean entry_focus_in_event(GtkWidget *widget, GdkEventFocus *event, gpointer data);
gboolean entry_focus_out_event(GtkWidget *widget, GdkEventFocus *event, gpointer data);
void about_button_clicked(GtkWidget *widget, gpointer data);
void start_button_clicked(GtkWidget *widget, gpointer data);
gboolean app_quit(GtkWidget *widget, GdkEvent *event, gpointer data);

#endif
