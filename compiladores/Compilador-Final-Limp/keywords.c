#include <string.h>
#include <keywords.h>

  /*Table of reserved words*/
 char *keywords[] = {
   "begin", "repeat", "until", "var","if", "then", "else", "while", "do", "procedure",
   "function", "not", "boolean","integer", "real", "double", "div", "mod", "and", "or","true", "false","end",
 };

 int iskeyword(char const *identifier) {
 	/*Check if character stream is a reserved word*/
    int i;
    for(i = BEGIN; i <= END; i++) {
        if (strcmp(keywords[i-BEGIN], identifier) == 0) {
            return i;
        }
    }

    return 0;
 }


