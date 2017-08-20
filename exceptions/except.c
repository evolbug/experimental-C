/*
   EXCEPTION SYSTEM

   !!! DO NOT RETURN FROM THE TRY BLOCK, THE EXCEPTION STACK WILL GO TITS-UP !!!
   (returning from other blocks is fine though)
*/



#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>



enum EXCEPTIONS
{
   ZERO=1,
   ONE=2,
   TWO=3
};

struct exception_stack
{
   struct exception_stack *prev;
   jmp_buf jump;
};

struct exception_stack *ex_stack=0;


#define try \
   { struct exception_stack ex_node; ex_node.prev = ex_stack; ex_stack=&ex_node; \
      switch (setjmp(ex_node.jump)) { case 0:

#define except_any break; default: ex_stack=ex_node.prev;

#define except(ex) break; case (ex): ex_stack=ex_node.prev;

#define catch }}

#define finally break;}{

#define throw(ex) if (ex_stack) longjmp(ex_stack->jump, ex)



int
foo (int n)
{
   throw(n);
}



int
main ()
{
   try
   {
      foo(TWO);
      puts("afterfoo");
   }

   except(ZERO)
   {
      puts("caught a zero!");
   }

   except(ONE)
   {
      puts("caught a one!");
   }

   except(TWO)
   {
      puts("caught a two!");
   }

   except_any
   {
      puts("caught what?");
   }

   finally
   {
      puts("finally");
   }
   catch;

}
