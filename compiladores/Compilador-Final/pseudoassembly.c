/**@<pseudoassembly.c>**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mypas.h>
#include <keywords.h>
#include <symtab.h>
#include <tokens.h>
#include <lexer.h>
#include <pseudoassembly.h>

/************************************** Semantic **************************************************
 *
 * OP	  | BOOLEAN     | NUMERIC |
 * NOT	  |    X	    |    NA
 * OR	  |    X	    |    NA
 * AND	  |    X        |    NA
 * CHS	  |    NA	    |    X
 * '+''-' |    NA 	    |    X
 * '*''/' |    NA 	    |    X
 * DIV    |    NA       |   INTEGER
 * MOD    |    NA       |   INTEGER
 * RELOP  | BOOL X BOOL |   NUM X NUM
 *
 *
 * EXPRESS  | INTEGGER | REAL   | DOUBLE
 * =====================================
 * INTEGER  | INTEGER  | REAL   | DOUBLE
 * REAL     |   REAL   | REAL   | DOUBLE
 * DOUBLE   |  DOUBLE  | DOUBLE | DOUBLE
 *
 *
 * LVALUE  | BOOLEAN | INTEGER | REAL   | DOUBLE
 * =============================================
 * BOOLEAN | BOOLEAN |    NA   |  NA    |   NA
 * INTEGER |   NA    | INTEGER |  NA    |   NA
 * REAL	   |   NA    |   REAL  |  REAL  |   NA
 * DOUBLE  |   NA    |  DOUBLE | DOUBLE | DOUBLE
 *
 *
 ************************************************************************************************/

/*Control the number of labels*/
int labelcounter = 1;
/*Flag of semantic error*/
int errcounter = 0;

/*
 * EXPRESS  | INTEGGER | REAL   | DOUBLE
 * =====================================
 * INTEGER  | INTEGER  | REAL   | DOUBLE
 * REAL     |   REAL   | REAL   | DOUBLE
 * DOUBLE   |  DOUBLE  | DOUBLE | DOUBLE
 */
int iscompatible(int ltype, int rtype)
{
	if(ltype == ERR_FATAL || rtype == ERR_FATAL)
		return 0;

	if(ltype == BOOLEAN || rtype == BOOLEAN)
    	return (ltype == rtype || ltype == 0 || rtype == 0);
    
    return 1;
}

int iscompatibleVariable(int ltype, int rtype)
{ 	/*Is compatible for asignment variable*/
	if(rtype == ERR_FATAL)	
		return 0;

	if(ltype == BOOLEAN || rtype == BOOLEAN)
        return (ltype == rtype || ltype == 0 || rtype == 0);
  
    return ltype >= rtype;
}

void prolog(void)
{
	/*Assenbly main prolog*/
	fprintf(object, "\t.text\n");
	fprintf(object, "\t.globl  main\n");
	fprintf(object, "\t.type main, @function\n");
	fprintf(object, "main:\n");
	fprintf(object, ".LFB0:\n");
	fprintf(object, "\tpushq %%rbp\n");
	fprintf(object, "\tmovq  %%rsp, %%rbp\n");
}

void epilog()
{
	/*Assembly main epilog*/
	fprintf(object, "\tmovl $0, %%rax\n");
	fprintf(object, "\tpopq	%%rbp\n");
	fprintf(object, ".LFE0:\n");
	fprintf(object, "\t.size	main, .-main\n");
	fprintf(object, "\t.section	.rodata\n");
	fprintf(object, "\t.align 4\n");
}

int symtab_assembly(void)
{
	/*Pseudo-Assembly definition of global variables*/
  int i;

  for(i = 0 ; i < symtab_nextentry ; i++)
  {
    switch(symtab[i][1])
    {
      case BOOLEAN:
       	fprintf(object, "\t.comm %s,1,1\n", symtab_stream + symtab[i][0]);
        break;
      case INTEGER:
        fprintf(object, "\t.comm %s,4,4\n", symtab_stream + symtab[i][0]);
        break;
      case REAL:
        fprintf(object, "\t.comm %s,4,4\n", symtab_stream + symtab[i][0]);
        break;
      case DOUBLE:
        fprintf(object, "\t.comm %s,8,8\n", symtab_stream + symtab[i][0]);
        break;
    }
  }
}

