// stack traces

#include <stdlib.h>  // exit
#include <stdio.h>   // printf

struct st_entry
{
   char file[64], call[64];
   unsigned int line;
};

struct {struct st_entry entry[255]; unsigned char curr;} ST = {.curr=0};


#define st_push(fn)\
   ST.entry[ST.curr] = \
      (struct st_entry) {.file=__FILE__, .call=#fn, .line=__LINE__};\
   if ((++ST.curr)+1 > 255) {puts("\nFunction stack overflow"); st_trace(); exit(1);}


#define st_assert(cond, msg) \
   ({if (!(cond)){\
      st_push(st_assert(cond, msg));\
      printf("\nAssert: %s", msg);\
      st_trace(); exit(1);}\
   cond;})


int st_entry_equal (struct st_entry* a, struct st_entry* b)
{
   int f = 0, c = 0; // file/call readers

   if (a->line != b->line) return 0;
   while (a->file[f]!=0 || b->file[f]!=0) if (a->file[f] != b->file[f]) return 0; else f++;
   while (a->call[c]!=0 || b->call[c]!=0) if (a->call[c] != b->call[c]) return 0; else c++;

   return 1;
}



// typed call
#define st_c(rtype, fn) ({ st_push(fn); rtype r = fn; ST.curr--; r;})

// void call
#define st_v(fn) ({ st_push(fn); fn; ST.curr--;})



void
st_trace ()
{
   printf("\nTraceback (last call first):\n");

   struct st_entry *entry;
   int skip = 0;

   for (int u = ST.curr; u > 0; u--)
   {
      entry = &ST.entry[u-1];

      if (skip)
      {
         if (!st_entry_equal(entry, &ST.entry[u]))
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

         if (st_entry_equal(entry, &ST.entry[u]))
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
   return st_assert(n%2, "oh no! it's even!");
}

int
foo (int n)
{
   return st_c(int, checkeven(n));
}

int
recurs(int n)
{
   if (n>0) return st_c(int, recurs(n-1));
   return 0;
}

int
main ()
{
   st_c(int, foo(13));
   st_c(int, recurs(255)); // 254 and overflow triggers
   st_c(int, foo(12));
}
