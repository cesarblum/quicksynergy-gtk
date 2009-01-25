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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 *  MA 02111-1307, USA.
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "synergy_config.h"
#include "ui.h"
#include "intl.h"

#if GTK_CHECK_VERSION(2,10,0)
static GtkUIManager *ui_manager = NULL;
static GtkActionGroup *action_group;

static gchar popup_menu_actions_xml[] =
    "<ui>\n"
    "<popup>\n"
    "<menuitem action=\"ActionSettings\" />\n"
    "<separator />\n"
    "<menuitem action=\"ActionQuit\" />\n"
    "</popup>"
    "</ui>\n";

static GtkActionEntry popup_menu_actions[] = {
    { "ActionSettings", GTK_STOCK_PREFERENCES, N_("Settings"), NULL, NULL, show_main_window },
    { "ActionQuit",     GTK_STOCK_QUIT,        N_("Quit"),     NULL, NULL, quicksynergy_quit },    
};
#endif

pid_t pid;

extern GtkWidget *main_window;
extern GtkWidget *notebook;
extern const guint8 qslogo[];

gboolean entry_focus_in_event(
        GtkWidget *widget, GdkEventFocus *event, gpointer data) {
    char *text;

    text = (char *) gtk_entry_get_text(GTK_ENTRY(widget));
    
    /* if the text on the entry is the default text (Above, Below, 
     * Left, Right), erase it when the entry gets focus */
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

void entry_changed_cb(GtkEntry *entry, gpointer data) {
    *((char **) data) = (char *) gtk_entry_get_text(entry);
}

void about_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window = (GtkWidget *) data;
    
    static const gchar *authors[] = {
        "César L. B. Silveira <cesarbs@gmail.com>",
        "Otávio C. Cordeiro <otavio@gmail.com>",
        "Jérémie Corbier (patches and Debian and Ubuntu packaging)",
        "Thomas Langewouters (patches and suggestions)",
        "Magnus Boman (patches)",
        "Todd Wease (patches)",
        NULL
    };
    
    static const gchar *documenters[] = {
        "César L. B. Silveira <cesarbs@gmail.com>",
        "Otávio C. Cordeiro <otavio@gmail.com>",
        NULL
    };
    
    static const gchar *copyright =
        "Copyright \xc2\xa9 2006-2009 César L. B. Silveira, Otávio C. Cordeiro";
    
    GdkPixbuf *logo = gdk_pixbuf_new_from_inline(-1, qslogo, FALSE, NULL);

    gtk_show_about_dialog(GTK_WINDOW(window),
                    "comments", _("A graphical user interface for synergy"),
                    "authors", authors,
                    "documenters", documenters,
                    "copyright", copyright,
                    "logo", logo,
                    "translator-credits", _("translator-credits"),
                    "version", VERSION,
                    "website", "http://quicksynergy.sourceforge.net",
                    "name", "QuickSynergy",
                    NULL);

    if(logo) {
        g_object_unref(logo);
    }
}

void start_button_clicked(GtkWidget *widget, gpointer data) {
    qs_state_t *state = (qs_state_t *) data;
    const char *env_home = getenv("HOME");
    char *filename;
    int status;
    
    if(!state->running) {
        if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 0) {    
            save_config(state);
            save_synergy_config(state);
        
            filename = (char *) malloc(
                (strlen(env_home) + strlen("/.quicksynergy/synergy.conf") + 1) * 
                sizeof(char));
        
            sprintf(filename, "%s%s", env_home, "/.quicksynergy/synergy.conf");
            
            if((pid = fork()) == 0) {
                execlp("synergys", "synergys", "-f", "--config",  filename, NULL);
            }
            
            gtk_widget_set_sensitive(notebook, FALSE);
            
            free(filename);
            
            state->running = SYNERGY_SERVER_RUNNING;
        }
        else if(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) == 1) {
            save_config(state);

            if((pid = fork()) == 0) {
                execlp("synergyc", "synergyc", "-f", state->hostname, NULL);
            }

            state->running = SYNERGY_CLIENT_RUNNING;
        }
        
        gtk_button_set_label(GTK_BUTTON(widget), GTK_STOCK_MEDIA_STOP);
        gtk_widget_set_sensitive(notebook, FALSE);
    }
    else {
        kill(pid, SIGTERM);

        wait(&status);
        
        gtk_button_set_label(GTK_BUTTON(widget), GTK_STOCK_EXECUTE);
        gtk_widget_set_sensitive(notebook, TRUE);
        
        state->running = 0;
    }
}

#if GTK_CHECK_VERSION(2,10,0)
void quicksynergy_quit(GtkWidget *widget, gpointer data) {
    qs_state_t *state = (qs_state_t *) data;
    
    if(state->running) {
        kill(pid, SIGTERM);
    }
    
    save_config(state);
    
    gtk_main_quit();
}

void close_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_hide_all(main_window);
}

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_widget_hide_all(widget);
    
    return TRUE;
}
#else
void close_button_clicked(GtkWidget *widget, gpointer data) {   
    qs_state_t *state = (qs_state_t *) data;
    
    if(state->running) {
        kill(pid, SIGTERM);
    }
    
    save_config(state);
    
    gtk_main_quit();
}

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    qs_state_t *state = (qs_state_t *) data;
    
    if(state->running) {
        kill(pid, SIGTERM);
    }
    
    save_config(state);
    
    return FALSE;
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
void show_main_window(GtkWidget *widget, gpointer data) {
    qs_state_t *state = (qs_state_t *) data;
    
    gtk_widget_show_all(main_window);
    
    if(!state->running || state->running == SYNERGY_SERVER_RUNNING) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
    }
    else {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
    }
}

void status_icon_popup(GtkStatusIcon *status_icon, guint button,
        guint activate_time, gpointer data) {
    GtkWidget *popup_menu;
    
    if(!ui_manager) {
        ui_manager = gtk_ui_manager_new();
        action_group = gtk_action_group_new("QSpopup");
        
#ifdef ENABLE_NLS
        gtk_action_group_set_translation_domain(action_group, PACKAGE);
#endif
        gtk_action_group_add_actions(action_group, popup_menu_actions, 2, data);
        
        gtk_ui_manager_insert_action_group(ui_manager, action_group, 0);
        gtk_ui_manager_add_ui_from_string(ui_manager, popup_menu_actions_xml,
            -1, NULL);
    }
    
    popup_menu = gtk_ui_manager_get_widget(ui_manager, "/popup");
    
    gtk_widget_show_all(popup_menu);
    
    gtk_menu_popup(GTK_MENU(popup_menu), NULL, NULL,
        gtk_status_icon_position_menu, status_icon, button, activate_time);
}
#endif
