#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <gtk/gtk.h>
#include "include/config.h"
#include "include/logo_data.h"
#include "include/proc.h"

extern GtkWidget *main_window;
extern GtkWidget *notebook;
extern GtkWidget *hostname_entry;
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
	
	if(!strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Start")) {
		gtk_button_set_label(GTK_BUTTON(widget), "Stop");
		stock = gtk_image_new_from_stock(GTK_STOCK_STOP, GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(widget), stock);
		
		gtk_widget_set_sensitive(notebook, FALSE);
	
		if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 0) {	
			save_config(entries);
			save_synergy_config(entries);
		
			filename = (char *) malloc(
				(strlen(env_home) + strlen("/.synergy.conf")) * sizeof(char));
		
			sprintf(filename, "%s%s", env_home, "/.synergy.conf");
			
			command = (char *) malloc(19 + strlen(filename));
			strcpy(command, "synergys --config ");
			strcat(command, filename);
		
			system(command);
			
			free(command);
			free(filename);
		} 
		else if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 1) {
			hostname = (char *) gtk_entry_get_text(GTK_ENTRY(hostname_entry));
			
			command = (char *) malloc(9 + strlen(hostname));
			strcpy(command, "synergyc ");
			strcat(command, hostname);
		
			system(command);
			
			free(command);
		}
	}
	else {
		gtk_button_set_label(GTK_BUTTON(widget), "Start");
		stock = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(widget), stock);
		
		gtk_widget_set_sensitive(notebook, TRUE);
		
		if(!(pid = get_process_by_name("synergys")))
			pid = get_process_by_name("synergyc");
		
		kill(pid, SIGKILL);
	}
}

gboolean app_quit(GtkWidget *widget, GdkEvent *event, gpointer data) {
	struct entry_data *entries = (struct entry_data *) data;
		
	save_config(entries);
	
	if(pid)
		kill(pid, SIGKILL);
	
	return FALSE;
}
