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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 *  MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "config.h"
#include "logo_data.h"
#include "proc.h"

#define VERSION "0.5"

extern GtkWidget *main_window;
extern GtkWidget *notebook;
extern GtkWidget *hostname_entry;
extern GtkWidget *table;
extern GtkWidget *vbox0;
extern int synergy_running;
extern int keep_running;
extern pid_t pid;

gboolean entry_focus_in_event(
        GtkWidget *widget, GdkEventFocus *event, gpointer data) {
	char *text;

	text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
	
	/* if the text on the entry is the default text (Above, Below, 
	 * Left, Right), erae it when the entry gets focus */
	if(!strcmp(text, (const char *) data))
		gtk_entry_set_text(GTK_ENTRY(widget), "");

	return FALSE;
}

gboolean entry_focus_out_event(
        GtkWidget *widget, GdkEventFocus *event, gpointer data) {
	char *text;

	text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
	
	/* if no text was written on the entry, get back to the default text
	 * when the entry loses focus */
	if(!strcmp(text, ""))
		gtk_entry_set_text(GTK_ENTRY(widget), (const gchar *) data);
	
	return FALSE;
}

void keep_running_toggled(GtkToggleButton *button, gpointer user_data) {
    if(gtk_toggle_button_get_active(button) == TRUE) {
        keep_running = 1;
    }
    else {
        keep_running = 0;
    }
}

void about_button_clicked(GtkWidget *widget, gpointer data) {
	GtkWidget *about;
    gchar *authors[2] = { "César L. B. Silveira", NULL };
    gchar *documenters[3] = { "Otávio C. Cordeiro", "César L. B. Silveira", NULL };

    /* build and display the about dialog */
    about = gtk_about_dialog_new();
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(about), "quicksynergy");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about), VERSION);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), 
            gdk_pixbuf_new_from_xpm_data((const char**)logo_data));
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about),
            "A GUI for synergy");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about), 
            "http://quicksynergy.sourceforge.net");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about), 
            (const gchar **) authors);
    gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about), 
            (const gchar **) documenters);

    gtk_widget_show_all(about);
}

void start_button_clicked(GtkWidget *widget, gpointer data) {
	struct entry_data *entries = (struct entry_data *) data;
	const char *env_home = getenv("HOME");
	char *filename, *hostname, *command;
	GtkWidget *stock;
	int status;
	
	if(!synergy_running) {	
		if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 0) {	
			save_config(entries);
			save_synergy_config(entries);
		
			filename = (char *) malloc(
				(strlen(env_home) + strlen("/.quicksynergy/synergy.conf")) * 
                sizeof(char));
		
			sprintf(filename, "%s%s", env_home, "/.quicksynergy/synergy.conf");
			
			if((pid = fork()) == 0) {
			    if(keep_running) {
    				execlp("synergys", "synergys", "--config",  filename, NULL);
    			}
    			else {
    			    execlp("synergys", "synergys", "-f", "--config",  filename, NULL);
    			}
    	    }
    	    else {
    	        if(keep_running)
    	            wait(&status);
    	    }
			
			gtk_widget_set_sensitive(notebook, FALSE);
			
			free(filename);
		}
		else if(gtk_notebook_get_current_page(
                    GTK_NOTEBOOK(notebook)) == 1) {
			hostname = 
                (char *) gtk_entry_get_text(GTK_ENTRY(hostname_entry));
			
			if((pid = fork()) == 0) {
			    if(keep_running) {
    				execlp("synergyc", "synergyc", hostname, NULL);
    			}
    			else {
        			execlp("synergyc", "synergyc", "-f", hostname, NULL);
        		}
        	}
        	else {
    	        if(keep_running)
    	            wait(&status);
    	    }
		}
		
    	gtk_button_set_label(GTK_BUTTON(widget), "Stop");		
		gtk_widget_set_sensitive(notebook, FALSE);
    
    	synergy_running = 1;
	}
	else {
		gtk_button_set_label(GTK_BUTTON(widget), "Start");
		
		if(keep_running) {
		    pid = get_pid_by_name("synergys");
		    
		    if(!pid)
		        pid = get_pid_by_name("synergyc");
		}
        
        kill(pid, SIGTERM);
        pid = 0;
        
        synergy_running = 0;
        
   		gtk_widget_set_sensitive(notebook, TRUE);
	}
}

void close_button_clicked(GtkWidget *widget, gpointer data) {	
	struct entry_data *entries = (struct entry_data *) data;
	
    if(synergy_running && !keep_running) {
        kill(pid, SIGTERM);
    }
    
    save_config(entries);
    
	gtk_main_quit();
}

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	struct entry_data *entries = (struct entry_data *) data;
	
    if(synergy_running && !keep_running) {
        kill(pid, SIGTERM);
    }
    
    save_config(entries);
    
    return FALSE;
}
