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


int countDown coroutine(int n)
{
   crBegin(ctx, int i, n);

   while (ctx->i > 0)
      yield (ctx->i--);

   crEnd(0);
}


int main ()
{
   void* crcontext = 0;
   int n = 0;

   while ((n=countDown(&crcontext, 10)))
      printf("%d\n", n);
}
