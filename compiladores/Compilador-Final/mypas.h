/**@<progcalc.h>::**/
#define MAX_EXTENSION_LEN 4

extern FILE *source;

extern FILE *object;

extern int lookahead;

extern int errcounter;

extern char *objfile;

char *isvalid(int argc, const char *entry, const char *out);
