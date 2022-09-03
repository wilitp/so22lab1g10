#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"

static void show_prompt(void) {
    printf("mybash> ");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit) {
        show_prompt();
        pipe = parse_pipeline(input);
        // printf("%p", (void *)pipe);

        /* Hay que salir luego de ejecutar? */
        quit = parser_at_eof(input);

        char *pstr = pipeline_to_string(pipe);
        printf("%s\n", pstr);
        free(pstr);

        /*
         * COMPLETAR
         *
         */
    }
    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
