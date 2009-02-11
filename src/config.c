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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "include/misc.h"

void load_config(struct entry_data *entries) {
	const char *env_home = getenv("HOME");
	char *filename, option[16], value[32];
	FILE *f;
	
	filename = (char *) malloc(
		(strlen(env_home) + strlen("/.quicksynergy.conf")) * sizeof(char));
	
	sprintf(filename, "%s%s", env_home, "/.quicksynergy.conf");
	
	if((f = fopen(filename, "r"))) {
		do {
			if(fscanf(f, "%s%s", option, value) != EOF) {
				if(!strcmp(option, "Above"))
					gtk_entry_set_text(GTK_ENTRY(entries->above_entry), value);
				else if(!strcmp(option, "Below"))
					gtk_entry_set_text(GTK_ENTRY(entries->below_entry), value);
				else if(!strcmp(option, "Left"))
					gtk_entry_set_text(GTK_ENTRY(entries->left_entry), value);
				else if(!strcmp(option, "Right"))
					gtk_entry_set_text(GTK_ENTRY(entries->right_entry), value);
			}
		} while(!feof(f));
	}
}

void save_config(struct entry_data *entries) {
	const char *env_home = getenv("HOME");
	char *filename;
	FILE *f;
	
	filename = (char *) malloc(
		(strlen(env_home) + strlen("/.quicksynergy.conf")) * sizeof(char));
	
	sprintf(filename, "%s%s", env_home, "/.quicksynergy.conf");
	
	f = fopen(filename, "w");
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above") &&
	   strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), ""))
        fprintf(f, "Above %s\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below") &&
	   strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), ""))
        fprintf(f, "Below %s\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left") &&
	   strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), ""))
        fprintf(f, "Left %s\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
	
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right") &&
	   strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), ""))
        fprintf(f, "Right %s\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
	
	fclose(f);
}

void save_synergy_config(struct entry_data *entries) {
	const char *env_home = getenv("HOME");
	char *filename, hostname[64];
	FILE *f;
	
	filename = (char *) malloc(
		(strlen(env_home) + strlen("/.synergy.conf")) * sizeof(char));
	
	sprintf(filename, "%s%s", env_home, "/.synergy.conf");
	
	f = fopen(filename, "w");
	
	gethostname(hostname, 64);

    fprintf(f, "section: screens\n");
    fprintf(f, "\t%s:\n", hostname);
	
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above"))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below"))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
	
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left"))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right"))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
	
    fprintf(f, "end\n");
	
    /* server links */
	fprintf(f, "section: links\n");
    fprintf(f, "\t%s:\n", hostname);
	
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above")) 
        fprintf(f, "\t\tup = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below"))
        fprintf(f, "\t\tdown = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left"))
        fprintf(f, "\t\tleft = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
	
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right"))
        fprintf(f, "\t\tright = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
	
    /* client links */
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above")) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
        fprintf(f, "\t\tdown = %s\n", hostname);
    }
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below")) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
        fprintf(f, "\t\tup = %s\n", hostname);
    }
	
	if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left")) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
        fprintf(f, "\t\tright = %s\n", hostname);
    }
	
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right")) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
        fprintf(f, "\t\tleft = %s\n", hostname);
    }
	
    fprintf(f, "end\n");

    fclose(f);
}
