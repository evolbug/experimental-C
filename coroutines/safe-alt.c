/*                                                             *\
   !!!                 USES DUFF'S DEVICE                  !!!
   !!!   SWITCHES INSIDE COROUTINES MIGHT CAUSE PROBLEMS   !!!
\*                                                             */



#include <stdio.h> // malloc, free for cr_extern
#include <stdlib.h> // printf




#define cr_await(...) (__VA_ARGS__, 0)

#define cr_break(...) (__VA_ARGS__, -1)

#define cr_yield(...) {*cr_state = __LINE__; return __VA_ARGS__; case __LINE__:;}

#define cr_init(...)             \
   (struct _){0, ##__VA_ARGS__};}\
   if (cr_stop) _->_ = cr_stop;  \
   long* cr_state = &_->_;       \
   switch (_->_) { case 0:

#define coroutine(type, ...) cr_##type(__VA_ARGS__)



#define cr_static(...) (__VA_ARGS__, char cr_stop)

#define st_context(...)             \
   struct _ {long _; __VA_ARGS__;}; \
   static const struct _ __reset__; \
   static struct _ cr_ctx;          \
   struct _ *_ = &cr_ctx;           \
   if (!_->_) {*_

#define st_finish(end_flag) default: {cr_ctx=__reset__; return end_flag;}}



#define cr_extern(...) (__VA_ARGS__, void** cr_ctx, char cr_stop)

#define ex_context(...)             \
   struct _ {long _; __VA_ARGS__;}; \
   struct _ *_ = (struct _*)*cr_ctx;\
   if (!_) {                        \
      _ = malloc(sizeof(struct _)); \
      *cr_ctx = _; *_

#define ex_finish(end_flag) default: {if (_) free(*cr_ctx); *cr_ctx=NULL; return end_flag;}}



int st_cDown coroutine (static, int n)
{
   st_context(int i) = cr_init(n);

   while (_->i > 0)
      cr_yield (_->i--);

   st_finish(0);
}


int ex_cDown coroutine (extern, int n)
{
   ex_context(int i) = cr_init(n);

   while (_->i > 0)
      cr_yield (_->i--);

   ex_finish(0);
}



struct word {
   const char* begin;
   int length;
};

struct word parse coroutine (extern, const char* str) {
   struct word sentinel = {NULL, 0};

   ex_context(const char* begin; const char* end) = cr_init(str, str);

   for(;; _->end++)
   {
      if(!*_->end)
      {
         int len = _->end - _->begin;

         if(len > 0)
            cr_yield((struct word){_->begin, len});

         break;
      }

      int len = _->end - _->begin;

      if(*_->end == ' ')
      {
         if(len > 0)
            cr_yield((struct word){_->begin, len});

         while(*_->end == ' ') _->end++;

         _->begin = _->end;
      }
   }

   ex_finish(sentinel);
}


int main ()
{
   {
      printf("st_cDown:\n");

      int val = 0;

      while ((val = st_cDown cr_await(10))>5)
         printf("%d \n", val);

      printf("\n");
      st_cDown cr_break(10);

      while ((val = st_cDown cr_await(5)))
         printf("%d \n", val);

      printf("\n");

   }

   {
      printf("ex_cDown:\n");

      int val = 0;
      void* ctx = 0;

      while ((val = ex_cDown cr_await(10, &ctx))>5)
         printf("%d \n", val);

      printf("\n");
      ex_cDown cr_break(10, &ctx);

      while ((val = ex_cDown cr_await(5, &ctx)))
         printf("%d \n", val);

      printf("\n");
   }

   {
      printf("parsing strings:\n");

      void* ctx = NULL;
      const char* string = "Hello world I am a string";

      struct word current_word = parse cr_await(string, &ctx);
      while(current_word.begin) {
         printf("'%.*s'\n", current_word.length, current_word.begin);
         current_word = parse cr_await(NULL, &ctx);
      }
   }

}
