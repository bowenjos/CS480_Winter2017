#include "scanner.h"

/*****
 * scanner.cpp
 * Author: Joshua Bowen
 * Purpose: This file contains all the functions associated with the scanner class object
*****/


/*****
 * Function: Scanner (Constructor)
 * Purpose: This function is used to set up a scanner class object.
 * It's actions are to open the sourceFile and initiate some variables
*****/

Scanner::Scanner(char* myFile)
{
	this->sourceFile.open(myFile);
	ignorePeek = 0;
	lineNumber = 0;
}

/*****
 * Function: ~Scanner (Deconstructor)
 * Purpose: This function is used to dismantle a scanner class object.
 * It's action is to close the open sourceFile
*****/

Scanner::~Scanner()
{
	if(this->sourceFile.is_open()){
		this->sourceFile.close();
	}
}

/*****
 * Function: checkOverflow
 * Purpose: Due to the nature of some tokens to be connected without white space, 
 * it was neccessary to design a function that would split these tokens into pieces.
 * To accomplish this I would pull the first part of a token off of the other tokens it was stuck to
 * Once this was achieved I would put those overflow tokens into something so they could be referenced later
 * This function references those overflow tokens and makes sure they are not forgotten
*****/

Token Scanner::checkOverflow(char *token)
{
	char newToken[100];
	int i = 0;
	int j = 0;

	//I check the first part of the overflow to see if it's a single character token, some kind of paren, bracket, brace, comma, or semi-colon
	if((token[i] == '[') || (token[i] == ']') || (token[i] == '(') || (token[i] == ')') || (token[i] == '{') || (token[i] == '}') || (token[i] == ',') || (token[i] == ';')){
		//If the above statement is true, I pull the first part of the token off and store the rest in an overflowToken, assuming there is further overflow
		strncpy(newToken, token, 1);
		newToken[i+1] = '\0';
		overflow = 1;
		while(token[i+1] != '\0'){
			overflowToken[j] = token[i+1];
			i++;
			j++;
		}
		overflowToken[i] = '\0';
	//If the token is not a single character, I assume that it is either an ID or a NUMBER and do a process similair to above
	}else{
		//The main difference here is that I need a loop to get all the token items
		while((token[i] != '[') && (token[i] != ']') && (token[i] != '(') && (token[i] != ')') && (token[i] != '{') && (token[i] != '}') && (token[i] != ',') && (token[i] != ';')){
			i++;
		}
		strncpy(newToken, token, i);
		newToken[i] = '\0';

		overflow = 1;	
		while(token[i] != '\0'){
			overflowToken[j] = token[i];
			i++;
			j++;
		}
		overflowToken[j] = '\0';
	}

	return createNewToken(newToken);
}

/*****
 * Function: createNewToken
 * Purpose: This function does what it says on the box, it's very repetitive but depending on the string value of the tokenized string
 * it will assign the newly created token an ENUM value from enum.h
*****/

