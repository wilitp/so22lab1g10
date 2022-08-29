#include <stdbool.h> /* para tener bool */

#include "command.h"

struct scommand_s {
    queue args; // Queue de *char
    char *redirect_in;
    char *redirect_out;
};

// TODO: implementacion de scommand


// TODO: implementacion de pipeline
