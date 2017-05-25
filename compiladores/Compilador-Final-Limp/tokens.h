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
	NO_FILE,
	INV_EXT
};
