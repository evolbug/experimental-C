/*
   EXCEPTION SYSTEM

   !!! DO NOT RETURN FROM THE TRY BLOCK, THE EXCEPTION STACK WILL GO TITS-UP !!!
   (returning from other blocks is fine though)
*/



#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>



struct exception
{
   const char* message;
   const long ID;
};

#define Exception(name, message) struct exception name = {message, (long)&name}


struct exception_stack
{
   struct exception_stack *prev;
   long ex_id;
   jmp_buf jump;
}
*ex_stack=0;


int
caught(int id, struct exception *exlist)
{
   while (exlist->ID != 0)
      if ((int) exlist->ID == id)
         return 1;

      else
         exlist++;

   return 0;
}

#define try \
   { struct exception_stack ex_node; ex_node.prev = ex_stack; ex_stack=&ex_node; \
      if(!(ex_node.ex_id = (int)setjmp(ex_node.jump))) {

#define except_any } else { ex_stack=ex_node.prev;

#define except(...) \
   } else if (caught(ex_node.ex_id, (struct exception[]){__VA_ARGS__, {0}})) { ex_stack=ex_node.prev;

#define catch }}

#define finally }{

#define throw(ex) ex_stack ? longjmp(ex_stack->jump, ex.ID) : ({puts(ex.message); exit(ex.ID);})


const Exception(MyException,      "OSHIT THIS AN EXCEPTION");
const Exception(MyOtherException, "OSHIT THIS ANOTHER EXCEPTION");



int
foo (int n)
{
   switch(n)
   {
      case 0:
         throw(MyException);

      case 1:
         throw(MyOtherException);
   }

   return n;
}



int
main ()
{
   try
   {
      foo(3);
      puts("afterfoo");
   }

   except(MyException)
   {
      puts("caught a MyException!");
   }

   except(MyOtherException)
   {
      puts("someother exception");
   }

   except_any
   {
      puts("anything goes!");
   }

   finally
   {
      puts("finally");
   }
   catch;

}
