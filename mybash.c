#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <assert.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "prompt.h"

static void clean_garbage(Parser parser, bool must_be_clean) {

    bool garbage = false;
    parser_garbage(parser, &garbage);
    if (garbage && must_be_clean) {
        printf("Error: %s\n", parser_last_garbage(parser));
        exit(1);
    }
}

int main(int argc, char *argv[]) {

    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    bool end_of_line;
    while (!quit) {
        show_prompt();
        do {
            pipe = parse_pipeline(input);
            if (pipe != NULL) {
                if (!(end_of_line = pipeline_is_empty(pipe))) {
                    // Loguear lo parseado
                    // char *pstr = pipeline_to_string(pipe);
                    // printf("%s\n", pstr);
                    // free(pstr);

                    // Ejecutar el pipeline

                    if (builtin_alone(pipe)) {
                        builtin_run(pipeline_front(pipe));

                    } else {
                        int cpid;
                        if ((cpid = fork()) == 0) {

                            // Ejecutar el pipeline y terminar el proceso hijo
                            execute_pipeline(pipe);
                            pipeline_destroy(pipe);
                            exit(EXIT_SUCCESS);
                        } else if (pipeline_get_wait(pipe)) {
                            // Esperar la ejecucion del pipeline
                            waitpid(cpid, NULL, 0);

                            // Limpiar a todos los hijos(pipelines que hayamos
                            // corrido en background)
                            while (waitpid(-1, NULL, WNOHANG) > 0)
                                ;
                        }
                    }

                } else {

                    // Limpio el \n al final
                    clean_garbage(input, true);
                }

                pipeline_destroy(pipe);
            } else {
                // Limpio toda la linea que dio error de parseo
                clean_garbage(input, false);
                end_of_line = true;
            }

            quit = parser_at_eof(input);
        } while (!end_of_line);
    }

    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
