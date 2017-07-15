#include <stdio.h>
#include <stdlib.h>


#define _CAT(a, b) a ## b
#define CAT(a, b) _CAT(a, b)


#define coroutine(...) (void** __CRCTX__, ##__VA_ARGS__)


#define crBegin(contextName, staticVars, ...)                             \
   struct __ctx {void** __CRJMP__; staticVars;}; struct __ctx* contextName;\
   if (!*__CRCTX__) {                                                      \
      *__CRCTX__ = (struct __ctx*) malloc(sizeof(struct __ctx));           \
      (*(struct __ctx*)(*__CRCTX__)) =                                     \
         (struct __ctx){calloc(1, sizeof(void*)),__VA_ARGS__};             \
   }                                                                       \
   contextName = (struct __ctx*) *__CRCTX__;                               \
   void** __CRJMP__ = contextName->__CRJMP__;                              \
   if (*__CRJMP__) goto *(*__CRJMP__);


#define crEnd(...)                                \
   free(((struct __ctx*)(*__CRCTX__))->__CRJMP__); \
   free(*__CRCTX__);                               \
   *__CRCTX__ = NULL;                              \
   return __VA_ARGS__;


#define yield(...) \
   {*__CRJMP__ = &&CAT(__CRJMP_, __LINE__); return __VA_ARGS__; CAT(__CRJMP_, __LINE__):;}


int countDown coroutine (int n)
{
   crBegin(ctx, int i, n);

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

   crBegin(ctx,
      const char* begin; const char* end,
      str, str
   );

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
