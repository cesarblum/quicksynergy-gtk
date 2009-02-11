/* Created by Anjuta version 1.2.4 */
/*	This file will not be overwritten */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gnome.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glade/glade.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "include/prog_image_data.h"

struct entries {
    GtkWidget *above_entry;
    GtkWidget *right_entry;
    GtkWidget *below_entry;
    GtkWidget *left_entry;
};

void save_config(const char *filename, const char *hostname, struct entries *entries);

GtkWidget *notebook;
GtkWidget *hostname_entry;
int server_running;
int client_running;
int pid;

/* callbacks */
void show_about(GtkWidget *widget) {
    GtkWidget *about;
    gchar *authors[2] = { "César L. B. Silveira", NULL };

    about = gtk_about_dialog_new();
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(about), "QuickSynergy");
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), gdk_pixbuf_new_from_xpm_data((const char**)image_data));
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about),
                                  "A simple GUI for setting up synergy clients and servers");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about), "http://uud.f2o.org/QuickSynergy/index.html");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about), (const gchar **) authors);

    gtk_widget_show_all(about);
}

gboolean entry_focus_in_event(GtkWidget *widget,  GdkEventFocus *event, gpointer user_data) {
    char *text;

    text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
    if(!strcmp(text, (const char *) user_data))
        gtk_entry_set_text(GTK_ENTRY(widget), "");

    return FALSE;
}

gboolean entry_focus_out_event(GtkWidget *widget,  GdkEventFocus *event, gpointer user_data) {
    char *text;

    text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
    if(!strcmp(text, ""))
        gtk_entry_set_text(GTK_ENTRY(widget), (const gchar *) user_data);

    return FALSE;
}

void start_cb(GtkWidget *widget, gpointer user_data) {
    GtkWidget *image;
    char *label, hostname[32], *filename, *remote_host;
    int status;

    if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 0) {
        if(!server_running) {
            gtk_button_set_label(GTK_BUTTON(widget), "Stop");
            image = gtk_image_new_from_stock(GTK_STOCK_STOP, GTK_ICON_SIZE_BUTTON);
            gtk_button_set_image(GTK_BUTTON(widget), image);

            gethostname(hostname, 32);
            filename = (char *) malloc((strlen(getenv("HOME"))+strlen("/.synergy.conf"))*sizeof(char));
            strcpy(filename, getenv("HOME"));
            strcat(filename, "/.synergy.conf");

            save_config(filename, hostname, (struct entries *) user_data);

            if((pid = fork()) == 0)
                execlp("synergys", "synergys", "--config", filename, (char *) NULL);
            else wait(&status);

            gtk_widget_set_sensitive(GTK_WIDGET(notebook), FALSE);

            server_running = 1;
        } else {
            kill(pid+1, SIGKILL);
            server_running = 0;

            gtk_button_set_label(GTK_BUTTON(widget), "Start");
            image = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
            gtk_button_set_image(GTK_BUTTON(widget), image);

            gtk_widget_set_sensitive(GTK_WIDGET(notebook), TRUE);
        }
    } else {
        if(!client_running) {  
            gtk_button_set_label(GTK_BUTTON(widget), "Stop");
            image = gtk_image_new_from_stock(GTK_STOCK_STOP, GTK_ICON_SIZE_BUTTON);
            gtk_button_set_image(GTK_BUTTON(widget), image);
            
            remote_host = (char *) gtk_entry_get_text(GTK_ENTRY(hostname_entry));

            if((pid = fork()) == 0)
                execlp("synergyc", "synergyc", remote_host, (char *) NULL);
            else wait(&status);

            gtk_widget_set_sensitive(GTK_WIDGET(notebook), FALSE);

            client_running = 1;
        } else {
            kill(pid+1, SIGKILL);
            client_running = 0;

            gtk_button_set_label(GTK_BUTTON(widget), "Start");
            image = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
            gtk_button_set_image(GTK_BUTTON(widget), image);

            gtk_widget_set_sensitive(GTK_WIDGET(notebook), TRUE);
        }
    }
}

