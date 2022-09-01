#include <stdbool.h>
#include <stdlib.h>
#include "command.h"
#include "parser.h"
#include "parsing.h"



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
static scommand parse_scommand(Parser p, bool* is_background) {
    /* Devuelve NULL cuando hay un error de parseo */
    assert(p!=NULL);
    if (!parser_at_eof(p)) {
        scommand cmd = scommand_new();
        arg_kind_t * type;
        char *data;
        do {
            parser_skip_blanks(p);
            data = parser_next_argument(p, &type);
            if (*data = '&') {
                *is_background = true;
            } else {
                switch (* type) {
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
        } while (data != NULL && !(*is_background));
        return cmd;
    } else {
        return NULL;
    }
}

pipeline parse_pipeline(Parser parser) {

    pipeline result = pipeline_new();
    bool * is_background = false;
    scommand cmd = parse_scommand(parser, &is_background);
    bool error = (cmd==NULL)
    bool another_pipe = true;
    

    while (another_pipe && !error && !is_background) {
        pipeline_push_back(result, cmd);
        parser_op_pipe(parser, &another_pipe)
        if (*another_pipe) {
            cmd = parse_scommand(parser, &is_background);
        }
        error = (cmd==NULL);
    }
    pipeline_set_wait(result, *is_background);
    // cat asdas | grep asd | head -1                                     &                             
    // glist    => cat asdas -> grep asd -> head -1
    // wait     => true


    /* Tolerancia a espacios posteriores */
    /* Consumir todo lo que hay inclusive el \n */
    /* Si hubo error, hacemos cleanup */
    return result; 
}


