/**@<progcalc.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>
#include <tokens.h>
#include <pseudoassembly.h>
#include <mypas.h>

char *objfile = NULL;
FILE *source, *object;
extern int lookahead; // @ parser.c

char *isvalid(int argc, const char *entry, const char *out)
{

    /*Check if there is a input*/
    if(argc == 1)
    {
        fprintf(stderr, "mypas: fatal error: no input files\n");
        fprintf(stderr, "compilation terminated.\n");
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

    /*Check if there is a name defined*/
    if(argc == 3)
    {
        if(!(objfile = strrchr(out, '.')))
        {
            fprintf(stderr, "file %s without extencion\n", out);
            exit(NO_FILE);
        }

        if(strcmp(objfile, ".s"))
        {
            fprintf(stderr, "Error: invalid %s file expected .s\n", objfile);
            exit(INV_EXT);
        }

        objfile = malloc(sizeof(out));
        strcpy(objfile, out);

    } else {
        objfile = malloc(sizeof(char)*3);
        strcpy(objfile, "a.s");
    }

    return objfile;
}

int main (int argc, char *argv[], char *envp[])
{
    char *objfile;

    /*Check if there is a argument valid*/
    objfile = isvalid(argc, argv[1], argv[2]);

    /*Open input file*/
   if (!(source = fopen (argv[1], "r")))
    {
        fprintf (stderr, "%s: cannot open %s... exiting\n", argv[0],argv[0] );
        exit (NO_FILE);
    }

    /*Open output file*/
    if(!(object = fopen(objfile, "w+")))
   {   
        fprintf(stderr, "cannot open object file... exiting\n");
        return NO_FILE; 
   }

    /*[[*/ fprintf(object, "\t.file   %s\n", argv[1]); /*]]*/
    mypas();

    fclose(object);
    fclose(source);

    if(errcounter)
    {
        printf("WARNING: Total of %d errors found\n", errcounter);
        if(remove(objfile))
            fprintf(stderr, "Unable to remove %s file\n", objfile);
    } else
    {
        printf("Successful compilation\n");
    }
    free(objfile);
    return 0;
}
