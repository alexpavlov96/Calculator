#ifndef _UTILS_H_
#define _UTILS_H
#include "types.h"

void myAssert( const char *String, boolean expression );
boolean isEmpty( char *s );
boolean isComment( char *s );
void freeQueue( QUEUE *Q );
void freeStack( STACK *S );
void freeString( char *s );
boolean isAlpha ( char c );
boolean isOper( char c );
int GetPriority( char c );
boolean isfunction( char c );
char getFuncSymbol( char *funcName );
boolean compareToDBL( double X, double Y );
boolean compareToINT( double X, int Y );

#endif