/* miscellaneous functions */
void save_config(const char *filename, const char *hostname, struct entries *entries) {
    FILE *fp;

    fp = fopen(filename, "w");

    fprintf(fp, "section: screens\n");
    fprintf(fp, "\t%s:\n", hostname);
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
    }
    fprintf(fp, "end\n");
    fprintf(fp, "section: links\n");
    /* server links */
    fprintf(fp, "\t%s:\n", hostname);
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above")) {
        fprintf(fp, "\t\tup = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right")) {
        fprintf(fp, "\t\tright = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below")) {
        fprintf(fp, "\t\tdown = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left")) {
        fprintf(fp, "\t\tleft = %s\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
    }
    /* client links */
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->above_entry)), "Above")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->above_entry)));
        fprintf(fp, "\t\tdown = %s\n", hostname);
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->right_entry)), "Right")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->right_entry)));
        fprintf(fp, "\t\tleft = %s\n", hostname);
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->below_entry)), "Below")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->below_entry)));
        fprintf(fp, "\t\tup = %s\n", hostname);
    }
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(entries->left_entry)), "Left")) {
        fprintf(fp, "\t%s:\n", gtk_entry_get_text(GTK_ENTRY(entries->left_entry)));
        fprintf(fp, "\t\tright = %s\n", hostname);
    }
    fprintf(fp, "end\n");

    fclose(fp);
}

int main(int argc, char *argv[])
{
	struct entries entries;
    GtkWidget *window1;
    GtkWidget *start_button;
    GtkWidget *image;
    GtkWidget *prog_image;
    GladeXML *xml;

	#ifdef ENABLE_NLS
		bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
		textdomain(PACKAGE);
	#endif

	gnome_init(PACKAGE, VERSION, argc, argv);
	glade_gnome_init();
	/*
	 * The .glade filename should be on the next line.
	 */
	xml = glade_xml_new(PACKAGE_SOURCE_DIR"/QuickSynergy.glade", NULL, NULL);

	glade_xml_signal_autoconnect(xml);
    window1 = glade_xml_get_widget(xml, "window1");
    entries.above_entry = glade_xml_get_widget(xml, "above_entry");
    entries.right_entry = glade_xml_get_widget(xml, "right_entry");
    entries.below_entry = glade_xml_get_widget(xml, "below_entry");
    entries.left_entry = glade_xml_get_widget(xml, "left_entry");
    start_button = glade_xml_get_widget(xml, "start_button");
    notebook = glade_xml_get_widget(xml, "notebook1");
    prog_image = glade_xml_get_widget(xml, "prog_image");
    hostname_entry = glade_xml_get_widget(xml, "hostname_entry");
    gtk_image_set_from_pixbuf(GTK_IMAGE(prog_image), gdk_pixbuf_new_from_xpm_data((const char **) image_data));
    gtk_button_set_label(GTK_BUTTON(start_button), "Start");
    image = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(start_button), image);
    gtk_entry_set_alignment(GTK_ENTRY(entries.above_entry), 0.5);
    gtk_entry_set_alignment(GTK_ENTRY(entries.right_entry), 0.5);
    gtk_entry_set_alignment(GTK_ENTRY(entries.below_entry), 0.5);
    gtk_entry_set_alignment(GTK_ENTRY(entries.left_entry), 0.5);
    g_signal_connect(G_OBJECT(entries.above_entry), "focus-in-event",
            G_CALLBACK(entry_focus_in_event), (gpointer) "Above");
    g_signal_connect(G_OBJECT(entries.right_entry), "focus-in-event",
            G_CALLBACK(entry_focus_in_event), (gpointer) "Right");
    g_signal_connect(G_OBJECT(entries.below_entry), "focus-in-event",
            G_CALLBACK(entry_focus_in_event), (gpointer) "Below");
    g_signal_connect(G_OBJECT(entries.left_entry), "focus-in-event",
            G_CALLBACK(entry_focus_in_event), (gpointer) "Left");
    g_signal_connect(G_OBJECT(entries.above_entry), "focus-out-event",
            G_CALLBACK(entry_focus_out_event), (gpointer) "Above");
    g_signal_connect(G_OBJECT(entries.right_entry), "focus-out-event",
            G_CALLBACK(entry_focus_out_event), (gpointer) "Right");
    g_signal_connect(G_OBJECT(entries.below_entry), "focus-out-event",
            G_CALLBACK(entry_focus_out_event), (gpointer) "Below");
    g_signal_connect(G_OBJECT(entries.left_entry), "focus-out-event",
            G_CALLBACK(entry_focus_out_event), (gpointer) "Left");
    g_signal_connect(G_OBJECT(start_button), "clicked", G_CALLBACK(start_cb), (gpointer)&entries);
    gtk_widget_show(window1);

    server_running = 0;
    client_running = 0;

    gtk_main();

    return 0;
}
