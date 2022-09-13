#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "prompt.h"

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

            end_of_line = pipeline_is_empty(pipe);
            if (!end_of_line) {
                // Loguear lo parseado
                // char *pstr = pipeline_to_string(pipe);
                // printf("%s\n", pstr);
                // free(pstr);

                // Ejecutar el pipeline

                int cpid;
                if ((cpid = fork()) == 0) {
                    execute_pipeline(pipe);
                    exit(EXIT_SUCCESS);
                } else if (pipeline_get_wait(pipe)) {
                    // Esperar al padre
                    waitpid(cpid, NULL, 0);

                    // Limpiar a todos los hijos(pipelines que hayamos corrido en background)
                    while(waitpid(-1, NULL, WNOHANG) > 0);
                }

            } else {

                // Limpio el \n al final
                bool garbage = false;
                parser_garbage(input, &garbage);
                if (garbage) {
                    printf("Error: %s\n",
                           parser_last_garbage(input));
                    exit(1);
                }
            }

            pipeline_destroy(pipe);

            quit = parser_at_eof(input);
        } while (!end_of_line);
    }

    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
