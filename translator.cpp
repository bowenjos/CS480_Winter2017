#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

using namespace std;

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

typedef struct
{
	int tokenType;
	string tokenValue;
	int tokenLineNumber;
} Token;

class TreeNode 
{
	public: 
		int lineNumber;
		int nValue;
		string sValue;
		int nodeType;
		int typeSpecifier;
		string rename;
		bool visited;

		TreeNode *C1;
		TreeNode *C2;
		TreeNode *C3;
		TreeNode *sibling;
};

class Scanner
{
	public:
		int setUpFile(char *myFile);
		Token checkOverflow(char *token);
		Token createNewToken(char *token);
		Token getToken();
		int checkSourceFile();	
	private:
		ifstream sourceFile;
		char cline[100];
		int lineNumber;
		int overflow;
		char overflowToken[100];
};

int Scanner::setUpFile(char *myFile)
{
	int success = 0;
	sourceFile.open(myFile);
	if(sourceFile.is_open()){
		cout << "File Openned" << endl;
		lineNumber = 0;
		overflow = 0;
		return 0;
	}else{
		cout << "Error Opening File" << endl;
		return -1;
	}
}

Token Scanner::checkOverflow(char *token)
{
	char newToken[100];
	int i = 0;
	int j = 0;

	if((token[i] == '[') || (token[i] == ']') || (token[i] == '(') || (token[i] == ')') || (token[i] == '{') || (token[i] == '}') || (token[i] == ',') || (token[i] == ';')){
		strncpy(newToken, token, 1);
		newToken[i+1] = '\0';
		overflow = 1;
		while(token[i+1] != '\0'){
			overflowToken[j] = token[i+1];
			i++;
			j++;
		}
		overflowToken[i] = '\0';

	}else{
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

Token Scanner::createNewToken(char *token)
{
	Token newToken;
	int i = 0;

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
		}else if(!strcmp(token,"return")){
			newToken.tokenType = RETURN; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
		}else if(!strcmp(token, "void")){
			newToken.tokenType = VOID; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1; 
		}else if(!strcmp(token, "while")){
			newToken.tokenType = WHILE; newToken.tokenValue = token; newToken.tokenLineNumber = lineNumber-1;
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
			i = 0;
			if((token[i] >= 'A' && token[i] <= 'Z') || (token[i] >= 'a' && token[i] <= 'z')){
				while(token[i] != '\0'){
					if((token[i] >= 'A' && token[i] <= 'Z') || (token[i] >= 'a' && token[i] <= 'z')){
						i++;
					}else{
						newToken = checkOverflow(token);
						return newToken;
					}
				}
				newToken.tokenType = ID;
				newToken.tokenValue = token;
				newToken.tokenLineNumber = lineNumber-1;
			}else if(token[i] >= '0' && token[i] <= '9'){
				while(token[i] != '\0'){
		   			if(token[i] >= '0' && token[i] <= '9'){
						i++;
					}else{
						newToken = checkOverflow(token);
						return newToken;
					}
				}
				newToken.tokenType = NUMBER;
				newToken.tokenValue = token;
				newToken.tokenLineNumber = lineNumber-1;
			}else{
				newToken = checkOverflow(token);
				return newToken;
			}
		}

	return newToken;

}

Token Scanner::getToken()
{
	string line;
	char *token;
	Token newToken;
	int i;

	if(overflow == 1){
		overflow = 0;
		newToken = createNewToken(overflowToken);
	}else{
		token = strtok(NULL, " \t\r\a\n");
		while(token == NULL){
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

int Scanner::checkSourceFile()
{
	if(!sourceFile.eof()){
		return 1;
	}else{
		cout << "End of File" << endl;
		return 0;
	}
}

class Parser
{
	public:	
		TreeNode* Parse(Token token);

		TreeNode* programFunc(); //declListFunc
		TreeNode* declListFunc(); //declListFunc declFunc | declFunc 
		TreeNode* declFunc(); //varDeclFunc | funDeclFunc
		TreeNode* varDeclFunc(); //typeSpec ID ; | typeSpec ID [ NUM ] ;
		// typeSpec -> int | void
		TreeNode* funDeclFunc(); //typeSpec ID ( params ) compoundStmt
		TreeNode* paramsFunc(); //paramListFunc | void
		TreeNode* paramListFunc(); //paramListFunc, paramFunc | paramFunc
		TreeNode* paramFunc(); //typeSpec ID | typeSpec ID [ ]
		TreeNode* compoundStmtFunc(); // { localDeclFunc stmtListFunc }
		TreeNode* localDeclFunc(); // localDeclFunc varDeclFunc | empty
		TreeNode* stmtListFunc(); // stmtListFunc stmtFunc | empty
		TreeNode* stmtFunc(); // expressStmtFunc | compoundStmtFunc | selectStmtFunc | iterStmtFunc | returnStmtFunc
		TreeNode* expressStmtFunc(); //expressionFunc ; | ;
		TreeNode* selectStmtFunc(); //if ( expressionFunc ) stmtFunc | if ( expressionFunc ) stmtFunc else stmtFunc
		TreeNode* iterStmtFunc(); //while ( expressionFunc ) stmtFunc
		TreeNode* returnStmtFunc(); //return ; | return expressionFunc ;
		TreeNode* expressionFunc(); //varFunc = expressionFunc | simpleExprFunc 
		TreeNode* varFunc(); //ID | ID [ expressionFunc ]
		TreeNode* simpleExprFunc(); //addExprFunc relop addExprFunc | addExprFunc
		// relop -> <= | < | > | >= | == | !=
		TreeNode* addExprFunc(); //addExprFun addop termFunc | termFunc
		// addop -> + | -
		TreeNode* termFunc(); //termFunc mulop factorFunc | factorFunc
		// mulop -> * | /
		TreeNode* factorFunc(); //( expressionFunc ) | varFunc | callFunc | num
		TreeNode* callFunc(); //ID ( argsFunc )
		TreeNode* argsFunc(); //argsListFunc | empty
		TreeNode* argsListFunc(); //argsListFunc , expressionFunc | expressionFunc
	
		TreeNode* root; 

};

TreeNode* Parser::Parse(Token token)
{

}

TreeNode* Parser::programFunc()
{
}

TreeNode* Parser::declListFunc()
{
}

TreeNode* Parser::declFunc()
{
}

TreeNode* Parser::varDeclFunc()
{
}

TreeNode* Parser::funDeclFunc()
{
}

TreeNode* Parser::paramsFunc()
{
}

TreeNode* Parser::paramListFunc()
{
}

TreeNode* Parser::paramFunc()
{
}

TreeNode* Parser::compoundStmtFunc()
{
}

TreeNode* Parser::localDeclFunc()
{
}

TreeNode* Parser::stmtListFunc()
{
}

TreeNode* Parser::stmtFunc()
{
}

TreeNode* Parser::expressStmtFunc()
{
}

TreeNode* Parser::selectStmtFunc()
{
}

TreeNode* Parser::iterStmtFunc()
{
}

TreeNode* Parser::returnStmtFunc()
{
}

TreeNode* Parser::expressionFunc()
{
}

TreeNode* Parser::varFunc()
{
}

TreeNode* Parser::simpleExprFunc()
{
}

TreeNode* Parser::addExprFunc()
{
}

TreeNode* Parser::termFunc()
{
}

TreeNode* Parser::factorFunc()
{
}

TreeNode* Parser::callFunc()
{
}

TreeNode* Parser::argsFunc()
{
}

TreeNode* Parser::argsListFunc()
{
}

int checkArguments( int argc, char *argv[], char **myFile)
{
	if(argc > 2) {
		cout << "Too Many Args, Correct Use: trans FILE_NAME" << endl;
		return -1;
	} else if(argc == 1) {
		cout << "Too Few Args, Correct Use: trans FILE_NAME" << endl;
		return -1;
	} else if(argc == 2) {
		*myFile = argv[1];
		cout << "opening file: " << *myFile << endl;
		return 0;
	}
}

int main(int argc, char *argv[])
{

	Scanner scan;
	Parser parse;
	char* myFile;
	Token token;

	if(checkArguments(argc, argv, &myFile) == -1) {
		return -1;
	}

	cout << myFile << endl;

	if(scan.setUpFile(myFile) == -1) {
		return -1;
	}
	
	
	while(scan.checkSourceFile()) {
		token = scan.getToken();
		parse.root = parse.Parse(token);
		cout << token.tokenValue << endl;
	}


	return 0;
}


