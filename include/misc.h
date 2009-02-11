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

#ifndef MISC_H
#define MISC_H

#include <gtk/gtk.h>

struct entry_data {
	GtkWidget *above_entry;
	GtkWidget *below_entry;
	GtkWidget *left_entry;
	GtkWidget *right_entry;
};

#endif
