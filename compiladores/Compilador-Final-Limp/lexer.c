/**@<lexer.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <keywords.h>
#include <lexer.h>

#define MAXID_SIZE 32

char lexeme[MAXID_SIZE + 1];

int floatOrDouble()
{
    /*Check double and float range*/
    double value = atof(lexeme);
    int maxrange, minrange;

    maxrange = (value <= 3.4E+38 && value >= 1.17E-45);
    minrange = (value <=  -3.4E+38 && value >= -1.17E-45);
    if( maxrange || minrange)
        return FLT;
    return DBL;
}

void skipspaces(FILE *tape)
{
	
	  while ( isspace ( lexeme[0] = getc (tape) ) );

        ungetc ( lexeme[0], tape );
}

int is_identifier(FILE *tape)
{
    /*Comsome os caracteres da fita contanto que seja uma letra ou numero*/
    int i = 0;

    lexeme[i] = getc(tape);
    if(isalpha(lexeme[i]))
    {
        for(i++; isalnum(lexeme[i] = getc(tape)); i++)
            if(i == MAXID_SIZE)
                i--;
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
	if ((i = iskeyword(lexeme))) return i;
        return ID;
    }
    ungetc(lexeme[i], tape);
    return 0;
}

int is_exponencial(FILE *tape, int i)
{
    /* Comsome a fita para o padrão (E|e)(epsolon|+|-)(0|1|...|9)^+ */
    lexeme[i] = getc(tape);

    if(lexeme[i] == 'E' || lexeme[i] == 'e')
    {
        i++;
        if((lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-')
        {
            i++;
            if(isdigit(lexeme[i] = getc(tape)))
            {
                for(i++; isdigit(lexeme[i] = getc(tape)); i++)
                    if(i == MAXID_SIZE)
                        i--;
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return floatOrDouble();
            }
            ungetc(lexeme[i], tape);
            i--;
        }
        else if(isdigit(lexeme[i]))  //Caso seja E(0|1|...|9)^+
        {
            for(i++; isdigit(lexeme[i] = getc(tape)); i++)
                if(i == MAXID_SIZE)
                    i--;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return floatOrDouble();
        }
        ungetc(lexeme[i], tape);
        i--;
    }
    ungetc(lexeme[i], tape);
    return 0;
}

int is_fractional(FILE *tape, int i)
{
    /*Comsome os caracteres caso seja um ponto seguido de ao menos um digito*/
    lexeme[i] = getc(tape);

    if(i == 0)  //Caso o i seja zero, não existe decimal anterior em lexene, então segue .[0-9]^+
    {
        if(lexeme[i] == '.')
        {
            i++;
            //Checa se existe um numero depois do ponto, após consumir todos digitos, caso encontre o caracter 'E' ao fim chama a função is_exponencial
            if(isdigit(lexeme[i] = getc(tape)))
            {
                for (i++; isdigit(lexeme[i] = getc(tape)) ; i++)
                    if(i == MAXID_SIZE)
                        i--;
                if(lexeme[i] == 'E' || lexeme[i] == 'e')  //Checa se existe exponencial podendo ser .[0-9]^+exp
                {
                    ungetc(lexeme[i], tape);
                    return is_exponencial(tape, i);
                }
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return floatOrDouble();
            }
            ungetc(lexeme[i], tape);
            i--;
        }
    }
    else if(lexeme[i] == '.')   // Quando i for diferente segue uint.[0-9]*
    {
        for (i++; isdigit(lexeme[i] = getc(tape)); i++)
            if(i == MAXID_SIZE)
                i--;
        if(lexeme[i] == 'E' || lexeme[i] == 'e')  //Checa se existe exponencial podendo ser .[0-9]*exp
        {
            ungetc(lexeme[i], tape);
            return is_exponencial(tape, i);
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return floatOrDouble();
    }

    ungetc(lexeme[i], tape);
    return 0;
}

int is_number(FILE *tape)
{
    /*Consome os caracteres decimais da fita case seja um decimal, decimal fracionario com ou sem exponencial*/
    int i = 0;

    lexeme[i] = getc(tape);

    //Checa se o caracter lido é um digito
    if (isdigit(lexeme[i]))
    {

        if (lexeme[i] == '0')
        {
            i++;
            //Caso o segundo caracter depois do zero seja um ponto, chama a função fractional uint.[0-9]*
            if((lexeme[i] = getc(tape)) == '.')
            {
                ungetc(lexeme[i], tape);
                return  is_fractional(tape, i);
            }
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return INT;
        }

        // [0-9]*
        for(i++; isdigit(lexeme[i] = getc(tape)); i++)
            if(i == MAXID_SIZE)
                i--;

        // Caso o caracter lido depois dos digitos seja um ponto, chama a função fractional
        if(lexeme[i] == '.')
        {
            ungetc(lexeme[i], tape);
            return  is_fractional(tape, i);
        }
        // Caso o caracter lido seja E ou e, chama a função que trata a parte exponencial
        if(lexeme[i] == 'E' || lexeme[i] == 'e')
        {
            ungetc(lexeme[i], tape);
            return is_exponencial(tape, i);
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return INT;
    }

    ungetc(lexeme[i], tape);

    return is_fractional(tape, 0);
}

int is_symbol(FILE *tape)
{
    /*Checa se a sequência de caracteres é um simbolo especifico*/
    int i = 0;

    lexeme[i] = getc(tape);

    switch(lexeme[i])
    {
    case ':':
        i++;
        if((lexeme[i] = getc(tape)) == '=')
        {
            i++;
            lexeme[i] = 0;
            return ASG;
        }
        ungetc(lexeme[i], tape);
        i--;
        ungetc(lexeme[i], tape);
        return 0;

    case '<':
	i++;
        if((lexeme[i] = getc(tape)) == '=')
        {
            i++;
            lexeme[i] = 0;
            return LEQ;
        }else if(lexeme[i] == '>')
        {
            i++;
            lexeme[i] = 0;
            return NEQ;
        }
        ungetc(lexeme[i], tape);
	lexeme[i] = 0;
        return LT;

    case '>':
	i++;
        if((lexeme[i] = getc(tape)) == '=')
        {
            i++;
            lexeme[i] = 0;
            return GEQ;
        }
        ungetc(lexeme[i], tape);
	lexeme[i] = 0;
        return GT;

    case '=':
      i++;
      lexeme[i] = 0;

      return EQ;

    }

    ungetc(lexeme[i], tape);
    return 0;
}


int gettoken (FILE *tokenstream)
{
    int token;

    skipspaces (tokenstream);
    if ( (token = is_identifier(tokenstream)) )
    {
        return token;
    }

    if ( (token = is_number(tokenstream)) )
    {
        return token;
    }

    if( (token = is_symbol(tokenstream)) )
    {
        return token;
    }

    lexeme[1] = 0;
    lexeme[0] = token =  getc (tokenstream);


    return token;
}