int gofalse(int label)
{
  fprintf(object, "\tcmpl $0, %%eax\n");
  fprintf(object, "\tjz .L%d\n", label);
  return label;
}

int jump(int label)
{
  fprintf(object,"\tjmp .L%d\n", label);
  return label;
}
int mkLabel(int label)
{
  fprintf(object, ".L%d:\n", label);;
  return label;
}

int neginst(int type)
{	/*negate the accumulated register*/
	switch(type)
	{
	case BOOLEAN:
		fprintf(object, "\tnot %%al\n");
		return BOOLEAN;

	case INTEGER:
		fprintf(object, "\tnegl %%eax\n");
		return INTEGER;

	case REAL:
		fprintf(object, "\tnegss %%eax\n");
		return REAL;

	case DOUBLE:
		fprintf(object, "\tnegsq %%rax\n");
		return DOUBLE;
	}
}

void convertConst(int acctype, char **lower32, char **higher32)
{	
	/*Used to convert const lexeme to value*/
    *lower32 = malloc(sizeof(lexeme+2));
    *higher32 = malloc(sizeof(lexeme+2));
    switch(acctype)
    {
        case INTEGER:
        	sprintf(*lower32 ,"$%s", lexeme);
        	*higher32 = NULL;
        	break;
        case REAL:
        {
        	float lex_val = atof(lexeme);
        	sprintf(*lower32 ,"$%i", *((int*)&lex_val));
        	*higher32 = NULL;
        }
        	break;

        case DOUBLE:
        {
        	/*Lower receive lower 32 bits of double and Higher receive 32bits higher*/
        	double lex_val = atof(lexeme);
   			long int temp = 0;
   			temp = (*((long int*)&lex_val) | temp) >> 32;
        	sprintf(*lower32 ,"$%u", *((unsigned int*)&lex_val));
        	sprintf(*higher32 ,"$%u", *((unsigned int*)&temp));
        }
        	break;
       	default:
       		strcpy(*lower32, "$0");
       		*higher32 == NULL;
    }
}

void convertRegtoStack(int ltype,int rtype)
{
	/*Convert synthetized accumulator value to stack type*/
	switch(ltype)
	{
		case REAL:
		/*Only case integer to real*/
		if(rtype == INTEGER)
			fprintf(object, "\tcvtsi2ss %%eax, %%eax\n");
		break;
		case DOUBLE:
			switch(rtype)
			{
			case INTEGER:
				fprintf(object, "\tcvtsi2sd %%eax, %%rax\n");
				break;
			case REAL:
				fprintf(object, "\tcvtss2sd %%eax, %%rax\n");
				break;
			}
	}
}

void convertToReal(int storedtype)
{	/*Take from stack using the stored type to store on register with the actual acctype
	if stored type < acctype then use cvt instruction
	else just mov from stack*/
	switch(storedtype)
	{
	case INTEGER:
		fprintf(object, "\tcvtsi2ss (%%rsp), %%xmm1\n"); //Convert stack integer to single precision
		break;
	case REAL:
		fprintf(object, "\tmovss (%%rsp), %%xmm1\n"); //Just move from stack to register
		break;
	}
}

void convertToDouble(int storedtype)
{	/*Take from stack using the stored type to store on register with the actual acctype
	if stored type < acctype then use cvt instruction
	else just mov from stack*/
	switch(storedtype)
	{
	case INTEGER:
		fprintf(object, "\tcvtsi2sd (%%rsp), %%xmm1\n"); //Convert stack integer to double precision
		break;
	case REAL:
		fprintf(object, "\tcvtss2sd (%%rsp), %%xmm1\n");	//Convert stack real single precision to double precision
		break;
	case DOUBLE:
		fprintf(object, "\tmovsd (%%rsp), %%xmm1\n"); // Just move from stack to register
		break;
	}
}

int immediateinst(int acctype)
{
    char *lower = NULL, *higher = NULL; // Constant value lower 32 bits and higher 32 bits (double only)
            
    convertConst(acctype, &lower, &higher);
    rmove(lower, higher, acctype);
    free(lower);
    free(higher);
}

