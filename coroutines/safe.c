/*                                                             *\
   !!! SWITCHES ARE NOT ALLOWED INSIDE THIS IMPLEMENTATION !!!
   !!!                 USES DUFF'S DEVICE                  !!!
\*                                                             */


#include <stdio.h>
#include <stdlib.h>


#define coroutine(...) \
   (void** context, ##__VA_ARGS__)


#define crContext(name, ...)                  \
   struct __ctx {long __s; __VA_ARGS__;};     \
   struct __ctx* name;                        \
   if (!*context) {                           \
      *context = malloc(sizeof(struct __ctx));\
      (*(struct __ctx*)(*context))

#define crInit(...) \
   (struct __ctx){.__s=0, ##__VA_ARGS__}; }


#define crBegin(name)              \
   name = (struct __ctx*) *context;\
   long* __state__ = &name->__s;   \
   switch (*__state__) {           \
      case 0: {


#define yield(...) \
   {*__state__ = __LINE__; return __VA_ARGS__; case __LINE__:;}

#define crEnd(end_flag) \
   free(*context); return end_flag;}};


int countDown coroutine (int n)
{
   crContext(ctx, int i) = crInit(n);
   crBegin(ctx);

   while (ctx->i > 0)
      yield (ctx->i--);

   crEnd(0);
}


struct word {
   const char* begin;
   int length;
};

struct word parse coroutine (const char* str) {
   struct word sentinel = {NULL, 0};

   crContext(ctx, const char* begin; const char* end) = crInit(str, str);
   crBegin(ctx)

   for(;; ctx->end++)
   {
      if(!*ctx->end)
      {
         int len = ctx->end - ctx->begin;

         if(len > 0)
            yield((struct word){ctx->begin, len});

         break;
      }

      int len = ctx->end - ctx->begin;

      if(*ctx->end == ' ')
      {
         if(len > 0)
            yield((struct word){ctx->begin, len});

         while(*ctx->end == ' ') ctx->end++;

         ctx->begin = ctx->end;
      }
   }

   crEnd(sentinel);
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
