/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <tokens.h>
#include <lexer.h>
#include <string.h>
#include <mypas.h>
#include <keywords.h>
#include <parser.h>


/*************************** grammar definition ************************************
 *
 * mypas -> body '.'
 *
 * body -> declarative imperative
 *
 * declarative -> [ VAR namelist ':' vartype ';' { namelist ':' vartype ';' } ]
 * 		  { sbpmod sbpname parmdef [ ':' fnctype ] ';' body ';' }
 *
 * imperative -> BEGIN stmtlist END
 *
 * sbpmod -> PROCEDURE | FUNCTION
 *
 * vartype -> INTEGER | REAL | DOUBLE | BOOLEAN
 *
 * fnctype -> INTEGER | REAL | DOUBLE | BOOLEAN
 *
 * sbpname -> ID
 *
 * parmdef -> [ ( [VAR] namelist ':' vartype { ';' [VAR] namelist ':' vartype } ) ]
 *
 * namelist -> ID { , ID }
 *
 * stmtlist -> stat { ';' stat }
 *
 * stat -> imperative
 * 	  | ifstmt
 *    | whilstmt
 *    | dostmt
 *    | smpexpr
 *    | <empty>
 *
 * ifstmt -> IF expr THEN stat [ ELSE stat ]
 *
 * whilestmt -> WHILE expr DO stat
 *
 * dostmt -> REPEAT stmtlist UNTIL expr
 *
 * smpexpr -> [-] fact {mulop fact} {addop fact {mulop fact}}
 *
 * fact ->  vrbl | cons | ( expr ) | TRUE | FALSE
 *
 * vrbl -> ID [ := expr]
 *
 * addop -> '+' | '-' | OR
 *
 * mulop -> '*' | '/' | AND
 *
 * cons -> INT | FLT | HEX | OCT | DBL | TRUE | FALSE
 *
 **************************************************************************************************/

/* mypas -> body '.' */
void mypas(void)
{   /*Initialize lookahead*/
    lookahead = gettoken (source);
    /*Program start*/
    body();
    match('.');
}

/* body -> declarative imperative */
void body(void)
{
    declarative();
    imperative();
}

/*
 * declarative -> [
 * 			VAR namelist ':' vartype ';' ||
 * 			   { namelist ':' vartype ';' }
 * 		  ]
 * 		  { sbpmod sbpname parmdef [ ':' fnctype ] ';' body ';' }
 */
void declarative (void)
{
 
    if(lookahead == VAR)
    {
        match(VAR);
        do
        {
            namelist();
            match(':');
	       
            vartype();
            match(';');
        }
        while (lookahead == ID);
    }
    while (lookahead == PROCEDURE || lookahead == FUNCTION)
    {
        match(lookahead);
        match(ID);
        parmdef();
        if(lookahead == ':')
        {
            match(':');
            fnctype();
        }
        match(';');
        body();
        match(';');
    }
}

/* vartype -> INTEGER | REAL | DOUBLE | BOOLEAN */
int vartype(void)
{
    switch(lookahead)
    {
    case INTEGER:
        match(INTEGER);
        return INTEGER;
    case REAL:
        match(REAL);
        return REAL;
    case DOUBLE:
        match(DOUBLE);
        return DOUBLE;
    default:
        match(BOOLEAN);
        return BOOLEAN;
    }
}

/* fnctype -> INTEGER | REAL | DOUBLE | BOOLEAN  */
int fnctype(void)
{
    switch(lookahead)
    {
    case INTEGER:
        match(INTEGER);
        return INTEGER;
    case REAL:
        match(REAL);
        return REAL;
    case DOUBLE:
        match(DOUBLE);
        return DOUBLE;
    default:
        match(BOOLEAN);
        return BOOLEAN;
    }
}

/* namelist -> ID { , ID } */
void namelist()
{

NEXT_name:
    match(ID);
    if(lookahead == ',')
    {
        match(',');
        goto NEXT_name;
    }
}

/* imperative -> BEGIN stmtlist END */
void imperative(void)
{
    match(BEGIN);
    stmtlist();
    match(END);
}

