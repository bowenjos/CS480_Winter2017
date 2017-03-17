#ifndef _TOKEN_H
#define _TOKEN_H

#include<string>

using namespace std;

/*****
 * token.h
 * Author: Joshua Bowen
 * Purpose: this struct represents a basic token item.
 * This token item is created by the scanner and utilized by the Parser to verify a syntactically correct input file and gather information
*****/


typedef struct
{
	int tokenType;
	string tokenValue;
	int tokenLineNumber;
} Token;

#endif
