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
#include "ui.h"
#include "intl.h"

void load_config(qs_state_t *state) {
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
                if(!strcmp(option, "Above")) {
                    state->above = strdup(value);
                } else if(!strcmp(option, "Below")) {
                    state->below = strdup(value);
                } else if(!strcmp(option, "Left")) {
                    state->left = strdup(value);
                } else if(!strcmp(option, "Right")) {
                    state->right = strdup(value);
                } else if(!strcmp(option, "Host")) {
                    state->hostname = strdup(value);
                } else if(!strcmp(option, "SynergysPath")) {
                    state->synergys_path = strdup(value);
                } else if(!strcmp(option, "SynergycPath")) {
                    state->synergyc_path = strdup(value);
                } else if(!strcmp(option, "ScreenName")) {
                    state->screen_name = strdup(value);
                }
            }
        } while(!feof(f));
    }
}

void save_config(qs_state_t *state) {
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
    
    if(strcmp(state->above, _("Above")) && strcmp(state->above, ""))
        fprintf(f, "Above %s\n", state->above);

    if(strcmp(state->below, _("Below")) && strcmp(state->below, ""))
        fprintf(f, "Below %s\n", state->below);

    if(strcmp(state->left, _("Left")) && strcmp(state->left, ""))
        fprintf(f, "Left %s\n", state->left);

    if(strcmp(state->right, _("Right")) && strcmp(state->right, ""))
        fprintf(f, "Right %s\n", state->right);

    fprintf(f, "Host %s\n", state->hostname);
    
    fprintf(f, "SynergysPath %s\n", state->synergys_path);
    fprintf(f, "SynergycPath %s\n", state->synergyc_path);
    
    fprintf(f, "ScreenName %s\n", state->screen_name);
    
    fclose(f);
}

void save_synergy_config(qs_state_t *state) {
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
