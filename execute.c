#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include "execute.h"

static char **free_arg_array(char **arr, unsigned int argc) {
    for (unsigned int i = 0; i < argc; ++i) {
        free(arr[i]);
    }
    free(arr);
    return arr = NULL;
}

static char **arg_array(scommand cmd, unsigned int argc) {

    char **argv = calloc((argc + 1), sizeof(char *));
    argv[argc] = NULL; // argv es null terminated

    for (unsigned i = 0; i < argc; ++i) {
        argv[i] = strdup(scommand_front(cmd));
        scommand_pop_front(cmd);
    }
    return argv;
}

/* Se encarga de la redireccion de input y output, ya sean
 * Tambien encarga de cerrar la salida del pipe abierto por el comando anterior.
 * SOLO DEBE LLAMARSE DESDE UN PROCESO HIJO
 */
// static void in_out_redirs(scommand cmd, unsigned long *pipe_prev,
//                           unsigned long *pipe_next) {}

void execute_pipeline(pipeline pipe) {
    unsigned int plen = pipeline_length(pipe);
    if (plen == 1) {
        // Ejecutar el unico comando
        scommand cmd = pipeline_front(pipe);
        unsigned int argc = scommand_length(cmd);
        char **argv = arg_array(cmd, argc);

        int cpid;

        // Hijo
        if (0 == (cpid = fork())) {

            if (strcmp(argv[0], "cd") == 0) { // No anda esto
                chdir(argv[1]);
                argv = free_arg_array(argv, argc);
                exit(0);
            } else if (execvp(argv[0], argv) < 0) {
                exit(1);
            }

            // Padre
        } else {
            if (pipeline_get_wait(pipe)) {
                wait(NULL);
            }
        }
    } else {
        exit(1);
    }
}
