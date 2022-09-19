#include <pwd.h>
#include <unistd.h>
#include <stdio.h>

#include "prompt.h"
void color_green(void);
void color_blue(void);
void color_reset(void);


void color_green(void) {
    printf("\033[1;32m");
}

void color_blue(void) {
    printf("\033[1;34m");
}

void color_reset(void) {
    printf("\033[0m");
}

void show_prompt(void) {
    
    // username
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    char * username = pw->pw_name;

    // work directory
    char work_dir[MAX_WORK_DIR_LENGTH];
    work_dir[MAX_WORK_DIR_LENGTH-1] = '\0'; // El último caracter es NULL
    getcwd(work_dir, MAX_WORK_DIR_LENGTH);
        
    char host_name[MAX_HOST_LENGTH];
    host_name[MAX_HOST_LENGTH-1] = '\0'; // El último caracter es NULL
    gethostname(host_name, MAX_HOST_LENGTH);


    // print actual prompt
    printf("(my_bash) ");
    color_green();
    printf("%s@%s", username, host_name);
    color_reset();
    printf(":");
    color_blue();
    printf("%s", work_dir);
    color_reset();
    printf("$ ");
    fflush(stdout);
}

    
