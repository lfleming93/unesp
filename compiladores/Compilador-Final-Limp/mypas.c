/**@<progcalc.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>
#include <tokens.h>
#include <mypas.h>

FILE *source;

extern int lookahead; // @ parser.c

void isvalid(int argc, const char *entry, const char *out)
{
    char *objfile = NULL;

    /*Check if there is a input*/
    if(argc == 1)
    {
        fprintf(stderr, "mypas: fatal error: no input files\n");
        fprintf(stderr, "parser terminated.\n");
        exit(NO_FILE);
    }    

    /*Check source file extension*/
    if(!(objfile = strrchr(entry, '.')))
    {
        fprintf(stderr, "file %s without extencion\n", entry);
        exit(NO_FILE);
    }

    /*Check extension*/
    if(strcmp(objfile, ".pas"))
    {
        fprintf(stderr, "Error: invalid file - expected .pas\n");
        exit(INV_EXT);
    }
}

int main (int argc, char *argv[], char *envp[])
{
    isvalid(argc, argv[1], argv[2]);

    if (!(source = fopen (argv[1], "r")))
    {
        fprintf (stderr, "%s: cannot open %s... exiting\n", argv[0],argv[0] );
        exit (-1);
    }

    mypas();

    fclose(source);
    exit(0);
}
