#ifndef _VARIABLE_H_
#define _VARIABLE_H
#include "types.h"

boolean varSet( VARIABLE *V, char Name, double Val );
boolean varGet( VARIABLE *V, TOK *T );

#endif