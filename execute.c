#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include "builtin.h"
#include "execute.h"
#include "tests/syscall_mock.h"

// Destruye un array de strings
static char **free_arg_array(char **arr, unsigned int argc) {
    for (unsigned int i = 0; i < argc; ++i) {
        free(arr[i]);
    }
    free(arr);
    return arr = NULL;
}

// Construye un array a partir de un comando
static char **arg_array(scommand cmd, unsigned int argc) {

    char **argv = (char **) calloc((argc + 1), sizeof(char *));
    argv[argc] = NULL; // argv es null terminated (requisito de execvp)

    for (unsigned i = 0; i < argc; ++i) {
        argv[i] = strdup(scommand_front(cmd));
        scommand_pop_front(cmd);
    }
    return argv;
}

/* Se encarga de la redireccion de input y output
 * SOLO DEBE LLAMARSE DESDE UN PROCESO HIJO
 */
static void in_out_redirs(scommand cmd, const int *pipe_prev, const int *pipe_next) {

    int in, out;

    // fname_aux: Variable auxiliar para los nombres de los archivos
    char *fname_aux = scommand_get_redir_in(cmd);
    if (fname_aux != NULL) {
        // Abrir archivo y asignar in
        // O_RDONLY: read-only
        in = open(fname_aux, O_RDONLY, 0);
        if (in < 0) {
            perror("No se pudo abrir el archivo de entrada");
        } else {
            // dup2(fd1, fd2): copia en fd2 fd1
            dup2(in, STDIN_FILENO);
        }
        close(in);
    // pipe_prev: de donde tiene que leer este comando
    } else if (pipe_prev) {
        in = *pipe_prev;
        dup2(in, STDIN_FILENO);
        close(in);

    }
    fname_aux = scommand_get_redir_out(cmd);

    if (fname_aux != NULL) {
        // Abrir archivo y asignar out
        // O_WRONLY: write-only
        // O_CREAT:  si el pathname no existe, lo crea
        // 0666:     permisos
        out = open(fname_aux, O_WRONLY | O_CREAT, 0666);
        if (out < 0) {
            perror("No se pudo abrir el archivo de salida");
        }
        dup2(out, STDOUT_FILENO);
        close(out);
    // pipe_next: a donde tiene que escribir este comando
    } else if (pipe_next) {
        out = *pipe_next;
        dup2(out, STDOUT_FILENO);
        close(out);
    }
}

static int execute_command(scommand cmd, int last_pipe_out, bool is_first, bool is_last, int *cpid) {

    // pipe file descriptor
    // pipefd -> [0: lectura, 1: escritura]
    int pipefd[2];

    // Si no es el ultimo, creamos una pipe
    if (!is_last && (pipe(pipefd) < 0)) {
        exit(1);
    }
    
    *cpid = fork();

    // Error
    if (*cpid < 0) {
        perror("Fork error");

    // Hijo
    } else if (*cpid == 0) {
        // Cerrar el fd de lectura (solo le interesa al padre)
        if(!is_last) {
            close(pipefd[0]);
        }

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

    if (pipeline_is_empty(apipe)) {
        return;
    }

    // si es un comando bulitin lo ejecutamos en el mismo proceso
    if (builtin_alone(apipe)) {
        builtin_run(pipeline_front(apipe));
        return;
    }

    // Array de pid's para luego esperar a los procesos hijos
    const unsigned int cpidc = plen;
    int *cpids = (int *) calloc(plen, sizeof(int));

    do {
        scommand cmd = pipeline_front(apipe);

        // Ejecutar el comando y guardar la salida pipeada
        // Como ultimo argumento le paso el puntero del ultimo pid
        last_pipe_out = execute_command(cmd, last_pipe_out, is_first, plen == 1,
                                        &cpids[cpidc - plen]);

        pipeline_pop_front(apipe);
        --plen;
        is_first = false;
    } while (pipeline_length(apipe));

    // Espero a los procesos hijos si es necesario
    if (pipeline_get_wait(apipe)) {
        for (unsigned int i = 0; i < cpidc; ++i) {
            waitpid(cpids[i], NULL, 0);
        }
    }
    free(cpids);
}
