#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "prompt.h"

int main( void ) {
    
    Parser input = parser_new(stdin);

    if (input == NULL) {
        perror("Error inicializando el Parser\n");
        exit(EXIT_FAILURE);
    }
    
    bool end_of_line;
    bool quit = false;
    while (!quit) {
        show_prompt();
        do {
            pipeline pipe = parse_pipeline(input);
            if (!(end_of_line = (pipe == NULL))) {
                // Loguear lo parseado
                // char *pstr = pipeline_to_string(pipe);
                // printf("%s\n", pstr);
                // free(pstr);

                // Ejecutar el pipeline

                execute_pipeline(pipe);

                pipeline_destroy(pipe);
            }

            quit = parser_at_eof(input);
        } while (!end_of_line);
    }

    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
