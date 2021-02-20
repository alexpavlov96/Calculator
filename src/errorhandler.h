#ifndef _ERRORHANDLER_H_
#define _ERRORHANDLER_H_
#include "qust.h"

typedef enum
{
  OK_SCANNER,     // expression is correct
  MEMORY_SCANNER, // no memory
  SYMBOL,         // unknown symbol
  SCIENTIFIC,     // error in scientific form
  FRACTION        // error in fraction
} scanner_t;

typedef enum
{
  WAIT_PREFIX,    // we are waiting for number, variable or '('
  WAIT_SUFFIX,    // we are waitnig for operation, function symbol or ')'
  ERROR,          // invalid order of operation/operands
  MEMORY_PARSER,  // no memory
  BRACKETS,       // number of '(' and of ')' are not equal
  DONE,           // expression is parsed
  END             // exit from loop
} parser_t;

typedef enum
{
  OK_CALCULATOR,     // no math or memory errors
  MEMORY_CALCULATOR, // no memory
  LARGE_NUMBER,      // overload type 'double'
  ZERO,              // division by zero
  SQRT,              // error at sqrt(x)
  LOG1,              // error at ln(x)
  LOG2,              // error at log(a,x) 
  ARCSIN,            
  ARCCOS,            
  POWER,             
  INITALIZE,         // using not initalized variable
  LVALUE,            // 'number' = 'expression'
  NO_EXPRESSION      // where are initilized variables,
                     // but where are no final expression 
} calculate_t;

scanner_t Scanner( char *s, QUEUE *Q );
boolean ErrorHandlerOfScanner( scanner_t e );
parser_t Parser ( QUEUE *ResQ, QUEUE *Q );
boolean ErrorHandlerOfParser( parser_t e );
calculate_t Calculate( QUEUE *Q, double *answer );
boolean ErrorHandlerOfCalculator( calculate_t e);
boolean isfunction( char c );

#endif