int mulinst(int inst, int storedtype, int acctype)
{	/*Check if it's a instruction of multiplicaiton or division
	if true then check type to translate
	else return 0*/
	switch(inst)
	{
	case '*': case AND:
		switch(acctype)
		{
		case BOOLEAN:
			/*1 byte and instruction */
			fprintf(object, "\tandb %%al, (%%rsp)\n");
			fprintf(object, "\tmovb (%%rsp), %%al\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '*';

		case INTEGER:
			/*4 byte multiplication instruction*/
			fprintf(object, "\tmull %%eax, (%%rsp)\n");
			fprintf(object, "\tmovl (%%rsp), %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '*';

		case REAL:
			/*stored type can only be less than or equal to acctype*/
			convertToReal(storedtype);
			/*4 bytes single precision multiplication instruction*/
			fprintf(object, "\tmovss %%eax, %%xmm0\n");
			fprintf(object, "\tmulss %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovss %%xmm0, %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '*';

		case DOUBLE:
			/*stored type can only be less than or equal to acctype*/
			convertToDouble(storedtype);
			/*8 bytes double precision multiplication instruction*/
			fprintf(object, "\tmovsd %%rax, %%xmm0\n");
			fprintf(object, "\tmulsd %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovsd %%xmm0, %%rax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '*';
		}
	break;

	case '/':
		switch(acctype)
		{
		case INTEGER:
			/*4 byte division instruction*/
		  	fprintf(object, "\tdivl	 %%eax, (%%rsp)\n");
		  	fprintf(object, "\tmovl	 (%%rsp), %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '/';

		case REAL:
			/*stored type can only be less than or equal to acctype*/
			convertToReal(storedtype);
			/*4 bytes single precision division instruction*/
			fprintf(object, "\tmovss %%eax, %%xmm0\n");
			fprintf(object, "\tdivss %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovss %%xmm0, %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '/';

		case DOUBLE:
			/*stored type can only be less than or equal to acctype*/
			convertToDouble(storedtype);
			/*8 bytes double precision division instruction*/
			fprintf(object, "\tmovsd %%rax, %%xmm0\n");
			fprintf(object, "\tdivsd %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovsd %%xmm0, %%rax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '/';

		}
	break;
	}

	return 0;
}

int addinst(int inst, int storedtype, int acctype)
{	/*Check if it's a instruction of addition or subtraction
	if true then check type to translate
	else return 0*/
	/*Convert only happends if storedtype less than acctype*/
	switch(inst)
	{
	case '+': case OR:
		switch(acctype)
		{
		case BOOLEAN:
			/*1 byte addition instruction*/
			fprintf(object, "\torb %%al, (%%rsp)\n");
			fprintf(object, "\tmovb %%(%%rsp), %%al\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '+';

		case INTEGER:
			/*4 byte addition instruction*/
			fprintf(object, "\taddl %%eax, (%%rsp)\n");
			fprintf(object, "\tmovl %%(%%rsp), %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '+';
		case REAL:
			/*stored type can only be less than or equal to acctype*/
			convertToReal(storedtype);
			/*4 bytes single precision addition instruction*/
			fprintf(object, "\tmovss %%eax, %%xmm0\n");
			fprintf(object, "\taddss %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovss %%xmm0, %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '+';
		case DOUBLE:
			/*stored type can only be less than or equal to acctype*/
			convertToDouble(storedtype);
			/*8 bytes double precision addition instruction*/
			fprintf(object, "\tmovsd %%rax, %%xmm0\n");
			fprintf(object, "\taddsd %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovsd %%xmm0, %%rax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '+';
		}
		break;
	case '-':

		switch(acctype)
		{
		case INTEGER:
			/*4 byte subtraction instruction*/
		  	fprintf(object, "\tsubl %%eax, (%%rsp)\n");
		  	fprintf(object, "\tmovl %%(%%rsp), %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '-';
		case REAL:
			/*stored type can only be less than or equal to acctype*/
			convertToReal(storedtype);
			/*4 bytes single precision subtraction instruction*/
			fprintf(object, "\tmovss %%eax, %%xmm0\n");
			fprintf(object, "\tsubss %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovss %%xmm0, %%eax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '-';
		case DOUBLE:
			/*stored type can only be less than or equal to acctype*/
			convertToDouble(storedtype);
			/*8 bytes double precision subtraction instruction*/
			fprintf(object, "\tmovsd %%rax, %%xmm0\n");
			fprintf(object, "\tsubsd %%xmm1, %%xmm0\n");
			fprintf(object, "\tmovsd %%xmm0, %%rax\n");
			fprintf(object, "\taddq $8, %%rsp\n");
			return '-';

		}
		break;

	}
	return 0;
}


int lmove(const char *variable, int type)
{
	/*Show instruction mov to store accumulated value on left side variable*/
	switch(type)
	{
	case BOOLEAN:
		/*Translation of a left side assignment variable of type boolean*/
		fprintf(object, "\tmovb %%al,  %s\n", variable);
		return BOOLEAN;

	case INTEGER:
		/*Translation of a left side assignment variable of type int*/
   		fprintf(object, "\tmovl %%eax,  %s\n", variable);
		return INTEGER;

	case REAL:
		/*Translation of a left side assignment variable of type float*/
   		fprintf(object, "\tmovss %%eax,  %s\n", variable);
		return REAL;

	case DOUBLE:
		/*Translation of a left side assignment variable of type double*/
   		fprintf(object, "\tmovsd %%rax, %s\n", variable);
		return DOUBLE;
	}
	return 0;
}

int rmove(const char *variable_one, const char *variable_two, int type)
{ 	/*Show instruction mov and push to save accumulated register on
	 stack and move a value of a variable or constant to register*/
	switch(type)
	{
	case BOOLEAN:
		/*Translation of a right side assignment variable or constant of type boolean*/
		fprintf(object, "\tpush, %%rax\n");
		fprintf(object, "\tmovb %s, %%al\n", variable_one);
		return BOOLEAN;

	case INTEGER:
		/*Translation of a right side assignment variable or constant of type int*/
		fprintf(object, "\tpush, %%rax\n");
	   	fprintf(object, "\tmovl %s, %%eax\n", variable_one);
		return INTEGER;

	case REAL:
		/*Translation of a right side assignment variable or constant of type float*/
		fprintf(object, "\tpush, %%rax\n");
	   	fprintf(object, "\tmovss %s, %%eax\n", variable_one); 
		return REAL;

	case DOUBLE:
		/*Translation of a right side assignment variable or constant of type double*/
		fprintf(object, "\tpush, %%rax\n");
		/*if variable two different of NULL then it's a double constant else it's a variable*/
		if(variable_two)
		{
			fprintf(object, "\tmovl %s, %%eax\n", variable_two);
			fprintf(object, "\tshlq $32, %%rax\n");	
			fprintf(object, "\torl %s, %%eax\n", variable_one);
		} else
	 	  	fprintf(object, "\tmovsd %s, %%rax\n", variable_one);
		return DOUBLE;
	}
	return 0;
}


void relinst(int op, int storetype, int acctype)
{
	/*Check type for the cmp instruction*/
	switch(acctype)
	{
		case INTEGER:
			fprintf(object, "\tmovl (%%rsp), %%ebx\n");
			fprintf(object, "\tcmpl %%eax, %%ebx\n");
			break;
		case REAL:
			/*convert to compare*/
			convertToReal(storetype);
			fprintf(object, "\tcmps %%eax, %%xmm1\n");
			break;
		case DOUBLE:
			/*convert to compare*/
			convertToDouble(storetype);
			fprintf(object, "\tcmpq %%rax, %%xmm1\n");
			break;
	}

	/*Check the relation operator to choose what jump use*/
	fprintf(object, "\tmovl $1, %%eax\n");
	switch(op)
	{
		case EQ:
			fprintf(object, "\tjz L%d\n", labelcounter);
	       	break;

	    case NEQ:
	        fprintf(object, "\tjnz L%d\n", labelcounter);
	        break;

	    case GT:
	     	fprintf(object, "\tjg L%d\n", labelcounter);
	        break;

	    case GEQ:
	       	fprintf(object, "\tjge L%d\n", labelcounter);
	        break;
	  
	    case LT:
	      	fprintf(object, "\tjl L%d\n", labelcounter);
	        break;

	    case LEQ:
	      	fprintf(object, "\tjle L%d\n", labelcounter);
	        break;
	}
	/*If the relation operator choose to not jump then eax equal to false*/
	fprintf(object, "\tmovl $0, %%eax\n");
	fprintf(object, ".L%d:\n", labelcounter++);
	fprintf(object, "\taddq $8, %%rsp\n");
}