Token Scanner::createNewToken(char *token)
{
	Token newToken;
	int i = 0;

	//This huge block determines the string value of the tokenized string and then assigns the new token the appropriate ENUM value, it also attributes a line number for the token.
	if(!strcmp(token, "eof")){
			newToken.tokenType = _EOF; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "error")){
			newToken.tokenType = ERROR; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "else")){
			newToken.tokenType = ELSE; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "if")){
			newToken.tokenType = IF; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "int")){
			newToken.tokenType = INT; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "return")){
			newToken.tokenType = RETURN; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "void")){
			newToken.tokenType = VOID; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "while")){
			newToken.tokenType = WHILE; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "read")){
			newToken.tokenType = READ; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "write")){
			newToken.tokenType = WRITE; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "call")){
			newToken.tokenType = CALL; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "+")){
			newToken.tokenType = PLUS; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "-")){
			newToken.tokenType = MINUS; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "*")){
			newToken.tokenType = MULT; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "/")){
			newToken.tokenType = DIV; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "<")){
			newToken.tokenType = LS; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "<=")){
			newToken.tokenType = LEQ; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, ">")){
			newToken.tokenType = GT; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, ">=")){
			newToken.tokenType = GEQ; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "==")){
			newToken.tokenType = EQ; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "!=")){
			newToken.tokenType = NEQ; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "=")){
			newToken.tokenType = ASSIGN; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, ";")){
			newToken.tokenType = SEMI; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, ",")){
			newToken.tokenType = COMMA; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "(")){
			newToken.tokenType = LPAREN; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, ")")){
			newToken.tokenType = RPAREN; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 		  
		}else if(!strcmp(token, "[")){
			newToken.tokenType = LBRACKET; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "]")){
			newToken.tokenType = RBRACKET; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "{")){
			newToken.tokenType = LBRACE; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "}")){
			newToken.tokenType = RBRACE; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else{	
			//This part of the function is special because it means that the type of token is either an ID, a NUMBER, or a bunch of tokens stuck together
			i = 0;
			if((token[i] >= 'A' && token[i] <= 'Z') || (token[i] >= 'a' && token[i] <= 'z')){
				while(token[i] != '\0'){
					if((token[i] >= 'A' && token[i] <= 'Z') || (token[i] >= 'a' && token[i] <= 'z')){
						i++;
					}else{
						//if the token isn't a bunch of letters to form an ID then it COULD be something like if() where there are parens in the token that are important
						newToken = checkOverflow(token);
						return newToken;
					}
				}
				newToken.tokenType = ID;
				newToken.tokenValue = token;
				newToken.tokenLineNumber = lineNumber-1;
			//Here I check to see if it's a number
			}else if(token[i] >= '0' && token[i] <= '9'){
				while(token[i] != '\0'){
		   			if(token[i] >= '0' && token[i] <= '9'){
						i++;
					}else{
						//If the token isn't a bunch of numbers to form a NUMBER then it could be something like a[10]; where the 10 inside is followed by ] which matters
						newToken = checkOverflow(token);
						return newToken;
					}
				}
				newToken.tokenType = NUMBER;
				newToken.tokenValue = token;
				newToken.tokenLineNumber = lineNumber-1;
			//If it doesn't start with a letter or a number then it could be a leftover token that starts with a paren or something to that affect
			}else{
				newToken = checkOverflow(token);
				return newToken;
			}
		}

	return newToken;
}

/*****
 * Function: getToken
 * Purpose: This function gets lines from the sourceFile and tokenizes them, it will run either of the above two functions
 * to assure that the token is made into something.
*****/

Token Scanner::getToken()
{
	string line;
	char *token;
	Token newToken;
	int i;

	//If you ran a peek function to look at a token but not get it, then you first have to make sure you get that token.
	if(peeked == 1 && ignorePeek != 1){
		peeked = 0;
		newToken = peekToken;
	//If you peeked further than single peak then you have to make sure that that token is attended to.
	}else if(peekedFurther == 1 && ignorePeek != 1){
		peekedFurther = 0;
		newToken = peekFurtherToken;
	//If the last token you retrieved had an overflow then that overflow must be attended to
	}else if(overflow == 1){
		overflow = 0;
		newToken = createNewToken(overflowToken);
	//If all of the above three checks pass (or fail would be a better word) then the function will finally get a new token
	}else{
		token = strtok(NULL, " \t\r\a\n");
		//If the new tokenized string is NULL then the line must be ended, so the function will loop until a line that is not null (or end of file) is found
		while(token == NULL){
			//If you reach the end of the file, a specal END token is generated
			//This special token is used in the parser to end the parsing process
			if(sourceFile.eof()){
				newToken.tokenType = END;
				return newToken;
			}
			getline(sourceFile, line);
			lineNumber++;
			strcpy(cline, line.c_str());
			token = strtok(cline, " \t\r\a\n");
		}	
		newToken = createNewToken(token);
	}

	return newToken;
}

/*****
 * Function: peek
 * Purpose: This function is used in the circumstance that you need to know what the next token
 * is to decide how to parse, but you DON'T want to process that token just yet
*****/

Token Scanner::peek()
{
	peekToken = getToken();
	peeked = 1;
	
	return peekToken;
}

/*****
 * Function: peekFurther
 * Purpose: When peeking forward once just isn't enough
 * I use this pretty much exclusively in the global variable decleration and function declaration as I need the type specifier for the node.
 * In this case both nodeTypes begin with a type specifier and are followed by an ID so a singular peek isn't sufficient
*****/

Token Scanner::peekFurther()
{
	ignorePeek = 1;
	peekFurtherToken = getToken();
	peekedFurther = 1;
	ignorePeek = 0;
	
	return peekFurtherToken;
}

/*****
 * Function: checkSourceFile
 * Purpose: This function checks to see if the source file has reached End of File.
*****/

int Scanner::checkSourceFile()
{
	if(!sourceFile.eof()){
		return 1;
	}else{
		cout << "End of File" << endl;
		return 0;
	}
}
