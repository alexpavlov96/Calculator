#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "qust.h"

void myAssert( const char *String, boolean expression )
{
  if (expression == FALSE)
  {
    fprintf(stderr, "\n\n");
    fprintf(stderr, String);
    fprintf(stderr, "\n\n");
    assert(expression);
  }
}

boolean isEmpty( char *s )
{
  int i;
  for (i = 0; s[i] != 0; i++)
  {
    if (s[i] != ' ' && s[i] != '\t' && s[i] != 26)
      return FALSE;
  }
  return TRUE;
}

boolean isComment( char *s )
{
  int i, isempty = 1;
  for (i = 0; s[i+1] != 0; i++)
  {
    if (s[i] == '/' && s[i+1] == '/' && isempty)
      return TRUE;
    else if (s[i] != ' ' && s[i] != '\t' && s[i] != 26)
      isempty = 0;
  }
  return FALSE;
}

void freeQueue( QUEUE *Q )
{
  LIST *P;
  while (Q->Head != NULL)
  {
	  P = Q->Head;
	  Q->Head = Q->Head->Next;
	  free(P);
  }
  Q->Tail = NULL;
}

void freeStack( STACK *S )
{
  LIST *P;
  while (S->Top != NULL)
  {
	  P = S->Top;
    S->Top = S->Top -> Next;
	  free(P);
  }
}

void freeString( char *s )
{
  int i;
  for (i = 0; s[i] != 0; i++)
    s[i] = 0;
}

boolean isAlpha( char c )
{
  if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
    return TRUE;
  return FALSE;
}

// comparing with accuracy epsilon
boolean compareToDBL( double X, double Y )
{
  double epsilon = pow((double)0.1,312);
  if (fabs(X - Y) < epsilon)
    return TRUE;
  return FALSE;
}

boolean compareToINT( double X, int Y )
{
  double epsilon = pow((double)0.1,312);
  if (fabs(X - Y) < epsilon)
    return TRUE;
  return FALSE;
}

/* returns the symbol for Polish Notation 
    which corresponds to this function */
char getFuncSymbol( char *funcName )
{
  if (!strcmp(funcName, "sin("))
    return 's';
  else if (!strcmp(funcName, "cos("))
    return 'c';
  else if (!strcmp(funcName, "tg("))
    return 't';
  else if (!strcmp(funcName, "ctg("))
    return 'g';
  else if (!strcmp(funcName, "ln("))
    return 'l';
  else if (!strcmp(funcName, "sqrt("))
    return 'q';
  else if (!strcmp(funcName, "floor("))
    return 'f';
  else if (!strcmp(funcName, "ceil("))
    return 'i';
  else if (!strcmp(funcName, "arcsin("))
    return 'x';
  else if (!strcmp(funcName, "arccos("))
    return 'y';
  else if (!strcmp(funcName, "arctg("))
    return 'z';
  else if (!strcmp(funcName, "log("))
    return 'L';
	return '0';
}

/* check whether the next character after
    one letter variable is 
    whitespace, an operation or end of the string */

boolean isOper( char c )
{
  switch (c)
  {
  case 0:
  case ' ':
  case '\t':
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
  case ')':
  case ',':
  case '=':
  case ';':
    return TRUE;
  default:
    return FALSE;
  }
}

/* if a character is 
   defined in Polish notation 
   as one of the functions */
boolean isfunction( char c )
{
  switch (c)
  {
  case 's': // sin
	case 'c': // cos
	case 't': // tg
	case 'g': // ctg
	case 'q': // sqrt
	case 'l': // ln
	case 'f': // floor
	case 'i': // ceil
	case 'x': // arcsin
	case 'y': // arccos
	case 'z': // arctg
  case 'L': // log(x,y)
	  return TRUE;
  }
  return FALSE;
}
