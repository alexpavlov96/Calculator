#include <stdio.h>
#include <stdlib.h>
#include "qust.h"
#include "errorhandler.h"
#include "types.h"
#include "utils.h"

int main( int argc, char *argv[] )
{
  FILE *F = stdin;
  QUEUE Qscan = {NULL},             // Queue, where numbers, functions, variables are formed
		    Qres = {NULL};              // RPN queue
  char *pString = NULL,             // pointer to current string
       *p = NULL;                   // extra pointer for use at realloc 
  char c = 0;                       // symbol for reading
  boolean bMemState = TRUE,         // do we have memory for string
          bLoop = TRUE;             // main loop
  unsigned long lCurrSize = 0,      // current number of symbols at string
                lCurrMemory = 2;    // value of memory for realloc
  double answer = 0;
  
  if (argc == 2)
  {
    if (( F = fopen( argv[1], "r" )) == 0 )
    {
       printf("ERROR: Enable to open the file\n");
	     return -1;
    }
  }
  else if (argc != 1)
	{
    printf("ERROR: more than 2 arguments\n");
    return -1;
  }
  do
  {
    bMemState = TRUE;
    do
    {
      c = (char)fgetc(F);
      if (c == EOF)
        bLoop = FALSE;
      if (pString == NULL && bMemState)
      {
        if ((pString = (char *)malloc(lCurrMemory)) == NULL)
          bMemState = FALSE;
      }

      if (lCurrSize >= lCurrMemory-1 && bMemState)
      {
        lCurrMemory *= 2;
        if ((p = (char *)realloc(pString, lCurrMemory)) == NULL)
          bMemState = FALSE;
        if (bMemState)
          pString = p;
      }

      /*  if no memory, delete all characters 
          to the next line, 
          if it exists, */
      if (!bMemState && bLoop)
      {
        while (c != '\n' && c != EOF)
          c = (char)fgetc(F);
        printf("ERROR: not enough memory\n");
      }

      // add current symbol to string
      else if (bMemState)
      {
        pString[lCurrSize] = c;
        lCurrSize++;
      }
    }while (c != '\n' && (bLoop) && (bMemState));
    
    if (bMemState)
    {
      pString[lCurrSize-1] = 0;
      printf("%s", pString);

      // Not empty and not comment type string
      // goes to lexical analys (Scanner),
      // when forming RPN (Parser),
      // when getting result (Calculate)
      if (!isEmpty(pString) && !isComment(pString))
        if (ErrorHandlerOfScanner(Scanner(pString, &Qscan)))
          if (ErrorHandlerOfParser(Parser(&Qres, &Qscan)))
            if (ErrorHandlerOfCalculator(Calculate(&Qres, &answer)))
              printf(" == %g", answer);
      // if (F != stdin)
      // we don't need '\n' at the end
      if (c != EOF)
        printf("\n");
    }
    // default state
    free(pString);
    pString = NULL;
    lCurrSize = 0;
    lCurrMemory = 2;
  } while (bLoop);
  return 0;
}
