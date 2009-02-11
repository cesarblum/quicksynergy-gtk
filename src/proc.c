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

pid_t get_process_by_name(char *command) {
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
