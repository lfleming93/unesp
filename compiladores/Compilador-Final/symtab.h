/**@<symtab.h>::**/

#define MAX_SYMTAB_ENTRIES 0X10000
/*Table with the offset no the variable name and type associeted*/
extern int symtab[MAX_SYMTAB_ENTRIES][2];
/*Control the last entry on symbol tab*/
extern int symtab_nextentry;
/*Stream that store the name of all global variables*/
extern char symtab_stream[];
/*Keep the position of the next space to store a variable name on stream*/
extern int symtab_stream_next_descriptor;

/*Put the name and type on syntab and stream*/
int symtab_append(char const *name, int type);

/*Serach for the variable name on table*/
int symtab_lookup(char const *name);