/*                                                             *\
   !!!                 USES DUFF'S DEVICE                  !!!
   !!!   SWITCHES INSIDE COROUTINES MIGHT CAUSE PROBLEMS   !!!
\*                                                             */



#include <stdio.h>
#include <stdlib.h>



#define coroutine(...) \
   (void** context, ##__VA_ARGS__)


#define cr_context(name, ...)                  \
   struct __ctx {long __s; __VA_ARGS__;};     \
   struct __ctx* name;                        \
   if (!*context) {                           \
      *context = malloc(sizeof(struct __ctx));\
      (*(struct __ctx*)(*context))

#define cr_init(...) \
   (struct __ctx){.__s=0, ##__VA_ARGS__}; }


#define cr_begin(name)              \
   name = (struct __ctx*) *context;\
   long* __state__ = &name->__s;   \
   switch (*__state__) {           \
      case 0:


#define cr_yield(...) \
   {*__state__ = __LINE__; return __VA_ARGS__; case __LINE__:;}

#define cr_end(end_flag) \
   free(*context); return end_flag;};



int countDown coroutine (int n)
{
   cr_context(ctx, int i) = cr_init(n);
   cr_begin(ctx);

   while (ctx->i > 0)
      cr_yield (ctx->i--);

   cr_end(0);
}


struct word {
   const char* begin;
   int length;
};

struct word parse coroutine (const char* str) {
   struct word sentinel = {NULL, 0};

   cr_context(ctx, const char* begin; const char* end) = cr_init(str, str);
   cr_begin(ctx)

   for(;; ctx->end++)
   {
      if(!*ctx->end)
      {
         int len = ctx->end - ctx->begin;

         if(len > 0)
            cr_yield((struct word){ctx->begin, len});

         break;
      }

      int len = ctx->end - ctx->begin;

      if(*ctx->end == ' ')
      {
         if(len > 0)
            cr_yield((struct word){ctx->begin, len});

         while(*ctx->end == ' ') ctx->end++;

         ctx->begin = ctx->end;
      }
   }

   cr_end(sentinel);
}



int main ()
{
   {
      printf("countdown:\n");

      void* ctx = NULL;
      int val = 0;

      while ((val = countDown(&ctx, 10)))
         printf("%d \n", val);

      printf("\n");
   }
   {
      printf("parsing strings:\n");

      void* ctx = NULL;
      const char* string = "Hello world I am a string";

      struct word current_word = parse(&ctx, string);
      while(current_word.begin) {
         printf("'%.*s'\n", current_word.length, current_word.begin);
         current_word = parse(&ctx, NULL);
      }
   }
}
