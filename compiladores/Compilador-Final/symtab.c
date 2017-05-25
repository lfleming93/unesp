/**@<symtab.c>::**/

#include <string.h>
#include <stdio.h>
#include <lexer.h>
#include <tokens.h>
#include <symtab.h>

int symtab[MAX_SYMTAB_ENTRIES][2];
int symtab_nextentry = 0;
char symtab_stream[MAX_SYMTAB_ENTRIES*(MAXID_SIZE+1)];
int symtab_stream_next_descriptor = 0;


int symtab_lookup(char const *name)
{
  /*Search for variable name on systab array
   if found return int position value
   if not found return -1*/
  int i;
  for(i=symtab_nextentry-1; i>-1; i--)
  {
    if(strcmp(symtab_stream + symtab[i][0], name) == 0) break;
  }
  return i;

}

int symtab_append(char const *name, int type)
{
    /*Store name on symtab_stream and returns next entry position
    return SYNTAB_OVERFLOW if symtab overflow
    return ALR_DEF if name already on symtab_stream
    */

  if(symtab_nextentry == MAX_SYMTAB_ENTRIES) return SYNTAB_OVERFLOW;
  if(symtab_lookup(name) > -1) return ALR_DEF;

  strcpy(symtab_stream + symtab_stream_next_descriptor, name);
  /* store the stream position in the symtab array */
  symtab[symtab_nextentry][0] = symtab_stream_next_descriptor;
  /*preview next stream entry position */
  symtab[symtab_nextentry][1] = type;
  /*preview next stream entry position */
  symtab_stream_next_descriptor += strlen(name)+1;

  return symtab_nextentry++;

}

