// stack traces

#include <stdlib.h>  // exit
#include <stdio.h>   // printf

struct __stackentry
{
   char file[64], call[64];
   unsigned int line;
};

struct {struct __stackentry entry[255]; unsigned char curr;} __s = {.curr=0};


#define __s_push(fn)\
   __s.entry[__s.curr] = \
      (struct __stackentry) {.file=__FILE__, .call=#fn, .line=__LINE__};\
   if ((++__s.curr)+1 > 255) {puts("\nFunction stack overflow"); strace(); exit(1);}


// default call
#define c(rtype, fn) ({ __s_push(fn); rtype r = fn; __s.curr--; r;})

// void call
#define v(fn) ({ __s_push(fn); fn; __s.curr--;})


#define assert(cond, msg) \
   ({if (!(cond)){\
      __s_push(assert(cond, msg));\
      printf("\nAssert: %s", msg);\
      strace(); exit(1);}\
   cond;})

int __entry_equal (struct __stackentry* a, struct __stackentry* b)
{
   int f = 0, c = 0; // file/call readers

   if (a->line != b->line) return 0;
   while (a->file[f]!=0) if (a->file[f] != b->file[f]) return 0; else f++;
   while (a->call[c]!=0) if (a->call[c] != b->call[c]) return 0; else c++;

   return 1;
}

void
strace ()
{
   printf("\nTraceback (last call first):\n");

   struct __stackentry *entry;
   int skip = 0;

   for (int u = __s.curr; u > 0; u--)
   {
      entry = &__s.entry[u-1];

      if (skip)
      {
         if (!__entry_equal(entry, &__s.entry[u]))
         {
            printf(" %d more ...\n", skip);
            skip = 0;
         }

         else
            skip++;
      }

      if (!skip)
      {
         printf("%12s:%-5d| %s\n", entry->file, entry->line, entry->call);

         if (__entry_equal(entry, &__s.entry[u]))
         {
            skip++;
            printf("%12s", "...");
         }
      }
   }

}



int
checkeven(int n)
{
   return assert(n%2, "oh no! it's even!");
}

int
foo (int n)
{
   return c(int, checkeven(n));
}

int
recurs(int n)
{
   if (n>0) return c(int, recurs(n-1));
   return 0;
}

int
main ()
{
   c(int, foo(13));
   c(int, recurs(253)); // 254 and overflow triggers
   c(int, foo(12));
}
