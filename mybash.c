#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"

static void show_prompt(void) {
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    printf("%s mybash>", cwd);
    fflush(stdout);
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
            /* Hay que salir luego de ejecutar? */
            quit = parser_at_eof(input);

            end_of_line = pipeline_is_empty(pipe);
            if(!end_of_line)  {
                char *pstr = pipeline_to_string(pipe);
                printf("%s\n", pstr);
                free(pstr);
            }

            pipeline_destroy(pipe);
            /*
             * COMPLETAR
             *
             */
        } while (!end_of_line);
    }
    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
