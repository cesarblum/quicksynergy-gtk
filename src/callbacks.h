/*  QuickSynergy -- a GUI for synergy
 *  Copyright (C) 2006, 2007, 2008, 2009 Cesar L. B. Silveira, Otavio C. Cordeiro
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */ 

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include "config.h"

enum synergy_mode {
    SYNERGY_SERVER_RUNNING = 1,
    SYNERGY_CLIENT_RUNNING
};

gboolean entry_focus_in_event(GtkWidget *widget, GdkEventFocus *event, gpointer data);
gboolean entry_focus_out_event(GtkWidget *widget, GdkEventFocus *event, gpointer data);
void entry_changed_cb(GtkEntry *entry, gpointer data);
void checkbox_changed_cb(GtkToggleButton *button, gpointer data);
void notebook_page_switched(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, gpointer user_data);
void about_button_clicked(GtkWidget *widget, gpointer data);
void start_button_clicked(GtkWidget *widget, gpointer data);
void close_button_clicked(GtkWidget *widget, gpointer data);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
#if GTK_CHECK_VERSION(2,10,0)
void quicksynergy_quit(GtkWidget *widget, gpointer data);
void show_main_window(GtkWidget *widget, gpointer data);
void status_icon_popup(GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer user_data);
#endif

#endif
