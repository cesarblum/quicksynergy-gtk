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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "synergy_config.h"
#include "intl.h"

extern GtkWidget *above_entry, *below_entry, *left_entry, *right_entry, *hostname_entry;

void load_config() {
    const char *env_home = getenv("HOME");
    char *filename, option[16], value[32];
    FILE *f;
    
    filename = (char *) malloc(
        (strlen(env_home) + strlen("/.quicksynergy/quicksynergy.conf") + 1) * 
        sizeof(char));
    
    sprintf(filename, "%s%s", env_home, "/.quicksynergy/quicksynergy.conf");
    
    if((f = fopen(filename, "r"))) {
        do {
            if(fscanf(f, "%s%s", option, value) != EOF) {
                if(!strcmp(option, "Above"))
                    gtk_entry_set_text(
                            GTK_ENTRY(above_entry), value);
                else if(!strcmp(option, "Below"))
                    gtk_entry_set_text(
                            GTK_ENTRY(below_entry), value);
                else if(!strcmp(option, "Left"))
                    gtk_entry_set_text(
                            GTK_ENTRY(left_entry), value);
                else if(!strcmp(option, "Right"))
                    gtk_entry_set_text(
                            GTK_ENTRY(right_entry), value);
                else if(!strcmp(option, "Host"))
                    gtk_entry_set_text(
                            GTK_ENTRY(hostname_entry), value);
            }
        } while(!feof(f));
    }
}

void save_config() {
    const char *env_home = getenv("HOME");
    char *filename;
    FILE *f;
    
    filename = (char *) malloc(
        (strlen(env_home) + strlen("/.quicksynergy") + 1) * sizeof(char));
        
    sprintf(filename, "%s%s", env_home, "/.quicksynergy");

    mkdir(filename, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    
    free(filename);
    
    filename = (char *) malloc(
        (strlen(env_home) + strlen("/.quicksynergy/quicksynergy.conf") + 1) * 
        sizeof(char));
    
    sprintf(filename, "%s%s", env_home, "/.quicksynergy/quicksynergy.conf");
    
    f = fopen(filename, "w");
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(above_entry)), _("Above")) &&
       strcmp(gtk_entry_get_text(GTK_ENTRY(above_entry)), ""))
        fprintf(f, "Above %s\n", gtk_entry_get_text(GTK_ENTRY(above_entry)));

    if(strcmp(gtk_entry_get_text(GTK_ENTRY(below_entry)), _("Below")) &&
       strcmp(gtk_entry_get_text(GTK_ENTRY(below_entry)), ""))
        fprintf(f, "Below %s\n", gtk_entry_get_text(GTK_ENTRY(below_entry)));

    if(strcmp(gtk_entry_get_text(GTK_ENTRY(left_entry)), _("Left")) &&
       strcmp(gtk_entry_get_text(GTK_ENTRY(left_entry)), ""))
        fprintf(f, "Left %s\n", gtk_entry_get_text(GTK_ENTRY(left_entry)));

    if(strcmp(gtk_entry_get_text(GTK_ENTRY(right_entry)), _("Right")) &&
       strcmp(gtk_entry_get_text(GTK_ENTRY(right_entry)), ""))
        fprintf(f, "Right %s\n", gtk_entry_get_text(GTK_ENTRY(right_entry)));

    fprintf(f, "Host %s\n", gtk_entry_get_text(GTK_ENTRY(hostname_entry)));
    
    fclose(f);
}

void save_synergy_config() {
    const char *env_home = getenv("HOME");
    char *filename, hostname[64];
    FILE *f;
    
    filename = (char *) malloc(
        (strlen(env_home) + strlen("/.quicksynergy") + 1) * sizeof(char));
    
    sprintf(filename, "%s%s", env_home, "/.quicksynergy");

    mkdir(filename, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    
    free(filename);
    
    filename = (char *) malloc(
        (strlen(env_home) + strlen("/.quicksynergy/synergy.conf") + 1) * 
        sizeof(char));
    
    sprintf(filename, "%s%s", env_home, "/.quicksynergy/synergy.conf");
    
    f = fopen(filename, "w");
    
    gethostname(hostname, 64);

    fprintf(f, "section: screens\n");
    fprintf(f, "\t%s:\n", hostname);
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(above_entry)), _("Above")))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(above_entry)));
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(below_entry)), _("Below")))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(below_entry)));
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(left_entry)), _("Left")))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(left_entry)));
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(right_entry)), _("Right")))
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(right_entry)));
    
    fprintf(f, "end\n");
    
    /* server links */
    fprintf(f, "section: links\n");
    fprintf(f, "\t%s:\n", hostname);
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(above_entry)), _("Above")))
        fprintf(f, "\t\tup = %s\n", gtk_entry_get_text(GTK_ENTRY(above_entry)));
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(below_entry)), _("Below")))
        fprintf(f, "\t\tdown = %s\n", gtk_entry_get_text(GTK_ENTRY(below_entry)));
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(left_entry)), _("Left")))
        fprintf(f, "\t\tleft = %s\n", gtk_entry_get_text(GTK_ENTRY(left_entry)));
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(right_entry)), _("Right")))
        fprintf(f, "\t\tright = %s\n", gtk_entry_get_text(GTK_ENTRY(right_entry)));
    
    /* client links */
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(above_entry)), _("Above"))) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(above_entry)));
        fprintf(f, "\t\tdown = %s\n", hostname);
    }
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(below_entry)), _("Below"))) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(below_entry)));
        fprintf(f, "\t\tup = %s\n", hostname);
    }
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(left_entry)), _("Left"))) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(left_entry)));
        fprintf(f, "\t\tright = %s\n", hostname);
    }
    
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(right_entry)), _("Right"))) {
        fprintf(f, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(right_entry)));
        fprintf(f, "\t\tleft = %s\n", hostname);
    }
    
    fprintf(f, "end\n");

    fclose(f);
}
