/**@<parser.h>::**/

#define MAX_ARG_NUM 1024

extern int lookahead; // @ local

extern int gettoken (FILE *); // @ lexer.c

extern FILE *source; // @ main.c

extern void match (int); // @ local

/* syntax names */
void mypas(void);
void body(void);
void declarative(void);
int vartype(void);
int fnctype(void);
char **namelist();
void imperative(void);
void parmdef(void);
void stmtlist(void);
void stat(void);
int expr(int inherited_type);
int smpexpr (int inherited_type);
void dostmt(void);
void whilestmt(void);
void ifstmt(void);
int oprelop(void);
int addop (int type);
int mulop (int type);

