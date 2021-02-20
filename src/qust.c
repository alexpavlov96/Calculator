#include <stdio.h>
#include <stdlib.h>
#include "qust.h"
#include "utils.h"

boolean Put( QUEUE *Q, TOK A)
{
  LIST *New = NULL;
  if ((New = (LIST *)malloc(sizeof(LIST))) == NULL)
    return FALSE;
  if (Q->Head == NULL)
    Q->Head = Q->Tail = New;
  else
  {
    Q->Tail->Next = New;
    Q->Tail = New;
  }
  New->T = A;
  New->Next = NULL;
  return TRUE;
}

boolean Get( QUEUE *Q, TOK *A)
{
  LIST *Old;
  if (Q->Head == NULL)
    return FALSE;
  Old = Q->Head;
  *A = Old->T;
  Q->Head = Q->Head->Next;
  if (Q->Head == NULL)
    Q->Tail = NULL;
  free(Old);
  return TRUE;
}



boolean Push( STACK *S, TOK A )
{
  LIST *New;
  if ((New = (LIST *)malloc(sizeof(LIST))) == NULL)
    return FALSE;
  New->Next = S->Top;
  New->T = A;
  S->Top = New;
  return TRUE;
}


boolean Pop( STACK *S, TOK *A)
{
  LIST *Old;
  Old = S->Top;
  if (Old == NULL)
    return FALSE;
  S->Top = S->Top->Next;
  *A = Old->T;
  free(Old);
  return TRUE;
}
