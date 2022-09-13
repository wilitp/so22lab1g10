#include "parsing.h"
#include "command.h"
#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Devuelve un comando ya parseado, Si se encuentra un símbolo del
 * operador pipe (|) o un fin de línea ("\n"), el procesamiento no avanza
 * y retorna lo parseado hasta ese punto.
 *
 * Si solo encuentra un "\n" o hay error de parseo devuelve NULL.
 *
 * 'is_background' indica si se encontro un ampersand (&), lo consume
 *  y retorna lo parseado hasta ese punto.
 */
static scommand parse_scommand(Parser parser, bool *is_background, bool * is_pipe) {
    /* Devuelve NULL cuando hay un error de parseo */
    assert(parser != NULL);
    if (parser_at_eof(parser)) {
        return NULL;
    }
    scommand cmd = scommand_new();
    arg_kind_t type;
    char *data = NULL;
    do {
        parser_skip_blanks(parser); // consume todos los espacios (" ")
        if (parser_at_eof(parser)) {
            break;
        }
        parser_op_background(parser, is_background);
        parser_op_pipe(parser, is_pipe);
        if (!(*is_background) && !(*is_pipe) && !parser_at_eof(parser)) {
            data = parser_next_argument(
                parser, &type); // consume todo hasta el proximo espacio (" ")
            if (data != NULL) {
                switch (type) {
                case ARG_NORMAL:
                    scommand_push_back(cmd, data);
                    break;
                case ARG_INPUT:
                    scommand_set_redir_in(cmd, data);
                    break;
                case ARG_OUTPUT:
                    scommand_set_redir_out(cmd, data);
                    break;
                }
            }
        }
    } while ((data != NULL) && !(*is_background) && !(*is_pipe) && !parser_at_eof(parser));
    return cmd;
}

pipeline parse_pipeline(Parser parser) {

    pipeline result = pipeline_new();
    bool is_background = false;
    bool another_command = false;
    bool error;

    do {
        // Leemos un comando
        scommand cmd = parse_scommand(parser, &is_background, &another_command);
        error = (cmd == NULL);
        if(error){
            break;
        }

        if (scommand_length(cmd) > 0) {
            // Si el comando no es vacio lo agregamos
            pipeline_push_back(result, cmd);
            parser_skip_blanks(parser);
        } 
    } while (another_command);


    pipeline_set_wait(result, !is_background);

    return result;
}
