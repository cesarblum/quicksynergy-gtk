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

#include "config.h"

#include <stdlib.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "callbacks.h"
#include "synergy_config.h"
#include "ui.h"
#include "intl.h"

GtkWidget *table;
GtkWidget *vbox0;

GtkWidget *main_window;
GtkWidget *notebook;
GtkWidget *start_button = NULL;

int main(int argc, char **argv) {
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *page;
    GtkWidget *page_label;
    GtkWidget *about_button;
    GtkWidget *close_button;
    GtkWidget *image;
    GtkWidget *label;
#if GTK_CHECK_VERSION(2,10,0)
    GtkStatusIcon *status_icon;
#endif
    qs_state_t *state;

#ifdef ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain (PACKAGE);
#endif

    /* initialize GTK */
    gtk_init(&argc, &argv);

    /* load previous configuration */
    state = load_config();

    /* build the main window */
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "QuickSynergy");
    gtk_window_set_position(GTK_WINDOW(main_window),
            GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 12);
    gtk_window_set_icon(GTK_WINDOW(main_window), make_logo());

    /* main window events */
    g_signal_connect(G_OBJECT(main_window), "delete_event",
        G_CALLBACK(delete_event), NULL);
    g_signal_connect(G_OBJECT(main_window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    /* main vbox that will hold the application's widgets */
    vbox = gtk_vbox_new(FALSE, 18);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    /* Server/Client/Settings notebook */
    notebook = gtk_notebook_new();
    g_signal_connect(G_OBJECT(notebook), "switch-page",
        G_CALLBACK(notebook_page_switched), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

    /* add server page to the notebook */
    page = make_server_tab(state);
    page_label = gtk_label_new(_("Share"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, page_label);

    /* add client page to notebook */
    page = make_client_tab(state);
    page_label = gtk_label_new(_("Use"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, page_label);

    /* add settings page to notebook */
    page = make_settings_tab(state);
    page_label = gtk_label_new(_("Settings"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, page_label);

    /* build the hbox that will hold the action buttons */
    hbox = gtk_hbox_new(TRUE, 22);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    /* about button */
    about_button = gtk_button_new_with_label(GTK_STOCK_ABOUT);
    gtk_button_set_use_stock(GTK_BUTTON(about_button), TRUE);
    g_signal_connect(G_OBJECT(about_button), "clicked",
        G_CALLBACK(about_button_clicked), main_window);
    gtk_box_pack_start(GTK_BOX(hbox), about_button, TRUE, TRUE, 0);

    /* start/stop button */
    start_button = gtk_button_new_with_label(GTK_STOCK_EXECUTE);
    gtk_button_set_use_stock(GTK_BUTTON(start_button), TRUE);
    g_signal_connect(G_OBJECT(start_button), "clicked",
        G_CALLBACK(start_button_clicked), (gpointer) state);
    gtk_box_pack_start(GTK_BOX(hbox), start_button, TRUE, TRUE, 0);

    /* close button */
    close_button = gtk_button_new_with_label(GTK_STOCK_CLOSE);
    gtk_button_set_use_stock(GTK_BUTTON(close_button), TRUE);
    g_signal_connect(G_OBJECT(close_button), "clicked",
        G_CALLBACK(close_button_clicked), (gpointer) state);
    gtk_box_pack_start(GTK_BOX(hbox), close_button, TRUE, TRUE, 0);

#if GTK_CHECK_VERSION(2,10,0)
    status_icon = gtk_status_icon_new_from_pixbuf(make_logo());

    gtk_status_icon_set_visible(status_icon, TRUE);
    gtk_status_icon_set_tooltip(status_icon, "QuickSynergy");

    g_signal_connect(G_OBJECT(status_icon), "popup-menu",
        G_CALLBACK(status_icon_popup), (gpointer) state);

    g_signal_connect(G_OBJECT(status_icon), "activate",
        G_CALLBACK(show_main_window), (gpointer) state);
#endif

    /* display the main window */
    gtk_widget_show_all(main_window);

    /* GTK mainloop */
    gtk_main();

    return 0;
}
