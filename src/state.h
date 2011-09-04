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

#ifndef STRUCTS_H
#define STRUCTS_H

#include <glib.h>

typedef struct qs_state {
    struct {
        gchar *above;
        gchar *below;
        gchar *left;
        gchar *right;
        gchar *hostname;
        gchar *client_name;
    } data;

    struct {
        int running;
        GPid pid;
    } proc;

    struct {
        int current_page;
    } ui;
} qs_state_t;

#endif
