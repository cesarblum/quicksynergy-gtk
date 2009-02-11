/*  QuickSynergy -- a GUI for synergy
 *  Copyright (C) 2006 César L. B. Silveira, Otávio C. Cordeiro
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

#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "logo_data.h"
#include "callbacks.h"
#include "config.h"
#include "proc.h"
#include "misc.h"

GtkWidget *main_window;
GtkWidget *notebook;
GtkWidget *hostname_entry;
GtkWidget *table;
GtkWidget *vbox0;
int synergy_running = 0;
int keep_running;
pid_t pid = 0;

int main(int argc, char **argv) {
	struct entry_data server_entries;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *page_label;
	GtkWidget *sep;
	GtkWidget *about_button;
	GtkWidget *start_button;
	GtkWidget *close_button;
	GtkWidget *check_button;
	GtkWidget *image;
	GtkWidget *label;
	
	/* initialize GTK */
	gtk_init(&argc, &argv);
	
	/* build the main window */
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window), "QuickSynergy");
	gtk_window_set_position(GTK_WINDOW(main_window), 
            GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(main_window), 5);
	
	/* main window events */
	g_signal_connect(G_OBJECT(main_window), "delete_event",
		G_CALLBACK(delete_event), (gpointer) &server_entries);
	g_signal_connect(G_OBJECT(main_window), "destroy",
		G_CALLBACK(gtk_main_quit), NULL);
	
	/* main vbox that will hold the application's widgets */
	vbox = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(main_window), vbox);
	
	/* Server/Client/Settings notebook */
	notebook = gtk_notebook_new();
	
	/* build the table that will hold the server layout widgets */
	table = gtk_table_new(3, 3, TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(table), 5);
	
	/* text entries for server configuration */
	server_entries.above_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.above_entry), "Above");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.above_entry), 0.5);
	gtk_entry_set_width_chars(
            GTK_ENTRY(server_entries.above_entry), 10);
	g_signal_connect(
            G_OBJECT(server_entries.above_entry), "focus-in-event", 
            G_CALLBACK(entry_focus_in_event), (gpointer) "Above");
	g_signal_connect(
            G_OBJECT(server_entries.above_entry), "focus-out-event",
            G_CALLBACK(entry_focus_out_event), (gpointer) "Above");
    
    /* attach entry to table */
	gtk_table_attach_defaults(GTK_TABLE(table), 
            server_entries.above_entry, 1, 2, 0, 1);
	
	server_entries.below_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.below_entry), "Below");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.below_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.below_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.below_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Below");
	g_signal_connect(G_OBJECT(server_entries.below_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Below");
		
    /* attach entry to table */
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.below_entry, 1, 2, 2, 3);
	
	server_entries.left_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.left_entry), "Left");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.left_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.left_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.left_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Left");
	g_signal_connect(G_OBJECT(server_entries.left_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Left");
		
    /* attach entry to table */
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.left_entry, 0, 1, 1, 2);	
	
	server_entries.right_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.right_entry), "Right");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.right_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.right_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.right_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Right");
	g_signal_connect(G_OBJECT(server_entries.right_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Right");
		
    /* attach entry to table */
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.right_entry, 2, 3, 1, 2);
	
	/* image to be displayed in the center of the main window */
	image = gtk_image_new_from_pixbuf(
		gdk_pixbuf_new_from_xpm_data((const char **) logo_data));
	gtk_table_attach_defaults(GTK_TABLE(table), image, 1, 2, 1, 2);
	
	/*
	gtk_table_set_row_spacings(GTK_TABLE(table), 30);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);	
	label = gtk_label_new("laodicea");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 1, 2);
	*/
	
	/* add server page to the notebook */
	page_label = gtk_label_new("Server");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, page_label);
	
	/* client's vbox */
	vbox0 = gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox0), 5);
	
	label = gtk_label_new("Server hostname/IP address:");
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
	gtk_box_pack_start(GTK_BOX(vbox0), label, FALSE, FALSE, 0);
	
	/* entry for placing the server's hostname/IP address */
	hostname_entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox0), hostname_entry, FALSE, FALSE, 0);	
	
	/* add client page to notebook */
	page_label = gtk_label_new("Client");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox0, page_label);
	
	/* settings page */
	vbox0 = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox0), 5);
	
	/* check button for setting the KeepRunning option */
	check_button = gtk_check_button_new_with_label(
	    "Keep synergy running after QuickSynergy is closed");
	g_signal_connect(G_OBJECT(check_button), "toggled",
	    G_CALLBACK(keep_running_toggled), NULL);
	gtk_box_pack_start(GTK_BOX(vbox0), check_button, FALSE, FALSE, 0);
	
	/* add settings page to notebook */
	page_label = gtk_label_new("Settings");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox0, page_label);
	
	/* pack the notebook into the main vbox */
	gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
	
	/* add a separator */
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 0);
	
	/* build the hbox that will hold the action buttons */
	hbox =  gtk_hbox_new(TRUE, 22);
	
	/* about button */
	about_button = gtk_button_new_with_label("About");
	g_signal_connect(G_OBJECT(about_button), "clicked",
		G_CALLBACK(about_button_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), about_button, TRUE, TRUE, 0);
	
	/* start/stop button */
	start_button = gtk_button_new_with_label("Start");
	g_signal_connect(G_OBJECT(start_button), "clicked",
		G_CALLBACK(start_button_clicked), (gpointer) &server_entries);
	gtk_box_pack_start(GTK_BOX(hbox), start_button, TRUE, TRUE, 0);	
	
	/* close button */
  	close_button = gtk_button_new_with_label("Close");
	g_signal_connect(G_OBJECT(close_button), "clicked",
		G_CALLBACK(close_button_clicked), (gpointer) &server_entries);
    gtk_box_pack_start(GTK_BOX(hbox), close_button, TRUE, TRUE, 0);
	
	/* pack the hbox into the main vbox */
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
	
	/* load previous configuration */
	load_config(&server_entries);
	
	/* if the KeepRunning configuration option is 1, set the
	 * settings check button to an active state */
	if(keep_running) {
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button), TRUE);
	}
	
	/* check if synergy is already running */
	if((pid = get_pid_by_name("synergys"))) {
		gtk_button_set_label(GTK_BUTTON(start_button), "Stop");
		gtk_widget_set_sensitive(notebook, FALSE);
		synergy_running = 1;
	}
	else if((pid = get_pid_by_name("synergyc"))) {
		gtk_button_set_label(GTK_BUTTON(start_button), "Stop");
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
		gtk_widget_set_sensitive(notebook, FALSE);
		synergy_running = 1;
	}

    /* display the main window */	
	gtk_widget_show_all(main_window);
	
	/* GTK mainloop */
	gtk_main();
	
	return 0;
}
