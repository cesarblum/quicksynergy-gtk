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
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

int selfunc(const struct dirent *ep) {
    if(isdigit(ep->d_name[0]))
        return 1;

    return 0;
}

pid_t get_pid_by_name(char *command) {
    FILE *fp;
    struct dirent **proclist;
    char filename[128], l[128], opt[128], val[128];
    int n, i;

    if((n = scandir("/proc", &proclist, selfunc, alphasort)) != -1) {
        for(i = 0; i < n; i++) {
            strcpy(filename, "/proc/");
            strcat(filename, proclist[i]->d_name);
            strcat(filename, "/status");

            if((fp = fopen(filename, "r")) != NULL) {
                while(!feof(fp)) {
                    fgets(l, 128, fp);

                    sscanf(l, "%s %s", opt, val);

                    if(!strcmp(opt, "Name:"))
                        if(!strcmp(val, command))
							return atoi(proclist[i]->d_name);
                }

                fclose(fp);
            }
        }
    }
	
	return 0;
}
