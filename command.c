#include <assert.h>
#include <glib.h>
#include <stdbool.h> /* para tener bool */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    cmd->redirect_in = NULL;
    cmd->redirect_out = NULL;

    cmd->args = NULL;

    assert(cmd!=NULL);
    assert(cmd->redirect_in == NULL);
    assert(cmd->redirect_out == NULL);
    return cmd;
}

scommand scommand_destroy(scommand self) {
    assert(self != NULL);
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

    free(self);
    self = NULL;

    return self;
}

void scommand_push_back(scommand self, char *argument) {
    assert(self != NULL);
    assert(argument != NULL);
    self->args = g_list_append(self->args, argument);
}

void scommand_pop_front(scommand self) {
    assert(self != NULL);
    assert(g_list_length(self->args) > 0);

    // Free data del elemento
    free(g_list_first(self->args)->data);

    // Free elemento en si(nodo)
    self->args = g_list_delete_link(self->args, self->args);
    
}

void scommand_set_redir_in(scommand self, char *filename) {
    assert(self != NULL);
    self->redirect_in = filename;
}
void scommand_set_redir_out(scommand self, char *filename) {
    assert(self != NULL);
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
    return self->redirect_out;
}

char *scommand_to_string(const scommand self) {
    assert(self != NULL);

    char *output = strdup("");

    if(g_list_length(self->args) == 0) {
      return output;
    }
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
    GList *commands;
    bool wait;
};

pipeline pipeline_new(void) {

    struct pipeline_s *new_pipe = malloc(sizeof(struct pipeline_s));
    new_pipe->commands = NULL;
    new_pipe->wait = true;

    assert(new_pipe != NULL);
    assert(pipeline_is_empty(new_pipe));
    assert(pipeline_get_wait(new_pipe));

    return new_pipe;
}

static void free_scommand(scommand cmd) { scommand_destroy(cmd); }

pipeline pipeline_destroy(pipeline pipe) {
    assert(pipe != NULL);

    // Liberar lista de comandos
    g_list_free_full(pipe->commands, (GDestroyNotify)&free_scommand);

    // Liberar pipe
    free(pipe);
    pipe = NULL;
    return pipe;
}

void pipeline_push_back(pipeline pipe, scommand cmd) {
    assert(pipe != NULL);
    assert(cmd != NULL);
    pipe->commands = g_list_append(pipe->commands, cmd);
}

void pipeline_pop_front(pipeline pipe) {
    assert(pipe != NULL);
    assert(g_list_length(pipe->commands) != 0);

    // Liberamos el command
    scommand_destroy(g_list_first(pipe->commands)->data);

    // Removemos el nodo
    pipe->commands = g_list_delete_link(pipe->commands, pipe->commands);
}

void pipeline_set_wait(pipeline pipe, const bool w) { 
  assert(pipe != NULL);
  pipe->wait = w; 
}

unsigned int pipeline_length(const pipeline pipe) {
    assert(pipe != NULL);

    return g_list_length(pipe->commands);
}

bool pipeline_is_empty(const pipeline pipe) {
    assert(pipe != NULL);

    return (pipeline_length(pipe) == 0);
}

scommand pipeline_front(const pipeline pipe) {
    assert(pipe != NULL);
    assert(g_list_length(pipe->commands));

    return g_list_first(pipe->commands)->data;
}

bool pipeline_get_wait(const pipeline pipe) { 
    assert(pipe != NULL);
    return pipe->wait; 
}

char *pipeline_to_string(const pipeline pipe) {
    assert(pipe != NULL);

    char *pipestr = strdup("");
    if(g_list_length(pipe->commands) == 0) {
      return pipestr;
    }
    char *killme;

    GList *current_cmd = pipe->commands;

    while (current_cmd != NULL) {
        char *cmdstr = scommand_to_string(current_cmd->data);

        killme = pipestr;
        pipestr = strmerge(pipestr, cmdstr);
        free(killme);
        free(cmdstr);

        // Si no es el ultimo entonces ponemos un pipe antes del proximo
        if(current_cmd->next != NULL) {
            killme = pipestr;
            pipestr = strmerge(pipestr, " | ");
            free(killme);
        }
        
        current_cmd = current_cmd->next;
    }

    // Si es background ponemos & al final
    if (pipe->wait == false) {
        killme = pipestr;
        pipestr = strmerge(pipestr, " &");
        free(killme);
    }

    return pipestr;
}
