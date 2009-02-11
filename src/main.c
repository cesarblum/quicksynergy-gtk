/*  QuickSynergy -- a GUI for synergy
 *  Copyright (C) 2006, 2007 Cesar L. B. Silveira, Otavio C. Cordeiro
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

#include "config.h"

#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "logo/qslogo.h"
#include "callbacks.h"
#include "synergy_config.h"
#include "screen_entry.h"
#include "proc.h"
#include "intl.h"

GtkWidget *main_window;
GtkWidget *notebook;
GtkWidget *hostname_entry;
GtkWidget *table;
GtkWidget *vbox0;
GtkWidget *above_entry, *below_entry, *left_entry, *right_entry;
int synergy_running = 0;
#ifndef HAVE_GTK_2_10
int keep_running;
#endif

int main(int argc, char **argv) {
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
#ifdef HAVE_GTK_2_10
    GtkStatusIcon *status_icon;
#endif
    pid_t pid = 0;

#ifdef ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain (PACKAGE);
#endif

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
        G_CALLBACK(delete_event), NULL);
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
    above_entry = screen_entry_new(_("Above"));
    below_entry = screen_entry_new(_("Below"));
    left_entry = screen_entry_new(_("Left"));
    right_entry = screen_entry_new(_("Right"));

    /* attach entries to table */
    gtk_table_attach_defaults(GTK_TABLE(table), above_entry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), below_entry, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), left_entry, 0, 1, 1, 2);            
    gtk_table_attach_defaults(GTK_TABLE(table), right_entry, 2, 3, 1, 2);       
    
    /* image to be displayed in the center of the main window */
    //image = gtk_image_new_from_pixbuf(
    //  gdk_pixbuf_new_from_xpm_data((const char **) logo_data));
    image = gtk_image_new_from_pixbuf(
        gdk_pixbuf_new_from_inline(-1, qslogo, FALSE, NULL));
    gtk_table_attach_defaults(GTK_TABLE(table), image, 1, 2, 1, 2);
    
    /*
    gtk_table_set_row_spacings(GTK_TABLE(table), 30);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);    
    label = gtk_label_new("laodicea");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 1, 2);
    */
    
    /* add server page to the notebook */
    page_label = gtk_label_new(_("Share"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, page_label);
    
    /* client's vbox */
    vbox0 = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox0), 5);
    
    label = gtk_label_new(_("Server hostname/IP address:"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox0), label, FALSE, FALSE, 0);
    
    /* entry for placing the server's hostname/IP address */
    hostname_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox0), hostname_entry, FALSE, FALSE, 0);    
    
    /* add client page to notebook */
    page_label = gtk_label_new(_("Use"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox0, page_label);

#ifndef HAVE_GTK_2_10   
    /* settings page */
    vbox0 = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox0), 5);
    
    /* check button for setting the KeepRunning option */
    check_button = gtk_check_button_new_with_label(
        _("Keep synergy running after QuickSynergy is closed"));
    g_signal_connect(G_OBJECT(check_button), "toggled",
        G_CALLBACK(keep_running_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(vbox0), check_button, FALSE, FALSE, 0);
    
    /* add settings page to notebook */
    page_label = gtk_label_new(_("Settings"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox0, page_label);
#endif
    
    /* pack the notebook into the main vbox */
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
    
    /* add a separator */
    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 0);
    
    /* build the hbox that will hold the action buttons */
    hbox =  gtk_hbox_new(TRUE, 22);
    
    /* about button */
    about_button = gtk_button_new_with_label(GTK_STOCK_ABOUT);
    gtk_button_set_use_stock(GTK_BUTTON(about_button), TRUE);
    g_signal_connect(G_OBJECT(about_button), "clicked",
        G_CALLBACK(about_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), about_button, TRUE, TRUE, 0);
    
    /* start/stop button */
    start_button = gtk_button_new_with_label(GTK_STOCK_EXECUTE);
    gtk_button_set_use_stock(GTK_BUTTON(start_button), TRUE);
    g_signal_connect(G_OBJECT(start_button), "clicked",
        G_CALLBACK(start_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), start_button, TRUE, TRUE, 0); 
    
    /* close button */
    close_button = gtk_button_new_with_label(GTK_STOCK_CLOSE);
    gtk_button_set_use_stock(GTK_BUTTON(close_button), TRUE);
    g_signal_connect(G_OBJECT(close_button), "clicked",
        G_CALLBACK(close_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), close_button, TRUE, TRUE, 0);
    
    /* pack the hbox into the main vbox */
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    
    /* load previous configuration */
    load_config();

#ifndef HAVE_GTK_2_10   
    /* if the KeepRunning configuration option is 1, set the
     * settings check button to an active state */
    if(keep_running) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button), TRUE);
    }
#endif
    
    /* check if synergy is already running */
    if((pid = get_pid_by_name("synergys"))) {
        gtk_button_set_label(GTK_BUTTON(start_button), _("Stop"));
        gtk_widget_set_sensitive(notebook, FALSE);
        synergy_running = 1;
    }
    else if((pid = get_pid_by_name("synergyc"))) {
        gtk_button_set_label(GTK_BUTTON(start_button), _("Stop"));
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
        gtk_widget_set_sensitive(notebook, FALSE);
        synergy_running = 1;
    }
    
#ifdef HAVE_GTK_2_10
    status_icon = gtk_status_icon_new_from_pixbuf(
        gdk_pixbuf_new_from_inline(-1, qslogo, FALSE, NULL));
    
    gtk_status_icon_set_visible(status_icon, TRUE);
    gtk_status_icon_set_tooltip(status_icon, "QuickSynergy");
    
    g_signal_connect(G_OBJECT(status_icon), "popup-menu", 
        G_CALLBACK(status_icon_popup), NULL);
#endif

    /* display the main window */   
    gtk_widget_show_all(main_window);
    
    /* GTK mainloop */
    gtk_main();
    
    return 0;
}
