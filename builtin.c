#include <assert.h>
#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp
#include <unistd.h> // chdir

#include "builtin.h"
#include "tests/syscall_mock.h"
#include "command.h"

static bool is_cd(scommand cmd) {
    return (strcmp(scommand_front(cmd), "cd") == 0);
}

static bool is_exit(scommand cmd) {
    return (strcmp(scommand_front(cmd), "exit") == 0);
}

static bool is_help(scommand cmd) {
    return (strcmp(scommand_front(cmd), "help") == 0);
}

// ---------------INTERNAL FUNCTIONS---------------

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    return is_cd(cmd) || is_exit(cmd) || is_help(cmd);
}

bool builtin_alone(pipeline p) {
    return (pipeline_length(p) == 1) && builtin_is_internal(pipeline_front(p));
}

void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));

    if (is_help(cmd)) {
        printf("Integrantes:\n"
               "\t- De Ipola Guillermo\n"
               "\t- Molina Franco\n"
               "\t- Ebner Lautaro\n"
               "\t- Ludueña Zakka Juan Pablo\n\n"
               "Usage: mybash <command>\n");

    } else if (is_cd(cmd)) {
        scommand_pop_front(cmd); // hago bosta el 'cd'
        if (chdir(scommand_front(cmd)) == (-1)) {            // ejecuto el chdir y veo si hay error
            char *str = scommand_to_string(cmd);
            perror(str); // si hay error, lo imprimo
            free(str);
        }

    } else if (is_exit(cmd)) {
        printf("bye! -mybash\n");
        exit(EXIT_SUCCESS);
    }
}
