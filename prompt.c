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
    getcwd(work_dir, MAX_WORK_DIR_LENGTH);

    // "username:work_dir$ »"
    printf("(my_bash) ");
    color_green();
    printf("%s:", username);
    color_blue();
    printf("%s", work_dir);
    // printf(" 〉");
    printf("%c ", 0x21c9);
    color_reset();
    fflush(stdout);
}

    
