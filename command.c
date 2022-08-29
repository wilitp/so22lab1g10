#include <glib.h>
#include <stdbool.h> /* para tener bool */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "command.h"
#include "strextra.h"

// TODO: implementacion de scommand

struct scommand_s {
    GList *args;
    char *redirect_in;
    char *redirect_out;
};

static char *copy_arg(char *arg, gpointer _) { return strdup(arg); }

scommand scommand_copy(const scommand self) {
    struct scommand_s *copy = scommand_new();

    if (copy->redirect_in) {
        copy->redirect_in = strdup(self->redirect_in);
    }

    if (copy->redirect_out) {
        copy->redirect_out = strdup(self->redirect_out);
    }

    copy->args = g_list_copy_deep(self->args, (GCopyFunc)&copy_arg, NULL);

    return copy;
}

scommand scommand_new(void) {
    scommand cmd = malloc(sizeof(struct scommand_s));

    // Sin redireccion por defecto
    cmd->redirect_in = (cmd->redirect_out = NULL);

    cmd->args = NULL;
    return cmd;
}

scommand scommand_destroy(scommand self) {
    // Lista de args
    g_list_free_full(self->args, (GDestroyNotify)*free);

    // Redireccion de input
    if (self->redirect_in) {
        free(self->redirect_in);
        self->redirect_in = NULL;
    }

    // Redireccion de output
    if (self->redirect_out) {
        free(self->redirect_out);
        self->redirect_out = NULL;
    }

    return self;
}

void scommand_push_back(scommand self, char *argument) {
    assert(argument != NULL);
    self->args = g_list_append(self->args, argument);
}

void scommand_pop_front(scommand self) {
    assert(g_list_length(self->args) > 0);
    // Free data del elemento
    free(g_list_first(self->args)->data);

    // Free elemento en si(nodo)
    self->args = g_list_remove(self->args, 0);
}

void scommand_set_redir_in(scommand self, char *filename) {
    assert(filename != NULL);
    self->redirect_in = filename;
}
void scommand_set_redir_out(scommand self, char *filename) {
    assert(filename != NULL);
    self->redirect_out = filename;
}

bool scommand_is_empty(const scommand self) {
    assert(self != NULL);
    return g_list_length(self->args) == 0;
}

unsigned int scommand_length(const scommand self) {
    assert(self != NULL);
    return g_list_length(self->args);
}

char *scommand_front(const scommand self) {
    assert(self != NULL);
    assert(g_list_length(self->args) > 0);
    return (char *)g_list_first(self->args)->data;
}

char *scommand_get_redir_in(const scommand self) { 
    assert(self != NULL);
    return self->redirect_in; 
}

char *scommand_get_redir_out(const scommand self) { 
    assert(self != NULL);
    return self->redirect_in; 
}

char *scommand_to_string(const scommand self) {
    assert(self != NULL);
    assert(g_list_length(self->args) > 0);

    char *output = strdup("");
    char *killme;

    // Argumentos
    GList *current_arg = self->args;
    while (current_arg != NULL) {

        killme = output;
        output = strmerge(output, current_arg->data);
        free(killme);

        if (current_arg->next != NULL) {
            killme = output;
            output = strmerge(output, " ");
            free(killme);
        }
        current_arg = current_arg->next;
    }

    // Redireccion de output
    if (self->redirect_out != NULL) {
        killme = output;
        output = strmerge(output, " > ");
        free(killme);

        killme = output;
        output = strmerge(output, self->redirect_out);
        free(killme);
    }

    // Redireccion de input
    if (self->redirect_in != NULL) {
        killme = output;
        output = strmerge(output, " < ");
        free(killme);

        killme = output;
        output = strmerge(output, self->redirect_in);
        free(killme);
    }

    return output;
}


struct pipeline_s {
    queue comands; 
    bool wait;
};

pipeline pipeline_new(void) {

    struct pipeline_s * new_pipe = malloc(sizeof(struct pipeline_s));
    new_pipe->comands = queue_empty();
    new_pipe->wait = false;

    assert(new_pipe != NULL && pipeline_is_empty(new_pipe) && pipeline_get_wait(new_pipe));
    return new_pipe;
}

pipeline pipeline_destroy(pipeline pipe){

    queue_destroy(pipe->comands);
    free(pipe);
    pipe = NULL;
    return pipe;
}

void pipeline_push_back(pipeline pipe, scommand scm){

    queue_enqueue(pipe->comands, scm);
}

void pipeline_pop_front(pipeline pipe){

    queue_dequeue(pipe->comands);
}

void pipeline_set_wait(pipeline pipe, const bool w){

    pipe->wait = w;
}

bool pipeline_is_empty(const pipeline pipe){

    return queue_is_empty(pipe->comands);
}

unsigned int pipeline_length(const pipeline pipe){
    
    return queue_size(pipe->comands);
}

scommand pipeline_front(const pipeline pipe){

    return queue_first(pipe);
}

bool pipeline_get_wait(const pipeline pipe){

    return pipe->wait;
}

char * pipeline_to_string(const pipeline self){

    return 0;
}
