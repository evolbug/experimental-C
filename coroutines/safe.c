/*                                                             *\
   !!! SWITCHES ARE NOT ALLOWED INSIDE THIS IMPLEMENTATION !!!
   !!!                 USES DUFF'S DEVICE                  !!!
\*                                                             */


#include <stdio.h>
#include <stdlib.h>


#define coroutine(...) \
   (void** context, ##__VA_ARGS__)


#define crContext(name, ...)                                 \
   struct __ctx {long __s; __VA_ARGS__;};                    \
   struct __ctx* name;                                       \
   if (!*context) {                                          \
      *context = (struct __ctx*)malloc(sizeof(struct __ctx));\
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

#define crFinish(end_flag) \
   free(*context); return end_flag;}};


int countDown coroutine (int n)
{
   crContext(ctx, int i) = crInit(n);
   crBegin(ctx)

   while (ctx->i > 0)
      yield (ctx->i--);

   crFinish(0);
}


int main ()
{
   void* crcontext = 0;
   int n = 0;

   while ((n=countDown(&crcontext, 10)))
      printf("%d\n", n);
}
