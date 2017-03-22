#ifndef _SCANNER_H
#define _SCANNER_H

#include "token.h"
#include "enum.h"

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<time.h>
#include<unistd.h>

using namespace std;

/*****
 * Scanner.h
 * Author: Joshua Bowen
 * Purpose: This scanner class function is to read in a source file and tokenize it
*****/

class Scanner
{
	public:
		Scanner(char*);
		~Scanner();
		
		Token checkOverflow(char *token);
		Token createNewToken(char *token);
		Token getToken();
		Token peek();
		Token peekFurther();
		int checkSourceFile();	
	private:
		ifstream sourceFile;
		char cline[100];
		int lineNumber;
		int overflow;
		int peeked;
		int peekedFurther;
		int ignorePeek;
		Token peekToken;
		Token peekFurtherToken;
		char overflowToken[100];
};

#endif
