#include "types.h"
#include "variable.h"

/* Accepts an array of variables 
    and the name of the variable and the value encountered, 
    which is assigned to it. 
    Returns 0 if the array is full */

boolean varSet ( VARIABLE *V, char Name, double Val )
{
  int i = 0;
  while (i < MAX_VARS && V[i].Name != '\0')
  {
    if (V[i].Name == Name)
    {
      V[i].Val = Val;
      return TRUE;
    }
    i++;
  }
  if (i >= MAX_VARS)
    return FALSE;
  if (V[i].Name == '\0')
  {
    V[i].Name = Name;
    V[i].Val = Val;
    return TRUE;
  }
  return FALSE;
}

/* Accepts an array of variables 
    and the token type "name". 
    Token is assigned by the value from the array. 
    Returns FALSE if this variable is not exists in array */

boolean varGet ( VARIABLE *V, TOK *T )
{
  int i = 0;
  while (i < MAX_VARS && V[i].Name != '\0')
  {
    if (V[i].Name == T->Name)
    {
      T->Id = TOK_NUM;
      T->Num = V[i].Val;
      return TRUE;
    }
    i++;
  }
  return FALSE;
}
