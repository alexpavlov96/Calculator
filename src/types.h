#ifndef _TYPES_H_
#define _TYPES_H_
#define MAX_VARS 51       
#define EXP 2.71828182846
#define PI 3.141592653589

typedef enum 
{
  FALSE, TRUE 
} boolean;

// type of token
typedef enum
{
  TOK_NUM,           // number
  TOK_OP,            // operation
  TOK_NAME           // variable
} TOKID;

// token structure
typedef struct
{
  TOKID Id;   // type of token
  double Num;
  char Op;
  char Name;
} TOK;

typedef struct tagLIST LIST;
struct tagLIST
{
  TOK T;
  LIST *Next;
};

typedef struct
{
  LIST *Head, *Tail;
} QUEUE;

typedef struct
{
  LIST *Top;
} STACK;

typedef struct
{
  char Name;
  double Val;
} VARIABLE;

#endif
