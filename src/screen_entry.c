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
 
#include "screen_entry.h"
#include "callbacks.h"

GtkWidget *screen_entry_new(const char *position) {
    GtkWidget *entry;

    entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(entry), position);
    gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
    gtk_entry_set_width_chars(GTK_ENTRY(entry), 10);

    g_signal_connect(G_OBJECT(entry), "focus-in-event",
    G_CALLBACK(entry_focus_in_event), (gpointer) position);

    g_signal_connect(G_OBJECT(entry), "focus-out-event",
    G_CALLBACK(entry_focus_out_event), (gpointer) position);

    return entry;
}
