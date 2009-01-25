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
#include "intl.h"

#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "logo/qslogo.h"
#include "callbacks.h"
#include "synergy_config.h"
#include "ui.h"

GdkPixbuf *make_logo() {
    return gdk_pixbuf_new_from_inline(-1, qslogo, FALSE, NULL);
}

GtkWidget *screen_entry_new(char **textp, const char *position) {
    GtkWidget *entry;

    entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(entry), *textp);
    gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
    gtk_entry_set_width_chars(GTK_ENTRY(entry), 10);

    g_signal_connect(G_OBJECT(entry), "focus-in-event",
        G_CALLBACK(entry_focus_in_event), (gpointer) position);

    g_signal_connect(G_OBJECT(entry), "focus-out-event",
        G_CALLBACK(entry_focus_out_event), (gpointer) position);
    
    g_signal_connect(G_OBJECT(entry), "changed",
        G_CALLBACK(entry_changed_cb), (gpointer) textp);

    return entry;
}

GtkWidget *make_server_tab(qs_state_t *state) {
    GtkWidget *table;
    GtkWidget *image;
    GtkWidget *above_entry;
    GtkWidget *below_entry;
    GtkWidget *left_entry;
    GtkWidget *right_entry;
    
    /* build the table that will hold the server layout widgets */
    table = gtk_table_new(3, 3, TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(table), 12);
    
    /* text entries for server configuration */
    above_entry = screen_entry_new(&state->above, _("Above"));
    below_entry = screen_entry_new(&state->below, _("Below"));
    left_entry = screen_entry_new(&state->left, _("Left"));
    right_entry = screen_entry_new(&state->right, _("Right"));
    
    /* attach entries to table */
    gtk_table_attach_defaults(GTK_TABLE(table), above_entry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), below_entry, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), left_entry, 0, 1, 1, 2);            
    gtk_table_attach_defaults(GTK_TABLE(table), right_entry, 2, 3, 1, 2);       
    
    /* image to be displayed in the center of the main window */
    image = gtk_image_new_from_pixbuf(make_logo());
    gtk_table_attach_defaults(GTK_TABLE(table), image, 1, 2, 1, 2);
    
    return table;
}

GtkWidget *make_client_tab(qs_state_t *state) {
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *entry;
    
    /* client's vbox */
    vbox = gtk_vbox_new(FALSE, 6);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 12);
    
    label = gtk_label_new(_("Server hostname/IP address:"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    
    /* entry for placing the server's hostname/IP address */
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    /* update state information when text changes */
    g_signal_connect(G_OBJECT(entry), "changed",
        G_CALLBACK(entry_changed_cb), (gpointer) &state->hostname);
    
    return vbox;
}

GtkWidget *make_settings_tab(qs_state_t *state) {
    GtkWidget *vbox_content;
    GtkWidget *vbox_section;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *entry;
    
    vbox_content = gtk_vbox_new(FALSE, 18);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_content), 12);
    
    /* "Paths" section */
    vbox_section = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_content), vbox_section, FALSE, FALSE, 0);
    
    label = gtk_label_new("");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_label_set_markup(GTK_LABEL(label), _("<b>Paths</b>"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(vbox_section), label, FALSE, FALSE, 0);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_section), hbox, TRUE, TRUE, 0);
    
    vbox = gtk_vbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    
    label = gtk_label_new(_("Path to synergys:"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_misc_set_padding(GTK_MISC(label), 12, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    
    label = gtk_label_new(_("Path to synergyc:"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_misc_set_padding(GTK_MISC(label), 12, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    
    vbox = gtk_vbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);
    
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);

    /* update state information when text changes */
    g_signal_connect(G_OBJECT(entry), "changed",
        G_CALLBACK(entry_changed_cb), (gpointer) &state->synergys_path);
    
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);
 
    /* update state information when text changes */
    g_signal_connect(G_OBJECT(entry), "changed",
        G_CALLBACK(entry_changed_cb), (gpointer) &state->synergyc_path);
    
    vbox = gtk_vbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    
    button = gtk_button_new_with_label("...");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    
    button = gtk_button_new_with_label("...");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    
    /* "Screen name" section */
    vbox_section = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_content), vbox_section, FALSE, FALSE, 0);
    
    label = gtk_label_new("");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_label_set_markup(GTK_LABEL(label), _("<b>Screen name</b>"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(vbox_section), label, FALSE, FALSE, 0);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_section), hbox, TRUE, TRUE, 0);
    
    label = gtk_label_new(_("Set this computer's screen name:"));
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_misc_set_padding(GTK_MISC(label), 12, 0);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
    
    /* update state information when text changes */
    g_signal_connect(G_OBJECT(entry), "changed",
        G_CALLBACK(entry_changed_cb), (gpointer) &state->screen_name);

    return vbox_content;
}

