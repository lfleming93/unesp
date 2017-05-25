	#define MAXID_SIZE 32

extern char *keywords[];
extern char lexeme[MAXID_SIZE+1];//@ lexer.c

/*Ignore spaces on source file*/
void skipspaces(FILE*);
/*Identifies a variable name or reserved word*/
int is_identifier(FILE*);
/*Control the exponencial part check*/
int is_exponencial(FILE*, int i);
/*Control the decimal part check*/
int is_fractional(FILE *tape, int i);
/*Check if it's a decimal number or float number*/
int is_number(FILE *tape);
/*Check if the next input is a relational operator or asgnment*/
int is_symbol(FILE *tape);
/*get the next token on stream*/
int gettoken (FILE *tokenstream);
/*check range float or double*/
int floatOrDouble();