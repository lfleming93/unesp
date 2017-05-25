/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <tokens.h>
#include <lexer.h>
#include <symtab.h>
#include <string.h>
#include <mypas.h>
#include <macros.h>
#include <keywords.h>
#include <pseudoassembly.h>
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
    /*[[*/prolog();/*]]*/
    imperative();
    /*[[*/epilog();/*]]*/
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
    /*
     * vardef -> VAR namelist ':' vartype ';' || vardef.symtab <- forall symbol in namelist.name do
     * 									symtab_append(symbol, vartype.type
     * 								end do
     *
     */
    if(lookahead == VAR)
    {
        match(VAR);
        do
        {

            /*[[*/ int type, i /*]]*/;
            /*Receive variable list and stores on namev*/
            /*[[*/char **namev = /*]]*/ namelist();
            match(':');
            /*[[*/ type = /*]]*/ vartype();
            /*After varlist type get append on syntab*/
            /*[[*/
            for(i = 0; namev[i]; i++)
            {
                symtab_append(namev[i], type); /*]]*/
                free(namev[i]);
            }
            free(namev);
            match(';');
        }
        while (lookahead == ID);
        /*Semantic Assembly Translation*/
        symtab_assembly();
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
char **namelist()
{
	/*Create the temp variable table with a list of variable name*/
    /*[[*/ char **symvec = calloc(MAX_ARG_NUM, sizeof(char **));
    int i = 0; /*]]*/

NEXT_name:
    /*[[*/
    symvec[i]=malloc(sizeof(lexeme) +1);
    strcpy(symvec[i], lexeme);
    i++ /*]]*/;
    match(ID);
    if(lookahead == ',')
    {
        match(',');
        goto NEXT_name;
    }
    return symvec;
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
        smpexpr(0);
        break;
    default:
        ;
    }
}

/* ifstmt -> IF expr THEN stat [ ELSE stat ] */
void ifstmt(void)
{
    int _endif, _else, syntype;

    match(IF);
    syntype = expr(BOOLEAN);

    if(syntype != BOOLEAN)
    {
    	fprintf(stderr, "incompatible if expr type - expected boolean type\n");
     	errcounter++;
    }

    /**/_endif = _else = gofalse(labelcounter++);/**/

    match(THEN);

    stat();
    if(lookahead == ELSE)
    {

	/**/_endif = jump(labelcounter++);/**/
	/**/mkLabel(_else);/**/

		match(ELSE);
        stat();
    }

    mkLabel(_endif);
}

/* whilestmt -> WHILE expr DO stat */
void whilestmt(void)
{
    int _while = labelcounter++, _endwhile, syntype;

    match(WHILE);

   /**/ mkLabel(_while); /**/

    syntype = expr(BOOLEAN);

    if(syntype != BOOLEAN)
    {
    	fprintf(stderr, "incompatible while expr type - expected boolean type\n");
     	errcounter++;
    }


   /**/ _endwhile = gofalse(labelcounter++); /**/

    match(DO);
    stat();

    /**/ jump(_while); /**/
    /**/ mkLabel(_endwhile); /**/
}

