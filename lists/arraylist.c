#include <stdlib.h> // malloc, realloc
#include <stdio.h>  // printf
#include <stdarg.h> // va_list, va_start, va_arg, va_end

#define LSBODY(type) { \
   va_list args; va_start(args, call);\
   unsigned int* realptr; \
   if (list) realptr = ((unsigned int*)list) - 1; \
   else {realptr = malloc(sizeof(unsigned int)); *realptr = 0;} \
   if (*realptr + count > (unsigned int)-1) \
      exit(printf("% 9li: %s(): ("#type")list grows past %u items\n",line, call, -1)); \
\
   realptr = realloc(realptr, (*realptr + count) * sizeof(type) + sizeof(unsigned int)); \
   *realptr = (*realptr) + count; \
   list = (type*) realptr + 1; \
   for (unsigned int i = *realptr-count; i < *realptr; i++) \
      list[i] = va_arg(args, type); \
   return list; \
}

#define ListNewType(type) \
type*  list_add_##type \
(type*  list, int count, unsigned long line, const char* call, ...) LSBODY(type)

#define ListNewPtrType(type) \
type** list_add_p##type \
(type** list, int count, unsigned long line, const char* call, ...) LSBODY(type*)

#define List(type) type*

#define Append(list, count, type, ...) \
   list = list_add_##type (list, count, __LINE__, __FUNCTION__, __VA_ARGS__)

#define AppendPtr(list, count, type, ...) \
   list = list_add_p##type(list, count, __LINE__, __FUNCTION__, __VA_ARGS__)

unsigned int Length (void* l) { return *((unsigned int*)l - 1); }
void Delete (void* l) {void *realptr = ((unsigned int*)l) - 1; free(realptr);}

/*
ListNewType(int);

int main()
{
   List(int) myls = 0;
   Append(myls, 2, int, 10, 20);

   for (int i = 0; i< Length(myls); i++)
      printf("%d", myls[i]);

   Delete(myls);
}
*/
