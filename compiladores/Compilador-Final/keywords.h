enum {
   BEGIN = 0x10001, REPEAT, UNTIL, VAR, IF, THEN, ELSE, WHILE, DO, PROCEDURE, FUNCTION, NOT, BOOLEAN, INTEGER, REAL, DOUBLE, DIV, MOD, AND, OR, TRUE, FALSE,END,
 };

 /*Check if identifier is a reserved word*/
 int iskeyword(char const *identifier);
 extern char *keywords[];
