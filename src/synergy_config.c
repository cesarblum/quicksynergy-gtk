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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glib.h>
#include "synergy_config.h"
#include "ui.h"
#include "intl.h"

qs_state_t *load_config() {
    const char *home_dir = getenv("HOME");
    qs_state_t *state;
    GKeyFile *key_file;
    
    chdir(home_dir);
    
    key_file = g_key_file_new();
    g_key_file_load_from_file(key_file, QS_CONF_DIR QS_CONF_FILE,
        G_KEY_FILE_NONE, NULL);
    
    state = (qs_state_t *) malloc(sizeof(qs_state_t));
    
    state->above =
        (g_key_file_has_key(key_file, "Share", "Above", NULL)   ?
         g_key_file_get_value(key_file, "Share", "Above", NULL) :
         _("Above"));

    state->below =
        (g_key_file_has_key(key_file, "Share", "Below", NULL)   ?
         g_key_file_get_value(key_file, "Share", "Below", NULL) :
         _("Below"));

    state->left =
        (g_key_file_has_key(key_file, "Share", "Left", NULL) ?
         g_key_file_get_value(key_file, "Share", "Left", NULL) :
         _("Left"));

    state->right =
        (g_key_file_has_key(key_file, "Share", "Right", NULL) ?
         g_key_file_get_value(key_file, "Share", "Right", NULL) :
         _("Right"));

    state->hostname =
        (g_key_file_has_key(key_file, "Use", "Hostname", NULL) ?
         g_key_file_get_value(key_file, "Use", "Hostname", NULL) :
         "");

    state->client_name =
        (g_key_file_has_key(key_file, "Use", "ClientName", NULL) ?
         g_key_file_get_value(key_file, "Use", "ClientName", NULL) :
         "");

    state->synergy_path =
        (g_key_file_has_key(key_file, "Settings", "SynergyPath", NULL) ?
         g_key_file_get_value(key_file, "Settings", "SynergyPath", NULL) :
         "/usr/bin");

    g_key_file_free(key_file);

    return state;
}

void save_config(qs_state_t *state) {
    const char *home_dir = getenv("HOME");
    GKeyFile *key_file;
    gsize length;
    gchar *data;
    
    chdir(home_dir);
    
    mkdir(QS_CONF_DIR, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    
    key_file = g_key_file_new();
    
    if(g_strcmp0(state->above, _("Above"))) {
        g_key_file_set_value(key_file, "Share", "Above", state->above);
    }

    if(g_strcmp0(state->below, _("Below"))) {
        g_key_file_set_value(key_file, "Share", "Below", state->below);
    }

    if(g_strcmp0(state->left, _("Left"))) {
        g_key_file_set_value(key_file, "Share", "Left", state->left);
    }

    if(g_strcmp0(state->right, _("Right"))) {
        g_key_file_set_value(key_file, "Share", "Right", state->right);
    }

    g_key_file_set_value(key_file, "Use", "Hostname", state->hostname);
    
    g_key_file_set_value(key_file, "Use", "ClientName", state->client_name);
    
    g_key_file_set_value(key_file, "Settings", "SynergyPath",
        state->synergy_path);
    
    data = g_key_file_to_data(key_file, &length, NULL);
    g_file_set_contents(QS_CONF_DIR QS_CONF_FILE, data, length, NULL);
    
    g_key_file_free(key_file);
}

void save_synergy_config(qs_state_t *state) {
    const char *home_dir = getenv("HOME");
    char hostname[64];
    FILE *f;
    
    chdir(home_dir);
    
    mkdir(QS_CONF_DIR, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    
    f = fopen(QS_CONF_DIR QS_SYNERGY_CONF_FILE, "w");
    
    gethostname(hostname, 64);

    fprintf(f, "section: screens\n");
    fprintf(f, "\t%s:\n", hostname);
    
    if(strcmp(state->above, _("Above"))) {
        fprintf(f, "\t%s:\n", state->above);
    }
    
    if(strcmp(state->below, _("Below"))) {
        fprintf(f, "\t%s:\n", state->below);
    }
    
    if(strcmp(state->left, _("Left"))) {
        fprintf(f, "\t%s:\n", state->left);
    }
    
    if(strcmp(state->right, _("Right"))) {
        fprintf(f, "\t%s:\n", state->right);
    }
    
    fprintf(f, "end\n");
    
    /* server links */
    fprintf(f, "section: links\n");
    fprintf(f, "\t%s:\n", hostname);
    
    if(strcmp(state->above, _("Above"))) {
        fprintf(f, "\t\tup = %s\n", state->above);
    }
    
    if(strcmp(state->below, _("Below"))) {
        fprintf(f, "\t\tdown = %s\n", state->below);
    }
    
    if(strcmp(state->left, _("Left"))) {
        fprintf(f, "\t\tleft = %s\n", state->left);
    }
    
    if(strcmp(state->right, _("Right"))) {
        fprintf(f, "\t\tright = %s\n", state->right);
    }
    
    /* client links */
    if(strcmp(state->above, _("Above"))) {
        fprintf(f, "\t%s:\n", state->above);
        fprintf(f, "\t\tdown = %s\n", hostname);
    }
    
    if(strcmp(state->below, _("Below"))) {
        fprintf(f, "\t%s:\n", state->below);
        fprintf(f, "\t\tup = %s\n", hostname);
    }
    
    if(strcmp(state->left, _("Left"))) {
        fprintf(f, "\t%s:\n", state->left);
        fprintf(f, "\t\tright = %s\n", hostname);
    }
    
    if(strcmp(state->right, _("Right"))) {
        fprintf(f, "\t%s:\n", state->right);
        fprintf(f, "\t\tleft = %s\n", hostname);
    }
    
    fprintf(f, "end\n");

    fclose(f);
}
