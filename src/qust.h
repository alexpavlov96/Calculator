#ifndef _QUST_H_
#define _QUST_H_
#include "types.h"

boolean Put( QUEUE *Q, TOK A);
boolean Get( QUEUE *Q, TOK *A);
boolean Push( STACK *S, TOK A);
boolean Pop( STACK *S, TOK *A);

#endif
