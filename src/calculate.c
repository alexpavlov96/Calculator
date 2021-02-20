#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "errorhandler.h"
#include "variable.h"
#include "utils.h"

/* calculate expression or return name of error */
calculate_t Calculate( QUEUE *Q, double *answer )
{
  STACK S = {NULL};                        // to push numbers and variables
  TOK T = {TOK_NUM, 0, '0', '0'},          // token to get from queue
      X = {TOK_NUM, 0, '0', '0'}, 
      Y = {TOK_NUM, 0, '0', '0'};          // to pop numbers and variables
  VARIABLE aVars[MAX_VARS] = {0};       
  double max = 1.7975 * pow(10.0,308);
  calculate_t state = OK_CALCULATOR; 
  
  while (Get(Q, &T))
  {
    if ( T.Id == TOK_NUM || T.Id == TOK_NAME )
	  {
	    if (T.Id == TOK_NUM && T.Num > max)
	      state = LARGE_NUMBER;
	    else if (!Push(&S, T))
        state = MEMORY_CALCULATOR;
	  }
	  else if ( T.Id == TOK_OP) 
    {
      switch (T.Op)
      {
        case ',':
          continue;
        // unary operations: minus and functions
        case '@':
        case 's': case 'c': case 't': case 'g':
        case 'q': case 'l': case 'f': case 'i':
        case 'x': case 'y': case 'z':
          myAssert("Can't POP the element!", Pop(&S, &X));
          if (X.Id == TOK_NAME)
            // changing variable to its value
            if (!varGet(aVars, &X))
              // it was not initalized
              state = INITALIZE;

          if (state == OK_CALCULATOR)
          {
            switch (T.Op)
            {
            // unary minus
            case '@':
	            if (!compareToDBL(X.Num, 0.0))
	              X.Num *= -1;
              break;
            // sin
            case 's':
		          X.Num = sin(X.Num);
              break;
            // cos
            case 'c':
		          X.Num = cos(X.Num);
              break;
            // tg
            case 't':
		          X.Num = tan(X.Num);
              break;
            // ctg
            case 'g':
		          if (compareToDBL(tan(X.Num), 0.0))
		            state = ZERO;
		          else
		            X.Num = 1/tan(X.Num);
		          break;
            // sqrt
            case 'q':
		          if (X.Num < 0)
			          state = SQRT;
		          else
		            X.Num = sqrt(X.Num);
		          break;
            // ln
            case 'l':
		          if (X.Num < 0 || compareToDBL((X.Num), 0))
		            state = LOG1;
		          else
		            X.Num = log(X.Num);
		          break;
            // floor
            case 'f':
              if (fabs(X.Num) < INT_MAX && !compareToINT(X.Num, (int)X.Num))
              {
                if (X.Num >= 0)
			            X.Num = 1.0 * (int)X.Num;
		            else
			            X.Num = 1.0 * (int)X.Num - 1;
              }
              break;
            // ceil
            case 'i':
		          if (fabs(X.Num) < INT_MAX && !compareToINT(X.Num, (int)X.Num))
              { 
		            if (X.Num <= 0)
			            X.Num = 1.0 * (int)X.Num;
		            else
			            X.Num = 1.0 * (int)X.Num + 1;
              }
              break;
            // arcsin
            case 'x':
		          if (fabs(X.Num) > 1)
		            state = ARCSIN;
		          else
		            X.Num = asin(X.Num);
		          break;
            // arccos
            case 'y':
		          if (fabs(X.Num) > 1)
		            state = ARCCOS;
		          else
		            X.Num = acos(X.Num);
		          break;
            // arctg
            case 'z':
		          X.Num = atan(X.Num);
              break;
          }
          if (!Push(&S,X))
            state = MEMORY_CALCULATOR;
          }
          break;
        // binary operations
	      case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case 'L':
          myAssert("Can't POP the element!", Pop(&S, &Y));
		      myAssert("Can't POP the element!", Pop(&S, &X));
          // finding values of variables
          if (X.Id == TOK_NAME)
            if (!varGet(aVars, &X))
              state = INITALIZE;
          if (Y.Id == TOK_NAME)
            if (!varGet(aVars, &Y))
              state = INITALIZE;

          if (state == OK_CALCULATOR)
          {
            switch (T.Op)
            {
			      case '+':
			        X.Num += Y.Num;
			      break;
			      case '-':
			        X.Num -= Y.Num;
			      break;
			      case '*':
			        X.Num *= Y.Num;
			      break;
			      case '/':
			        if (compareToDBL(Y.Num,0.0))
			          state = ZERO;
			        else
			          X.Num /= Y.Num;
			      break;
			      case '^':
		          if (compareToDBL(X.Num, 0.0) && Y.Num < 0)
			          state = POWER;
			        else
			          X.Num = pow( X.Num, Y.Num );
			      break;
            case 'L':
              if (X.Num < 0 || compareToDBL(X.Num, 0.0) || 
                  compareToDBL(X.Num, 1.0) ||
                  Y.Num < 0 || compareToDBL(Y.Num, 0.0))
                state = LOG2;
              else
                X.Num = log(Y.Num) / log(X.Num);
              break;
            }
            if (X.Num > max || X.Num < -max)
              state = LARGE_NUMBER;
            else if (!Push(&S,X))
              state = MEMORY_CALCULATOR;
          }
          break;
        case '=':
          myAssert("Can't POP the element!", Pop(&S, &Y));
          myAssert("Can't POP the element!", Pop(&S, &X));
          if (X.Id != TOK_NAME)
            // assignment to number
            state = LVALUE;

          if (state == OK_CALCULATOR)
          {
            /* if 'variable1' = 'variable2',
               finding the value of 'variable2' */
            if (Y.Id != TOK_NUM)
            {
              if (!varGet(aVars, &Y))
                state = INITALIZE;
            }
            myAssert("No place for variable!", varSet(aVars,X.Name,Y.Num));
          }
          break;
        case ';':
          freeStack(&S);
          continue;
      }
    }
    if (state != OK_CALCULATOR)
    {
      freeStack(&S);
      freeQueue(Q);
	    return state;
    }
  }
  /* no final expression */
  if (S.Top == NULL)
  {
    freeStack(&S);
    return NO_EXPRESSION;
  }
  // else - S.Top->T.Num is result
  myAssert("Can't POP the element!", Pop(&S, &X));

  if (S.Top != NULL)
  {
    freeStack(&S);
    freeQueue(Q);
	  return state;
  }

  if (X.Id != TOK_NUM)
    if (!varGet(aVars, &X))
     return INITALIZE;
  
  *answer = X.Num;
  return OK_CALCULATOR;
}
