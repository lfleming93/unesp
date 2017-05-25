/**@<pseudoassembly.c>**/

#define MAX_STACK 64

/*unifined label_counter*/
extern int labelcounter;
extern int FLAG_ERR;

int iscompatible(int ltype, int rtype);
int iscompatibleVariable(int ltype, int rtype);	

/*print assembly start*/
void prolog(void);
void epilog(void);

/*pseudo-assembly global variable*/
int symtab_assembly(void);

/*control-pseudo-in truction*/
int gofalse(int label);
int jump(int label);
int mkLabel(int label);

/*convert convert lexeme to appropriate number*/
int immediateinst(int acctype);

/*Control data transfer*/
int lmove(const char *variable, int type);
int rmove(const char *variable, const char *variable_two, int type);

/*Convert assembly register value to  left type register - convert operation on register only*/
void convertRegtoStack(int ltype,int rtype);
/*Used to convert const lexeme to value*/
void convertConst(int acctype, char **lower32, char **higher32);

/*Convert function - used only on arithmetic operators*/
void convertToReal(int storedtype);
void convertToDouble(int storedType);

/*ULA-pseudo-intruction*/
/*Negate operation*/
int neginst(int type);
/*Add | Sub | OR*/
int addinst(int inst, int storedtype, int acctype);
/*Mul | Div | AND*/
int mulinst(int inst, int storedtype, int acctype);
/*Relation operation to pseudo-assembly*/
void relinst(int op, int storetype, int acctype);