/* dostmt -> REPEAT stmtlist UNTIL expr */
void dostmt(void)
{
    /**/ int _repeat = labelcounter++, syntype; /**/
    match(REPEAT);

   /**/ mkLabel(_repeat); /**/
    stmtlist();
    match(UNTIL);

    syntype = expr(BOOLEAN);

    if(syntype != BOOLEAN)
    {
    	fprintf(stderr, "incompatible repeat expr type - expected boolean type\n");
     	errcounter++;
    }
   /**/ gofalse(_repeat); /**/
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
int expr(int inherited_type)
{
    int type_one, type_two, op_temp;

    type_one = smpexpr(0);

    if(op_temp = relop())
    {
        type_two = smpexpr(type_one);

        /*Relacional smpexpr so type_one and type_two should always have equal type and never boolean type*/
        if(type_one == BOOLEAN || type_two == BOOLEAN)
        {
            fprintf(stderr, "incompatible relational smpexpr type - boolean relop: fatal error.\n");
			errcounter++;
            return max(type_one, type_two); //cause error
        }

        /*Check if err was returned*/
        if(type_one == ERR_FATAL || type_two == ERR_FATAL)
            return ERR_FATAL;

        /*if type two (register)is  less than type one (stack) use use convert instruction on register*/
        convertRegtoStack(type_one, type_two);
        /*Relop to pseudo-assembly*/
        relinst(op_temp, type_one, max(type_one, type_two));
        /*Return BOOLEAN relational is true*/
        return BOOLEAN;
    }

    /*Propagate fatal error*/
    if(!iscompatible(inherited_type, type_one))
        return ERR_FATAL;
    else
        return type_one;
}

/* smpexpr -> [-] fact {mulop fact} {addop fact {mulop fact}}
	fact ->  vrbl | cons | ( expr ) | TRUE | FALSE
    vrbl -> ID [ := expr] */

int smpexpr (int inherited_type)
{
    int varlocality, //Syntab position of the index of the variable name
     lvalue_seen = 0,//Difference between a left variable of the assignment operation
     acctype = inherited_type, //Accumulated type
     ltype, rtype, //Used to compare left side of the assignment of the rtype
     syntype = 0; // Used to return error
    int opstack[MAX_STACK], nextOp = -1, tempOp;//operator stack - used to control precedence
    int typestack[MAX_STACK], nextType = -1; // type stack - used to control operand type
    int negseen = 0;

    /*[[*/
    if (lookahead =='-')
    {
        match('-');
        if(acctype == BOOLEAN)
        {
            fprintf(stderr, "incompatible unary operator: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        else if (acctype == 0)
        {
            acctype = INTEGER;
        }
        negseen = 1;
    }
    else if (lookahead == NOT)
    {
        match(NOT);
        if(acctype > BOOLEAN)
        {
            fprintf(stderr, "incompatible unary operator: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        else if (acctype == 0)
        {
            acctype = BOOLEAN;
        }
        negseen = 1;
    }
    /*]]*/

T_entry:
F_entry:

    switch (lookahead)
    {
    case ID:
        /* symbol must be declared */

        varlocality = symtab_lookup(lexeme);

        if(varlocality < 0)
        {
            fprintf(stderr, "parser %s not declared: fatal error\n", lexeme);
            errcounter++;
            syntype = ERR_FATAL;
        }

        match (ID);

        if(lookahead == ASG)
        {
            /*located variable is lvalue*/
            /*[[*/ lvalue_seen = 1; /*]]*/
            ltype = symtab[varlocality][1];

            match(ASG);

            rtype = expr(ltype);

            if(iscompatibleVariable(ltype, rtype))
            {
                acctype = max(ltype, acctype);
            }
            else
            {
                fprintf(stderr, "incompatible type - ltype variable can't promote to rtype: fatal error.\n");
                errcounter++;
                syntype = ERR_FATAL;
            }
            /*[[*/
        }
        else if(varlocality > -1)
        {
            if(iscompatible(symtab[varlocality][1], acctype))
            {
               acctype = max(symtab[varlocality][1], acctype);
            }else
            {
                fprintf(stderr, "incompatible variable type: fatal error\n");
                errcounter++;
                syntype = ERR_FATAL;
            }
            /*Semantic translation of a right side variable*/
            rmove(symtab_stream + symtab[varlocality][0], NULL, acctype);
            if(nextType > -1)
                convertRegtoStack(typestack[nextType], symtab[varlocality][1]);
        }/*]]*/

        break;

    case INT:

        immediateinst(max(INTEGER, acctype));

        match (INT);

        if(iscompatible(INTEGER, acctype))
            acctype = max(INTEGER, acctype);
        else
        {
            fprintf(stderr, "incompatible type - acctype is boolean - found integer type: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        /*[[*/
        break;

    case FLT:

	   immediateinst(max(REAL, acctype));

        match(FLT);

        if(iscompatible(REAL, acctype))
            acctype = max(REAL, acctype);
        else
        {
            fprintf(stderr, "incompatible type - acctype is boolean - found real type: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        break;

    case DBL:

		 immediateinst(max(DOUBLE, acctype));

        match(DBL);

        if(iscompatible(DOUBLE, acctype))
            acctype = DOUBLE;
        else
        {
            fprintf(stderr, "incompatible type - acctype is boolean - found double type: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        break;

    case TRUE:

        match(TRUE);

        if(acctype != BOOLEAN && acctype != 0)
        {
            fprintf(stderr, "incompatible type - acctype is not a boolean: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        else acctype = BOOLEAN;

        rmove("$1", NULL, BOOLEAN);
        break;

    case FALSE:

        match(FALSE);

        if(acctype != BOOLEAN && acctype != 0)
        {
            fprintf(stderr, "incompatible type - acctype is not a boolean: fatal error.\n");
            errcounter++;
            syntype = ERR_FATAL;
        }
        else acctype = BOOLEAN;
        rmove("$0", NULL, BOOLEAN);
        break;

    default:
        match ('(');

        ltype = expr(0);

        if(iscompatible (ltype, acctype))
        {
            /*convert accumulated register value to ltype if necessary*/
            convertRegtoStack(acctype, ltype);
            acctype = max(acctype, ltype);
        }
        else
        {
            fprintf(stderr, "parenthesized type incompatible with accumulated\n");
            errcounter++;
            syntype = ERR_FATAL;
        }

        match(')');
    }

    /*Store the actual acctype to later compare with previous type*/
    nextType++;
    typestack[nextType] = acctype;

    /*smpexpression ends down here*/
    if(lvalue_seen && varlocality > -1)
    {
        /*convert accumulated register value to ltype if necessary*/
        convertRegtoStack(ltype, rtype);
        /*assignment instruction to assemblt*/
        lmove((symtab_stream + symtab[varlocality][0]), ltype);
    }

    if(negseen)
    {
        negseen = 0;
        neginst(acctype);

    }

    /*If there is a operator check if itens a mulInst and make the correspondent assembly - SEMANTIC*/
    if(  > -1)
    {
    	if(mulinst(opstack[nextOp], typestack[nextType-1], acctype))
    	{
    		nextOp--;
    		nextType--;
    		/*Store the resultant type on stack*/
    		typestack[nextType] = acctype;
    	}
    }

    if (tempOp = mulop(acctype))
    {	/*[[*/
        if(tempOp == ERR_FATAL)
        {
            syntype = ERR_FATAL;
        }
        else
        {
            nextOp++;
            opstack[nextOp] = tempOp;
        }
         goto F_entry;/*]]*/
    }else



    /*If there is a operator check if itens a addInst and make the correspondent assembly - SEMANTIC*/
    if(nextOp > -1)
    {
    	if(addinst(opstack[nextOp], typestack[nextType-1], acctype))
    	{
    		nextOp--;
    		nextType--;
    		/*Store the resultant type on stack*/
    		typestack[nextType] = acctype;
    	}
    }

    if (tempOp = addop(acctype))
    { /*[[*/
        if(tempOp == ERR_FATAL)
        {
            syntype = ERR_FATAL;
        }
        else
        {
            nextOp++;
            opstack[nextOp] = tempOp;
        }
    	goto T_entry;/*]]*/
    }

    /*Check if a error occurred*/
    if(syntype == ERR_FATAL)
        return ERR_FATAL;
    else
        return acctype;
}

/*addop -> '+' | '-' | OR*/
int addop (int type)
{
    switch(lookahead)
    {
    case '+':
        match('+');
        if(type == BOOLEAN)
        {
            fprintf(stderr, "incompatible operator - expected not boolean type: fatal error.\n");
            errcounter++;
            return ERR_FATAL;
        }
        return '+';
    case '-':
        match('-');
        if(type == BOOLEAN)
        {
            fprintf(stderr, "incompatible operator - expected not boolean type: fatal error.\n");
            errcounter++;
            return ERR_FATAL;
        }

        return '-';

    case OR:
        match(OR);
        if(type != BOOLEAN)
        {
            fprintf(stderr, "incompatible operator - expect boolean type: fatal error.\n");
            errcounter++;
            return ERR_FATAL;
        }
        return OR;
        break;
    }
    return 0;
}

/*mulop -> '*' | '/' | AND*/
int mulop (int type)
{
    switch(lookahead)
    {
    case '*':
        match('*');
        if(type == BOOLEAN)
        {
            fprintf(stderr, "incompatible operator - expected not boolean type: fatal error.\n");
            errcounter++;
            return ERR_FATAL;
        }
        return '*';
    case '/':
        match('/');
        if(type == BOOLEAN)
        {
            fprintf(stderr, "incompatible operator expect not boolean type: fatal error.\n");
            errcounter++;
            return ERR_FATAL;
        }
        return '/';
    case AND:
        match(AND);
        if(type != BOOLEAN)
        {
            fprintf(stderr, "incompatible operator - expect boolean type: fatal error.\n");
            errcounter++;
            return ERR_FATAL;
        }
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
        fclose(source);
        fclose(object);
        remove(objfile);
        exit (SYNTAX_ERR);
    }
}

