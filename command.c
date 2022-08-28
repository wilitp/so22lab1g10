#include <stdbool.h> /* para tener bool */

#include "command.h"

struct scommand_s {
  char ** front;
  char ** back;
  char * redirect_in;
  char * redirect_out;
};

// TODO: implementacion de scommand


struct pipeline_s {
  scommand * front;
  scommand * back;
};

// TODO: implementacion de pipeline

