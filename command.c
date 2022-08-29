#include <stdbool.h> /* para tener bool */
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "strextra.h"

struct scommand_s {
    queue args; // Queue de *char
    char *redirect_in;
    char *redirect_out;
};

// TODO: implementacion de scommand

scommand scommand_new(void) {
    scommand cmd = malloc(sizeof(struct scommand_s));

    // Sin redireccion por defecto
    cmd->redirect_in = (cmd->redirect_out = NULL);

    cmd->args = queue_empty();
    return cmd;
}

scommand scommand_destroy(scommand self) {
    queue_destroy(self->args);
    if (self->redirect_in) {
        free(self->redirect_in);
        self->redirect_in = NULL;
    }
    if (self->redirect_out) {
        free(self->redirect_out);
        self->redirect_out = NULL;
    }

    return self;
}

void scommand_push_back(scommand self, char *argument) {
    queue_enqueue(self->args, argument);
}

void scommand_pop_front(scommand self) { queue_dequeue(self->args); }

void scommand_set_redir_in(scommand self, char *filename) {
    self->redirect_in = filename;
}
void scommand_set_redir_out(scommand self, char *filename) {
    self->redirect_out = filename;
}

bool scommand_is_empty(const scommand self) {
    return queue_is_empty(self->args);
}

unsigned int scommand_length(const scommand self) {
    return queue_size(self->args);
}

char *scommand_front(const scommand self) { return queue_first(self->args); }

char *scommand_get_redir_in(const scommand self) { return self->redirect_in; }

char *scommand_get_redir_out(const scommand self) { return self->redirect_in; }

char *scommand_to_string(const scommand self) {
    char *output = strdup("");
    // char *killme;
    //
    // queue args = ;
    //
    //
    // // Argumentos
    //
    //
    // // Redireccion de output
    // if (self->redirect_out != NULL) {
    //     killme = output;
    //     output = strmerge(output, " > ") free(killme);
    //     free(killme);
    //
    //     killme = output;
    //     output = strmerge(output, self->redirect_out);
    //     free(killme);
    // }
    //
    // // Redireccion de input
    // if (self->redirect_in != NULL) {
    //     killme = output;
    //     output = strmerge(output, " < ");
    //     free(killme);
    //
    //     killme = output;
    //     output = strmerge(output, self->redirect_in);
    //     free(killme);
    // }

    return output;
}

// TODO: implementacion de pipeline
