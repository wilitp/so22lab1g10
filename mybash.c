#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "prompt.h"
#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"

// static void show_prompt(void) {
//     char cwd[PATH_MAX];
//     getcwd(cwd, PATH_MAX);
//     printf("%s mybash>", cwd);
//     fflush(stdout);
// }

int main(int argc, char *argv[]) {
    exit_my_bash = false;
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
            if(!end_of_line)  {
                // Loguear lo parseado
                // char *pstr = pipeline_to_string(pipe);
                // printf("%s\n", pstr);
                // free(pstr);

                // Ejecutar el pipeline
                execute_pipeline(pipe);
            }

            pipeline_destroy(pipe);
            /*
             * COMPLETAR
             *
             */

            quit = parser_at_eof(input);
        } while (!end_of_line);
    }

    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
