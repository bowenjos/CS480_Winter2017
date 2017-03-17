#ifndef _ENUM_H
#define _ENUM_H

/*****
 * Enum.h
 * Author: Joshua Bowen
 * Purpose: These enums represent assorted Token and Node types. They are used as a variety of checks throughout the Translator
*****/ 


enum 
{
	_EOF,
	ERROR,
	ELSE,
	IF,
	INT,
	RETURN,
	VOID,
	WHILE,
	PLUS, MINUS, MULT, DIV, //+ - * /
	LS, LEQ, GT, GEQ, // < <= > >=
	EQ, NEQ, // == !=
	ASSIGN,
	SEMI,
	COMMA,
	LPAREN, RPAREN,
	LBRACKET, RBRACKET, 
	LBRACE, RBRACE,
	READ, WRITE,
	NUMBER,
	ID,
	PROGRAM,
	DECLARATION,
	VARIABLE, 
	ARRAY, 
	FUNCTION,
	EXPRESSION,
	CALL,
	COMPOUND,
	TYPE_SPECIFIER,
	PARAMETER_LIST,
	PARAMETER,
	STATEMENT_LIST,
	STATEMENT,
	ARGUMENTS,
	END
};

#endif