/* parmdef -> [ ( [VAR] namelist ':' vartype { ';' [VAR] namelist ':' vartype } ) ] */
void parmdef(void)
{
    if(lookahead=='(')
    {
        match('(');

par_begin:
        if(lookahead==VAR) match(VAR);
        namelist();
        match(':');
        vartype();
        if(lookahead == ';')
        {
            match(';');
            goto par_begin;
        }
        match(')');
    }
}

/* stmtlist -> stat { ';' stat } */
void stmtlist(void)
{
stmt_begin:
    stat();
    if(lookahead == ';')
    {
        match(';');
        goto stmt_begin;
    }
}

/* stat -> imperative
* 	      | ifstmt
*         | whilestmt
*         | dostmt
*         | smpexpr
*         | <empty>
*/
void stat(void)
{

    switch(lookahead)
    {
    case BEGIN:
        imperative();
        break;
    case IF:
        ifstmt();
        break;
    case WHILE:
        whilestmt();
        break;
    case REPEAT:
        dostmt();
        break;
    case ID: /*hereafter we got FIRST(smpexpr):*/
    case INT:
    case FLT:
    case DBL:
    case '(':
    case '-':
    case NOT:
    case TRUE:
    case FALSE:
        smpexpr();
        break;
    default:
        ;
    }
}

/* ifstmt -> IF expr THEN stat [ ELSE stat ] */
void ifstmt(void)
{
    match(IF);
    expr();
    match(THEN);
    stat();
    if(lookahead == ELSE){
		match(ELSE);
        stat();
    }
}

/* whilestmt -> WHILE expr DO stat */
void whilestmt(void)
{
    match(WHILE);

    expr();

    match(DO);

    stat();
}

/* dostmt -> REPEAT stmtlist UNTIL expr */
void dostmt(void)
{
    match(REPEAT);

    stmtlist();

    match(UNTIL);

    expr();
}


/* relop = EQ | NEQ | GT | GEQ | LT | LEQ */
int relop(void)
{
    switch(lookahead)
    {
    case EQ:
        match(EQ);
        return EQ;

    case NEQ:
        match(NEQ);
        return NEQ;

    case GT:
        match(GT);
        return GT;

    case GEQ:
        match(GEQ);
        return GEQ;

    case LT:
        match(LT);
        return LT;

    case LEQ:
        match(LEQ);
        return LEQ;
    }

    return 0;
}

/*expr -> expr [opre smpexpr]*/
void expr()
{
    smpexpr();

    if(relop())
    {
       smpexpr();
    }
}

/* smpexpr -> [-] fact {mulop fact} {addop fact {mulop fact}}
	fact ->  vrbl | cons | ( expr ) | TRUE | FALSE
    vrbl -> ID [ := expr] */

void smpexpr (void)
{

    if (lookahead =='-')
    {
        match('-');
    }
    else if (lookahead == NOT)
    {
        match(NOT);
    }

T_entry:
F_entry:

    switch (lookahead)
    {
    case ID:

        match (ID);

        if(lookahead == ASG)
        {
            match(ASG);
            expr();
        }
        break;

    case INT:
        match (INT);
        break;

    case FLT:
        match(FLT);
        break;

    case DBL:
        match(DBL);
        break;

    case TRUE:
        match(TRUE);
        break;

    case FALSE:
        match(FALSE);
        break;

    default:
        match ('(');
        expr();
        match(')');
    }


    if (mulop()) 
        goto F_entry;

    if (addop())
        goto T_entry;
}

/*addop -> '+' | '-' | OR*/
int addop (void)
{
    switch(lookahead)
    {
    case '+':
        match('+');
        return '+';
    case '-':
        match('-');
        return '-';

    case OR:
        match(OR);
        return OR;
    }
    return 0;
}

/*mulop -> '*' | '/' | AND*/
int mulop (void)
{
    switch(lookahead)
    {
    case '*':
        match('*');
        return '*';

    case '/':
        match('/');
        return '/';
    case AND:
        match(AND);
        return AND;
    }
    return 0;
}

/***************************** lexer-to-parser interface **************************/

int lookahead; // @ local

void match (int expected)
{

    if ( expected == lookahead)
    {
        lookahead = gettoken (source);
    }
    else
    {
        fprintf(stderr,"parser: token mismatch error. found # %d ",
                lookahead);
        fprintf(stderr,"whereas expected # %d\n",
                expected);
        exit (SYNTAX_ERR);
    }
}

