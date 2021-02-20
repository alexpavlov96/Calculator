#include <stdlib.h>
#include <assert.h>
#include "errorhandler.h"
#include "utils.h"

// for right assotiativity of degree '^'
#define HIGHEST_PRIORITY 7

int GetPriority( char c )
{
  if (isfunction(c))
    return 2;
  switch (c)
  {
  case ';':
    return 0;
  case '=':
    return 1;
  case '(':
    return 2;
  case ')':
  case ',':
    return 3;
  case '+':
  case '-':
	case '@':
    return 4;
  case '*':
  case '/':
    return 5;
  case '^':
    return 6;
  }
  return 0;
}

static boolean DropOpers ( QUEUE *Q, STACK *S, int Priority )
{
  TOK T = {TOK_NUM, 0, '0', '0'};
  while (S->Top != NULL && GetPriority(S->Top->T.Op) >= Priority)
  {
    myAssert("\nCan't POP the element!\n", Pop(S, &T));
    if (!Put(Q, T))
     return FALSE;
  }
  return TRUE;
}


/* Polish Notation */
parser_t Parser ( QUEUE *Qres, QUEUE *Qscan )
{
  parser_t state = WAIT_PREFIX;

  int brackets = 0,                 // balance of brackets
      log_state = 0,                // balance of commas and logarifm symbols
      prior = 0;                    // to get priority 
                                    // considering right associativity of degree
  TOK T = {TOK_NUM, 0, '0', '0'};            
  STACK S = {NULL};                

  while ( state != END )
  {
    if (state != ERROR && state != MEMORY_PARSER && state != BRACKETS)
    {
      /* when tokens are ended, check,
         what we are waiting for, and balance of brackets */
      if (!Get(Qscan, &T))
      {
       if ((state == WAIT_SUFFIX) && (brackets == 0))
          state = DONE;
       else if (brackets != 0)
         state = BRACKETS;
       else if ((state != DONE) && (state != MEMORY_PARSER) && (state != BRACKETS) && (T.Id == TOK_NUM))
	       state = ERROR;
      }
    }
   switch (state)
   {

   case WAIT_PREFIX:
     if (T.Id == TOK_NUM || T.Id == TOK_NAME)
     {
       if (Put(Qres, T))
         state = WAIT_SUFFIX;
       else
         state = MEMORY_PARSER;
     }
     else if (T.Id == TOK_OP)
	   {
		   if (T.Op == '(' || isfunction(T.Op))
       {
         if (!Push(&S, T))
           state = MEMORY_PARSER;
		     brackets++;
         if (T.Op == 'L')
           log_state++;
       }
		  // unary minus
      else if (T.Op == '-')
		  {
        T.Op = '@';
		    if (!Push(&S, T))
          state = MEMORY_PARSER;
		  }
		  else
			  state = ERROR;
		}
    break;

    case WAIT_SUFFIX:
       if (T.Id == TOK_OP)
       {
         if (T.Op != ')')
         {
           /* only the degree have the right associativity, 
               so when you hit "^" in the stack operations 
               if on top of it was "^", it remains there */ 
           if (T.Op != '^')
             prior = GetPriority(T.Op);
		       else
             prior = HIGHEST_PRIORITY;

 		       if (!DropOpers(Qres, &S, prior))
             state = MEMORY_PARSER;
           
           /* before the operation "," should be 
               sign of the logarithm function "L"*/
           else if (T.Op == ',')
           {
             if (S.Top == NULL)
               state = ERROR;
             else if (S.Top->T.Id != TOK_OP || S.Top->T.Op != 'L')
               state = ERROR;
             log_state--;
           }
           // operation ";" has the lowest priority, 
           // it goes directly in the queue

           if (T.Op == ';')
           {
             if (!Put(Qres, T))
               state = MEMORY_PARSER;
           }
           // other operations - in the stack
           else
           {
             if (!Push(&S, T))
               state = MEMORY_PARSER;
           }
           if (state != ERROR && state != MEMORY_PARSER)
             state = WAIT_PREFIX;
		     }
	       else if (T.Op == ')')
         {
           /*  Removing all operations before the meeting "(" or character of function. 
               Removing bracket or function symbol. 
               If this is function symbol, it is put 
               in queue as unary operation */

     	     if (!DropOpers(Qres, &S, GetPriority(T.Op)))
             state = MEMORY_PARSER;
		       else if (!Pop(&S, &T))
             state = BRACKETS;
		       else if (isfunction(T.Op))
           {
             if (!Put(Qres, T))
               state = MEMORY_PARSER;
           }
           if (state != MEMORY_PARSER && state != BRACKETS)
             state = WAIT_SUFFIX;
		       brackets--;
         }
       }
	     else
	    	 state = ERROR;
       break;

    case ERROR:
    case MEMORY_PARSER:
    case BRACKETS:
      freeQueue(Qres);
      freeQueue(Qscan);
      freeStack(&S);
      return state;
    
    case DONE:
      /* All the remaining operations in the stack 
         are put in the queue */
      if (!DropOpers(Qres, &S, 0))
        state = MEMORY_PARSER;
      else if (S.Top == NULL)
      {
        if (log_state != 0)
          // number of commas and symbols of logarithm 
          // does not match
          state = ERROR;
        else if (state != MEMORY_PARSER)
          state = END;
        return state;
      }
      else
        state = ERROR;
      break;
    }
  }
  return ERROR;
}
