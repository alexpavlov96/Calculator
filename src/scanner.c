#include <string.h>
#include <math.h>

#include "qust.h"
#include "errorhandler.h"
#include "types.h"
#include "utils.h"

#define MAXDEGREE 309
#define FUNC_NAME_SIZE 10

scanner_t Scanner( char *s, QUEUE *Q )
{
  TOK T = {TOK_NUM, 0, '0', '0'};                 // token going to queue
  double decimal;                                 // for getting a number
  char funcName[FUNC_NAME_SIZE] = {'0'};        
  char funcSymbol = '0';          // defines function
  int funcNameIndex = 0;          // current number of symbols in funcName
  boolean lBadLoop = FALSE;       // loop of getting funcName
  int degree = 0,                 // degree in scientific form
      signdegree = 0;             
  scanner_t state = OK_SCANNER;

  while  (*s != '\0')
  {
    if (state != OK_SCANNER)
    {
      freeQueue(Q);
      return state;
    }
	  switch (*s)
    {
	  case ' ':
	  case '\t':
    case 26:
      s++;
	    continue;
    // exponent
	  case 'e':
      T.Id = TOK_NUM;
	    T.Num = EXP;
      s++;
	    break;
	  case 'p':
      // pi
	    if (*(s+1) == 'i')
		  {
		    T.Id = TOK_NUM;
		    T.Num = PI;
        s += 2;
	  	}
      // variable 'p'
      else if (isOper(*(s+1)) && isAlpha(*s))
      {
        T.Id = TOK_NAME;
        T.Name = *s;
        funcNameIndex = 0;
        s++;
      }
      else
		    state = SYMBOL;
      break;
	  case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '(':
    case ')':
    case ',':
    case '=':
    case ';':
      T.Id = TOK_OP;
      T.Op = *s;
      s++;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
      T.Id = TOK_NUM;
      T.Num = 0;
	    if (*s != '.')
        // integer part of number
        while ((*s >= '0') && (*s <= '9'))
        {
		      T.Num = T.Num * 10 + (double)(*s - '0');
	        s++;
        }
      if (*s == '.')
      {
        // decimal part of number
		    s++;
        decimal = 1;
        // after point - only number
		    if  (*s == '\0' || (*s < '0') || (*s > '9') && *s != ' ' && *s != '\t')
		    {
          state = FRACTION;
          continue;
        }
        while ((*s != '\0') && (*s >= '0') && (*s <= '9'))
        {
		      T.Num += (double)(*s - '0') / (decimal *= 10);
		      s++;
        }
      }
	    if ((*s == 'e') || (*s == 'E'))
      {
        // sientific
		    degree = 0, signdegree = 0;
		    s++;
        // after 'e' - only number or +/-
        if (*s == '-')
		      signdegree = -1;
		    else if (*s == '+')
		      signdegree = 1;
		    else if ((*s >= '0') && (*s <= '9'))
		      degree = degree * 10 + (int)(*s - '0');
		    else
        {
		      state = SCIENTIFIC;
          continue;
        }
        s++;

        if (*s == '\0')
	      {
          // after +/- only number
		      if (signdegree != 0) 
          {
              state = SCIENTIFIC;
              continue;
          }
		      if (degree != 0)
			      T.Num = T.Num * pow(10.0, degree);
		      if (!Put(Q, T))
          {
            state = MEMORY_SCANNER;
            continue;
          }
			    return OK_SCANNER;
        }
        // if there were not +/-
        // after 'e' and numbers - no waste symbols
	      if ((*s < '0' || *s > '9') && (*(s-1) < '0' || *(s-1) > '9'))
        {
          state = SCIENTIFIC;
          continue;
        }

	      while ((*s >= '0') && (*s <= '9'))
	      {
	        if (degree < MAXDEGREE)
		        degree = degree * 10 + (int)(*s - '0');
		      s++;
	      }
	      if (degree != 0)
        {
          if (signdegree == -1)
	          T.Num = T.Num / pow(10.0, degree);
	        else
            T.Num = T.Num * pow(10.0, degree);
        }
      }
      break;
      default:
        /* meets another character, maybe it 
               name of a function or variable, otherwise - error */
        lBadLoop = TRUE;
        while (lBadLoop)
        {
          if (state != OK_SCANNER)
          {
            freeQueue(Q);
            return state;
          }
          // too many characters for the name of the function
          if (funcNameIndex > FUNC_NAME_SIZE-1)
          {
            state = SYMBOL;
            continue;
          }
          funcName[funcNameIndex++] = *s;
          // if the name of the function - one of these, the loop is completed
			    funcSymbol = getFuncSymbol(funcName);
          if (funcSymbol != '0')
			    {
            lBadLoop = FALSE;
			      T.Id = TOK_OP;
            T.Op = funcSymbol;
            funcNameIndex = 0;
            freeString(funcName);
            s++;
          }
          /* If after this symbol there is a space, 
                 operation or end of a line 
                 and this symbol is the single-letter variable,
              then exit the search function loop */
          else if (isOper(*(s+1)))
          {
            if (funcNameIndex == 1 && isAlpha(*s))
            {
              T.Id = TOK_NAME;
              T.Name = *s;
              lBadLoop = FALSE;
              funcNameIndex = 0;
              s++;
            }
            else
            {
              state = SYMBOL;
              continue;
            }
          }
          if (lBadLoop)
            s++;
        }
        break;
    }
	  if (!Put(Q, T))
    {
      state = MEMORY_SCANNER;
      continue;
    }
  }
  return OK_SCANNER;
}
