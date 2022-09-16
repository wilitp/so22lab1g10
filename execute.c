#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "tests/syscall_mock.h"

#include "builtin.h"
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
 * SOLO DEBE LLAMARSE DESDE UN PROCESO HIJO
 */
static void in_out_redirs(scommand cmd, int *pipe_prev, int *pipe_next) {

    // Variable auxiliar para los nombres de los archivos
    char *fname_aux = NULL;

    int in, out;

    if ((fname_aux = scommand_get_redir_in(cmd))) {
        // Abrir archivo y asignar in
        in = open(fname_aux, O_RDONLY, 0);
        if (in < 0) {
        } else {
            dup2(in, STDIN_FILENO);
        }
    } else if (pipe_prev) {
        in = *pipe_prev;
        dup2(in, STDIN_FILENO);
    }

    if ((fname_aux = scommand_get_redir_out(cmd))) {
        // Abrir archivo y asignar in
        out = open(fname_aux, O_WRONLY | O_CREAT, 0666);
        dup2(out, STDOUT_FILENO);
    } else if (pipe_next) {
        out = *pipe_next;
        dup2(out, STDOUT_FILENO);
    }
}

static int execute_command(scommand cmd, int last_pipe_out, bool is_first,
                           bool is_last) {
    int pipefd[2];

    int cpid;

    // Si no es el ultimo, creamos una pipe
    if (!is_last && pipe(pipefd) < 0) {
        exit(1);
    }

    // Hijo

    if (0 == (cpid = fork())) {

        // Redireccion
        in_out_redirs(cmd, !is_first ? &last_pipe_out : NULL,
                      !is_last ? &pipefd[1] : NULL);

        // Ejecutar comando
        if (builtin_is_internal(cmd)) {
            builtin_run(cmd);
            exit(EXIT_SUCCESS);
        } else {
            unsigned int argc = scommand_length(cmd);
            char **argv = arg_array(cmd, argc);
            if (execvp(argv[0], argv) < 0) {
                // Manejar errores al ejecutar
                fprintf(stderr, "Command not found: %s\n", argv[0]);
                argv = free_arg_array(argv, argc);
                exit(1);
            }
            argv = free_arg_array(argv, argc);
        }

        // Padre
    } else {

        /* Cerrar los archivos de escritura del comando anterior
         * y el lado de escritura de nuestra pipe si es necesario
         * (El proceso padre no los usa)
         */
        if (!is_first) {
            close(last_pipe_out);
        }
        if (!is_last) {
            close(pipefd[1]);
        }
    }
    return pipefd[0];
}

void execute_pipeline(pipeline apipe) {

    // El fd de lectura del pipe abierto para el comando anterior
    int last_pipe_out;

    bool is_first = true;

    unsigned int plen = pipeline_length(apipe);
    do {
        scommand cmd = pipeline_front(apipe);

        // Ejecutar el comando y guardar la salida pipeada
        last_pipe_out =
            execute_command(cmd, last_pipe_out, is_first, plen == 1);

        pipeline_pop_front(apipe);
        --plen;
        is_first = false;
    } while (pipeline_length(apipe));
    while(waitpid(-1, NULL, 0) > 0);
}
