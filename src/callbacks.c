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

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "include/config.h"
#include "include/logo_data.h"
#include "include/proc.h"

extern GtkWidget *main_window;
extern GtkWidget *notebook;
extern GtkWidget *hostname_entry;
extern GtkWidget *table;
extern GtkWidget *vbox0;
extern pid_t pid;

gboolean entry_focus_in_event(GtkWidget *widget, GdkEventFocus *event, gpointer data) {
	char *text;

	text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
	
	if(!strcmp(text, (const char *) data))
		gtk_entry_set_text(GTK_ENTRY(widget), "");

	return FALSE;
}

gboolean entry_focus_out_event(GtkWidget *widget, GdkEventFocus *event, gpointer data) {
	char *text;

	text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
	
	if(!strcmp(text, ""))
		gtk_entry_set_text(GTK_ENTRY(widget), (const gchar *) data);
	
	return FALSE;
}

void about_button_clicked(GtkWidget *widget, gpointer data) {
	GtkWidget *about;
    gchar *authors[2] = { "César L. B. Silveira", NULL };
    gchar *documenters[3] = { "Otávio C. Cordeiro", "César L. B. Silveira", NULL };

    about = gtk_about_dialog_new();
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(about), "QuickSynergy");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about), "0.3");
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), gdk_pixbuf_new_from_xpm_data((const char**)logo_data));
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about),
            "A GUI for setting up synergy clients and servers");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about), "http://quicksynergy.sourceforge.net");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about), (const gchar **) authors);
    gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about), (const gchar **) documenters);

    gtk_widget_show_all(about);
}

void start_button_clicked(GtkWidget *widget, gpointer data) {
	struct entry_data *entries = (struct entry_data *) data;
	const char *env_home = getenv("HOME");
	char *filename, *hostname, *command;
	GtkWidget *stock;
	int status;
	
	if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Start")) {
		gtk_button_set_label(GTK_BUTTON(widget), "Stop");
		stock = gtk_image_new_from_stock(GTK_STOCK_STOP, GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(widget), stock);
		
		//gtk_widget_set_sensitive(notebook, FALSE);
	
		if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 0) {	
			gtk_widget_set_sensitive(table, FALSE);
			
			save_config(entries);
			save_synergy_config(entries);
		
			filename = (char *) malloc(
				(strlen(env_home) + strlen("/.synergy.conf")) * sizeof(char));
		
			sprintf(filename, "%s%s", env_home, "/.synergy.conf");
			
			/*
			command = (char *) malloc(19 + strlen(filename));
			strcpy(command, "synergys -f --config ");
			strcat(command, filename);
			*/
			
			if(fork() == 0)
				execlp("synergys", "synergys", "--config", filename, NULL);
			else
				wait(&status);
			
			//free(command);
			free(filename);
		}
		else if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 1) {
			gtk_widget_set_sensitive(vbox0, FALSE);

			hostname = (char *) gtk_entry_get_text(GTK_ENTRY(hostname_entry));
			
			/*
			command = (char *) malloc(9 + strlen(hostname));
			strcpy(command, "synergyc ");
			strcat(command, hostname);
			*/
			
			if(fork() == 0)
				execlp("synergyc", "synergyc", hostname, NULL);
			else
				wait(&status);
		
			//system(command);
			
			//free(command);
		}
	}
	else {
		gtk_button_set_label(GTK_BUTTON(widget), "Start");
		stock = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(widget), stock);
		
		gtk_widget_set_sensitive(table, TRUE);
		gtk_widget_set_sensitive(vbox0, TRUE);
		
		if(!(pid = get_pid_by_name("synergys")))
			pid = get_pid_by_name("synergyc");
		
		if(pid) {
    		kill(pid, SIGTERM);
            pid = 0;
        }
	}
}

void close_button_clicked(GtkWidget *widget, gpointer data) {
	if(!(pid = get_pid_by_name("synergys")))
		pid = get_pid_by_name("synergyc");
		
	if(pid) kill(pid, SIGKILL);
	
	gtk_main_quit();
}
