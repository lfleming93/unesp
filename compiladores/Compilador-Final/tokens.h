/**@<tokens.h>::**/

/*Lexer tokens*/
enum {
	ID = 1025,
	INT,
	FLT,
	DBL,
	ASG,
	GEQ,
	LEQ,
	NEQ,
	LT,
	GT,
	EQ,
};

/*Error tokens*/
enum {
	SYNTAX_ERR = -64,
	SYNTAB_OVERFLOW,
	ALR_DEF, //Name already used
	ERR_FATAL, //fatal error
	INV_EXT, //invalid extension
	NO_FILE,
};
