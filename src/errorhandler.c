#include <stdio.h>
#include "errorhandler.h"
#include "types.h"

boolean ErrorHandlerOfScanner( scanner_t exeption )
{
  if (exeption == OK_SCANNER)
    return TRUE;
  printf(" == ERROR: ");

  switch (exeption)
  {
  case MEMORY_SCANNER:
    printf("in scanner: Not enough memory");
    break;
  case SYMBOL:
    printf("Unknown symbol");
    break;
  case SCIENTIFIC: 
    printf("in scientific form");
    break;
  case FRACTION:
    printf("in fraction");
    break;
  }
  return FALSE;
}

boolean ErrorHandlerOfParser( parser_t exeption )
{
  if (exeption == END)
    return TRUE;
  printf(" == ERROR: ");
  
  switch (exeption)
  {
  case MEMORY_PARSER:
    printf("in parser: Not enough memory");
    break;
  case ERROR:
    printf("invalid order of operands/operations");
    break;
  case BRACKETS:
    printf("brackets");
    break;
  }
  return FALSE;
}

boolean ErrorHandlerOfCalculator( calculate_t exeption)
{
   if (exeption == OK_CALCULATOR)
    return TRUE;
  printf(" == ERROR: ");
  
  switch (exeption)
  {
  case MEMORY_CALCULATOR:
    printf("in calculator: Not enough memory");
    break;
  case LARGE_NUMBER:
    printf("too large number");
    break;
  case ZERO:
    printf("division by zero");
    break;
  case SQRT:
    printf("in function sqrt(x): must be x >= 0");
    break;
  case LOG1:
    printf("in function ln(x): must be x > 0");
    break;
  case LOG2:
    printf("in function log(x,y): must be x > 0 and x != 1 and y > 0");
    break;
  case ARCSIN:
    printf("in function arcsin(x): must be |x| <= 1");
    break;
  case ARCCOS:
    printf("in function arccos(x): must be |x| <= 1");
    break;
  case POWER:
    printf("when x ^ y: can't be x = 0 and y < 0");
    break;
  case INITALIZE:
    printf("when using 'x', 'x' must be initilized before");
    break;
  case LVALUE:
    printf("when x = y, 'x' must be variable");
    break;
  case NO_EXPRESSION:
    printf("where is no final expression");
    break;
  }
  return FALSE;
}
