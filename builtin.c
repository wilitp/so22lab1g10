#include <stdio.h>
#include <assert.h> 
#include <string.h>     // strcmp
#include <stdbool.h>    // true, false 
#include <unistd.h>     // chdir

#include "command.h"
#include "builtin.h"


bool is_cd(scommand cmd) {
    return (strcmp(scommand_front(cmd), "cd")==0);
}

bool is_exit(scommand cmd) {
    return (strcmp(scommand_front(cmd), "exit")==0);
}

bool is_help(scommand cmd) {
    return (strcmp(scommand_front(cmd), "help")==0);
}

// ---------------INTERNAL FUNCTIONS---------------


bool builtin_is_internal(scommand cmd){
    assert(cmd!=NULL);
    return is_cd(cmd) || is_exit(cmd) || is_help(cmd);
}

bool builtin_alone(pipeline p) {
    return (pipeline_length(p) == 1) && builtin_is_internal(pipeline_front(p));    
}

void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));

    if (is_help(cmd)) {
        printf( "Integrantes:/n"
                "/t- De Ipola Guillermo/n"
                "/t- Molina Franco/n"
                "/t- Ebner Lautaro/n"
                "/t- Ludueña Zakka Juan Pablo/n/n"
                "Usage: mybash <command>\n\n");
  
    } else if (is_cd(cmd)) {
        scommand_pop_front(cmd);    // hago bosta el 'cd'
        chdir(scommand_front(cmd)); // lo que sigue es el path

    } else if (is_exit(cmd)) {
        printf("bye! -mybash");
        exit_my_bash = true;
    }
}

