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
void namelist();
void imperative(void);
void parmdef(void);
void stmtlist(void);
void stat(void);
void expr(void);
void smpexpr (void);
void dostmt(void);
void whilestmt(void);
void ifstmt(void);
int oprelop(void);
int addop (void);
int mulop (void);

