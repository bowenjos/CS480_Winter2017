#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<time.h>
#include<unistd.h>

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
		TreeNode();

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

TreeNode::TreeNode()
{
	lineNumber = 0;
	nValue = 0;
	sValue = "NULL";
	nodeType = 0;
	typeSpecifier = 0;
	rename = "NULL";
	visited = 0;
}

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

Scanner::Scanner(char* myFile)
{
	this->sourceFile.open(myFile);
	ignorePeek = 0;
	lineNumber = 0;
}

Scanner::~Scanner()
{
	if(this->sourceFile.is_open()){
		this->sourceFile.close();
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

	if(peeked == 1 && ignorePeek != 1){
		peeked = 0;
		newToken = peekToken;
	}else if(peekedFurther == 1 && ignorePeek != 1){
		peekedFurther = 0;
		newToken = peekFurtherToken;
	}else if(overflow == 1){
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

	cout << newToken.tokenValue << endl;
	usleep(50000);

	return newToken;
}

Token Scanner::peek()
{
	cout << "peek: ";
	peekToken = getToken();
	peeked = 1;
	
	return peekToken;
}

Token Scanner::peekFurther()
{
	cout << "peekFurther: ";
	ignorePeek = 1;
	peekFurtherToken = getToken();
	peekedFurther = 1;
	ignorePeek = 0;
	
	return peekFurtherToken;
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
		TreeNode* Parse(); //Start Parser

		Parser(char*);
		~Parser();

		TreeNode* programFunc(Token token); //declListFunc
		TreeNode* varDeclFunc(Token token); //typeSpec ID ; | typeSpec ID [ NUM ] ;
		// typeSpec -> int | void
		TreeNode* funDeclFunc(Token token); //typeSpec ID ( params ) compoundStmt
		TreeNode* paramsListFunc(Token token); //paramListFunc, paramFunc | paramFunc
		TreeNode* paramFunc(Token token); //typeSpec ID | typeSpec ID [ ]
		TreeNode* compoundStmtFunc(Token token); // { localDeclFunc stmtListFunc }
		TreeNode* localDeclFunc(Token token); // localDeclFunc varDeclFunc | empty
		TreeNode* localVarDeclFunc(Token token);
		TreeNode* stmtListFunc(Token token); // stmtListFunc stmtFunc | empty
		TreeNode* stmtFunc(Token token); // expressStmtFunc | compoundStmtFunc | selectStmtFunc | iterStmtFunc | returnStmtFunc
		TreeNode* selectStmtFunc(Token token); //if ( expressionFunc ) stmtFunc | if ( expressionFunc ) stmtFunc else stmtFunc
		TreeNode* iterStmtFunc(Token token); //while ( expressionFunc ) stmtFunc
		TreeNode* returnStmtFunc(Token token); //return ; | return expressionFunc ;
		TreeNode* readStmtFunc(Token token); //READ ID ;
		TreeNode* writeStmtFunc(Token token); //WRITE ID ;
		TreeNode* callStmtFunc(Token token); // ID ( argsFunc ) ;
		TreeNode* callFunc(Token token); // ID ( argsFunc )
		TreeNode* gainFunc(Token token); //varFunc = expressionFunc ;	
		TreeNode* varFunc(Token token); //ID | ID [ expressionFunc ]
		TreeNode* compareExprFunc(Token token); //addExprFunc relop addExprFunc | addExprFunc
		TreeNode* expressionFunc(Token token); 
		TreeNode* numberFunc(Token token);
		// relop -> <= | < | > | >= | == | !=
		// addop -> + | -
		// mulop -> * | /
		TreeNode* argsFunc(Token token); //argsListFunc | empty
		TreeNode* argsListFunc(Token token); //argsListFunc , expressionFunc | expressionFunc
	
		TreeNode* root; 

		Scanner* scan;

};

Parser::Parser(char* myFile)
{
	this->scan = new Scanner(myFile);
}

Parser::~Parser()
{
	delete this->scan;
}

TreeNode* Parser::Parse()
{
	cout << "Parse" << endl;

	Token token = scan->getToken();
	
	root = programFunc(token);

	return root;
}

TreeNode* Parser::programFunc(Token token)
{
	cout << "programFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token peekToken = scan->peek();
	Token peekFurtherToken = scan->peekFurther();

	newNode->nodeType = 1;
	newNode->nodeType = PROGRAM;
	
	newNode->lineNumber = token.tokenLineNumber;

	if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && ((peekFurtherToken.tokenType == SEMI) || peekFurtherToken.tokenType == LBRACKET)){
		newNode->sibling = varDeclFunc(token);
	}else if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && (peekFurtherToken.tokenType == LPAREN)){
		newNode->sibling = funDeclFunc(token);
	}
	

	return newNode;

}

TreeNode* Parser::varDeclFunc(Token token)
{

	cout << "varDeclFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	Token peekFurtherToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->typeSpecifier = token.tokenType;

	newToken = scan->getToken();

	newNode->sValue = token.tokenValue;

	newToken = scan->getToken();

	if(newToken.tokenType == LBRACKET){
		newNode->nodeType = ARRAY;
		newToken = scan->getToken();
		if(newToken.tokenType != NUMBER){
			//ERROR
		}
		newNode->nValue = atoi(newToken.tokenValue.c_str());
		newToken = scan->getToken();
		if(newToken.tokenType == RBRACKET){
			newToken = scan->getToken();
			if(newToken.tokenType != SEMI){
				//Error
			}
		}else{
			//Error
		}
	}else if(newToken.tokenType == SEMI){
		newNode->nodeType = VARIABLE;
	}else{
		//Error
	}
	
	newToken = scan->getToken();
	if(newToken.tokenType == END){
		return newNode;
	}

	peekToken = scan->peek();
	peekFurtherToken = scan->peekFurther();

	if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && ((peekFurtherToken.tokenType == SEMI) || peekFurtherToken.tokenType == LBRACKET))
		newNode->sibling = varDeclFunc(token);
	else if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && (peekFurtherToken.tokenType == LPAREN))
		newNode->sibling = funDeclFunc(token);

	return newNode;
}

TreeNode* Parser::funDeclFunc(Token token)
{
	cout << "funDeclFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	Token peekFurtherToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->typeSpecifier = token.tokenType;

	newToken = scan->getToken();

	newNode->sValue = token.tokenValue;

	newToken = scan->getToken();

	if(newToken.tokenType == LPAREN){
		newNode->nodeType = FUNCTION;
		newToken = scan->getToken();
		newNode->C1 = paramsListFunc(newToken);
		newToken = scan->getToken();
		if(newToken.tokenType == RPAREN){
			newToken = scan->getToken();
			newNode->C2 = compoundStmtFunc(newToken);
		}else{
			//ERROR
			cout << "Line Number " << newNode->lineNumber << " ERROR: Needed Right Paren" << endl;
		}
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Needed Left Paren" << endl;
	}

	peekToken = scan->peek();
	if(peekToken.tokenType == END){
		return newNode;
	}else{
		newToken = scan->getToken();
		newNode->sibling = funDeclFunc(token);
	}

	return newNode;
}

TreeNode* Parser::paramsListFunc(Token token)
{
	cout << "paramsListFunc" << endl;

	TreeNode* newNode = new TreeNode();

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = STATEMENT_LIST;

	if(token.tokenType == VOID || token.tokenType == RPAREN){
		newNode->typeSpecifier = VOID;
	}else if(token.tokenType == INT){
		newNode->sibling = paramFunc(token);
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Are your parameters correct?" << endl;
	}

	return newNode;
}

TreeNode* Parser::paramFunc(Token token)
{
	cout << "paramFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;

	if(token.tokenType == INT || token.tokenType == VOID){
		newNode->typeSpecifier = token.tokenType;
		newToken = scan->getToken();
		newNode->sValue = newToken.tokenValue;
		peekToken = scan->peek();

		if(peekToken.tokenType == LBRACKET){
			newToken = scan->getToken();
			newNode->nodeType = ARRAY;
			newToken = scan->getToken();
			if(newToken.tokenType != RBRACKET){
				//ERROR
				cout << "Line Number " << newNode->lineNumber << " ERROR: Your array is broken" << endl;
			}
		}else{
			newNode->nodeType = VARIABLE;
		}
		peekToken = scan->peek();

		if(peekToken.tokenType == COMMA){
			newToken = scan->getToken();
			newToken = scan->getToken();
			newNode->sibling = paramFunc(newToken);
		}else if(peekToken.tokenType == RPAREN){
			return newNode;
		}else{
			//ERROR
			cout << "Line Number " << newNode->lineNumber << " ERROR: Something is wrong with your parameters" << endl;
		}
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Your parameter lacks a type specifier" << endl;
	}

	return newNode;
}

TreeNode* Parser::compoundStmtFunc(Token token)
{
	cout << "compoundStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = COMPOUND;

	if(token.tokenType == LBRACE){
		newToken = scan->getToken();
		if(newToken.tokenType == INT || newToken.tokenType == VOID){
			newNode->C1 = localDeclFunc(newToken);
			newToken = scan->getToken();
		}else{
			newNode->C1 = localDeclFunc(newToken);
		}
		newNode->C2 = stmtListFunc(newToken);
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing a Left Brace" << endl;
	}
	newToken = scan->getToken();
	if(newToken.tokenType != RBRACE){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing a Right Brace" << endl;
	}

	cout << "Leaving compoundStmtFunc" << endl;
	return newNode;

}

TreeNode* Parser::localDeclFunc(Token token)
{
	cout << "localDeclFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = DECLARATION;

	if(token.tokenType == VOID || token.tokenType == INT){
		newNode->sibling = localVarDeclFunc(token);
	}else{
		newNode->sibling = NULL;
	}

	return newNode;

}

TreeNode* Parser::localVarDeclFunc(Token token)
{
	cout << "localVarDeclFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	
	if(token.tokenType == INT || token.tokenType == VOID){
		newNode->typeSpecifier = token.tokenType;
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing a type specifier" << endl;
	}
	newToken = scan->getToken();
	if(newToken.tokenType != ID){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing a variable or array name" << endl;
	}
	newNode->sValue = newToken.tokenValue;
	peekToken = scan->peek();
	if(peekToken.tokenType == LBRACKET){
		newNode->nodeType = ARRAY;
		newToken = scan->getToken();
		newToken = scan->getToken();
		if(newToken.tokenType == NUMBER){
			newNode->nValue = atoi(newToken.tokenValue.c_str());
			newToken = scan->getToken();
			if(newToken.tokenType != RBRACKET){
				//ERROR
				cout << "Line Number " << newNode->lineNumber << " ERROR: Array isn't properly closed" << endl;
			}
		}else if(newToken.tokenType != RBRACKET){
			//ERROR
			cout << "Line Number " << newNode->lineNumber << " ERROR: Array isn't properly closed" << endl;
		}
	}
	newToken = scan->getToken();
	if(newToken.tokenType != SEMI){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing semicolon" << endl;
	}
	
	peekToken = scan->peek();
	if(peekToken.tokenType == VOID || peekToken.tokenType == INT){
		newToken = scan->getToken();
		newNode->sibling = localVarDeclFunc(token);
	}

	return newNode;
}

TreeNode* Parser::stmtListFunc(Token token)
{
	cout << "stmtListFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = STATEMENT_LIST;

	if(token.tokenType == ID){
		peekToken = scan->peek();
		if(peekToken.tokenType == LPAREN){
			newNode->sibling = callStmtFunc(token);
		}else if(peekToken.tokenType == ASSIGN || peekToken.tokenType == LBRACKET){
			newNode->sibling = gainFunc(token);
		}else{
			cout << "Line Number " << newNode->lineNumber << " ERROR: Included ID, but is neither an assignment nor a function call" << endl;
			//ERROR
		}
	}else if(token.tokenType == IF){
		newNode->sibling = selectStmtFunc(token);
	}else if(token.tokenType == WHILE){
		newNode->sibling = iterStmtFunc(token);
	}else if(token.tokenType == RETURN){
		newNode->sibling = returnStmtFunc(token);
	}else if(token.tokenType == READ){
		newNode->sibling = readStmtFunc(token);
	}else if(token.tokenType == WRITE){
		newNode->sibling = writeStmtFunc(token);
	}else if(token.tokenType == CALL){
		newNode->sibling = callFunc(token);
	}else if(token.tokenType == LBRACE){
		newNode->sibling = compoundStmtFunc(token);
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Statement written is not a valid statement type" << endl;
	}

	return newNode;

}


TreeNode* Parser::stmtFunc(Token token)
{
	cout << "stmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	if(token.tokenType == ID){
		peekToken = scan->peek();
		if(peekToken.tokenType == LPAREN){
			return callStmtFunc(token);
		}else if(peekToken.tokenType == ASSIGN || peekToken.tokenType == LBRACKET){
			return gainFunc(token);
		}else{
			//ERROR
			cout << "Line Number " << newNode->lineNumber << " ERROR: Included ID, but is neither an assignment nor a function call" << endl;
		}
	}else if(token.tokenType == IF){
		return selectStmtFunc(token);
	}else if(token.tokenType == WHILE){
		return iterStmtFunc(token);
	}else if(token.tokenType == RETURN){
		return returnStmtFunc(token);
	}else if(token.tokenType == READ){
		return readStmtFunc(token);
	}else if(token.tokenType == WRITE){
		return writeStmtFunc(token);
	}else if(token.tokenType == CALL){
		return callFunc(token);
	}else if(token.tokenType == LBRACE){
		return compoundStmtFunc(token);
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Statement written is not a valid statement type" << endl;
	}

}

TreeNode* Parser::selectStmtFunc(Token token)
{
	cout << "selectStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = STATEMENT;
	newNode->sValue = token.tokenValue;

	if(token.tokenType != IF){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: You probably shouldn't ever get this error" << endl;
	}
	newToken = scan->getToken();
	if(newToken.tokenType != LPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: IF statement but no Parenthesis" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = compareExprFunc(newToken);
	newToken = scan->getToken();
	if(newToken.tokenType != RPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: IF statement not properly closed" << endl;
	}
	newToken = scan->getToken();
	newNode->C2 = compoundStmtFunc(newToken);
	peekToken = scan->peek();
	if(peekToken.tokenType == ELSE){
		newToken = scan->getToken();
		newToken = scan->getToken();
		newNode->C3 = compoundStmtFunc(newToken);
	}
	
	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		cout << "Leaving selectStmtFunc" << endl;
		return newNode;
	}
	
	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);


	cout << "Leaving selectStmtFunc" << endl;
	return newNode;
}

TreeNode* Parser::iterStmtFunc(Token token)
{
	cout << "iterStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = STATEMENT;
	newNode->sValue = token.tokenValue;

	if(token.tokenType != WHILE){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Probably won't see this error" << endl;
	}
	newToken = scan->getToken();
	if(newToken.tokenType != LPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: WHILE statement, but no parenthesis" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = compareExprFunc(newToken);
	newToken = scan->getToken();
	if(newToken.tokenType != RPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Parenthesis not properly closed" << endl;
	}
	newToken = scan->getToken();
	newNode->C2 = compoundStmtFunc(newToken);
			
	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		return newNode;
	}

	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);

	cout << "Leaving iterStmtFunc" << endl;
	return newNode;
}

TreeNode* Parser::returnStmtFunc(Token token)
{
	cout << "returnStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = RETURN;

	if(token.tokenType != RETURN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: You shouldn't see this error" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = expressionFunc(newToken);

	newToken = scan->getToken();
	if(newToken.tokenType != SEMI){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing semicolon" << endl;
	}

	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		return newNode;
	}

	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);

	return newNode;
}

TreeNode* Parser::readStmtFunc(Token token)
{
	cout << "readStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = READ;

	if(token.tokenType != READ){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: You shouldn't see this error" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = varFunc(newToken);
	newToken = scan->getToken();
	if(newToken.tokenType != SEMI){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing semicolon" << endl;
	}

	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		return newNode;
	}

	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);

	return newNode;
}

TreeNode* Parser::writeStmtFunc(Token token)
{
	cout << "writeStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = WRITE;

	if(token.tokenType != WRITE){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: You shouldn't see this error" << endl;
	}

	newToken = scan->getToken();
	if(newToken.tokenType != LPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing parenthesis" << endl;
	}

	cout << "Here" << endl;

	newToken = scan->getToken();
	peekToken = scan->peek();

	newNode->C1 = expressionFunc(newToken);

	newToken = scan->getToken();
	if(newToken.tokenType != RPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Parenthesis not closed" << endl;
	}

	newToken = scan->getToken();
	if(newToken.tokenType != SEMI){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing semicolon" << endl;

	}

	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		return newNode;
	}

	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);

	return newNode;
}

TreeNode* Parser::callStmtFunc(Token token)
{
	cout << "callStmtFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	
	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = CALL;

	newNode->typeSpecifier = VOID; //????

	if(token.tokenType != ID){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Function call is lacking ID, you probs shouldn't see this" << endl;
	}
	newNode->sValue = token.tokenValue;
	newToken = scan->getToken();
	if(newToken.tokenType != LPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Function call lacking parenthesis" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = argsListFunc(newToken);
	newToken = scan->getToken();
	if(newToken.tokenType != RPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Parenthesis are not closed properly" << endl;
	}
	newToken = scan->getToken();
	if(newToken.tokenType != SEMI){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing semicolon" << endl;

	}
	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		return newNode;
	}

	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);

	return newNode;
}

TreeNode* Parser::callFunc(Token token)
{
	cout << "callFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	
	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = CALL;

	newNode->typeSpecifier = VOID; //????

	if(token.tokenType != ID){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Function call is lacking ID, you probs shouldn't see this" << endl;
	}
	newNode->sValue = token.tokenValue;
	newToken = scan->getToken();
	if(newToken.tokenType != LPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Function call lacking parenthesis" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = argsListFunc(newToken);
	newToken = scan->getToken();
	if(newToken.tokenType != RPAREN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Parenthesis are not closed properly" << endl;
	}

	return newNode;
}


TreeNode* Parser::gainFunc(Token token)
{
	cout << "gainFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->C1 = varFunc(token);
	newToken = scan->getToken();

	if(newToken.tokenType != ASSIGN){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing assignment character" << endl;
	}

	newNode->nodeType = ASSIGN;

	newToken = scan->getToken();
	
	newNode->C2 = expressionFunc(newToken);
	
	newToken = scan->getToken();
	if(newToken.tokenType != SEMI){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Missing semicolon" << endl;

	}

	peekToken = scan->peek();
	if(peekToken.tokenType == RBRACE){
		return newNode;
	}else{
		newToken = scan->getToken();
		newNode->sibling = stmtFunc(newToken);
	}

	cout << "Leaving gainFunc" << endl;
	return newNode;

}

TreeNode* Parser::varFunc(Token token)
{
	cout << "varFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;

	if(token.tokenType != ID){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Variable is not an ID" << endl;
	}
	newNode->sValue = token.tokenValue;
	peekToken = scan->peek();
	if(peekToken.tokenType == LBRACKET)
	{
		newToken = scan->getToken();
		newNode->nodeType = ARRAY;
		newToken = scan->getToken();
		newNode->C1 = expressionFunc(newToken);
		newToken = scan->getToken();
		if(newToken.tokenType != RBRACKET){
			//ERROR
			cout << "Line Number " << newNode->lineNumber << " ERROR: ARRAY is not closed properly" << endl;

		}
	}else{
		newNode->nodeType = VARIABLE;
	}

	cout << "leaving varFunc" << endl;

	return newNode;
}

TreeNode* Parser::compareExprFunc(Token token)
{
	cout << "compareExprFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;

	newNode->C1 = expressionFunc(token);
	newToken = scan->getToken();
	newNode->nodeType = newToken.tokenType;
	newToken = scan->getToken();
	newNode->C2 = expressionFunc(newToken);
	return newNode;
}

TreeNode* Parser::expressionFunc(Token token)
{

	cout << "expressionFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	TreeNode* tempNode = new TreeNode();

	newNode->lineNumber = token.tokenLineNumber;

	peekToken = scan->peek();
	if(token.tokenType == ID && peekToken.tokenType != LPAREN){
		tempNode = varFunc(token);
	}else if(token.tokenType == ID && peekToken.tokenType == LPAREN){
		tempNode = callFunc(token);
	}else if(token.tokenType == NUMBER){
		tempNode = numberFunc(token);
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Expression has neither Variable nor Number" << endl;
	}

	peekToken = scan->peek();

	if((peekToken.tokenType != PLUS) && (peekToken.tokenType != MINUS) && (peekToken.tokenType != MULT) && (peekToken.tokenType != DIV)){
		//ERROR
		cout << "Exit expressionFunc" << endl;
		return tempNode;
		cout << "Line Number " << newNode->lineNumber << " ERROR: Not a valid operand" << endl;
	}
	newToken = scan->getToken();
	newNode->C1 = tempNode;

	newNode->nodeType = newToken.tokenType;
	newToken = scan->getToken();
	peekToken = scan->peek();
	if(newToken.tokenType == ID){
		if(peekToken.tokenType != SEMI && peekToken.tokenType != RPAREN && peekToken.tokenType != RBRACE){
			newNode->C2 = expressionFunc(newToken);
		}else{
			newNode->C2 = varFunc(newToken);
		}
	}else if(newToken.tokenType == NUMBER){
		if(peekToken.tokenType != SEMI && peekToken.tokenType != RPAREN && peekToken.tokenType != RBRACE){
			newNode->C2 = expressionFunc(newToken);
		}else{
			newNode->C2 = numberFunc(newToken);
		}
	}
	cout << "Exit expressionFunc" << endl;
	return newNode;

}

TreeNode* Parser::numberFunc(Token token)
{

	cout << "numberFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;

	newNode->lineNumber = token.tokenLineNumber;
	if(token.tokenType != NUMBER){
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: Not a number" << endl;
	}
	newNode->nValue = atoi(token.tokenValue.c_str());
	newNode->nodeType = NUMBER;

	return newNode;

}

TreeNode* Parser::argsFunc(Token token)
{
	cout << "argsFunc" << endl;

	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;

	if(token.tokenType == NUMBER){
		newNode->nodeType = NUMBER;
		newNode->nValue = atoi(token.tokenValue.c_str());

		peekToken = scan->peek();

		if(peekToken.tokenType == COMMA){
			newToken = scan->getToken();
			newToken = scan->getToken();
			newNode->sibling = argsFunc(newToken);
		}else if(peekToken.tokenType == RPAREN){
			return newNode;
		}
	}else if(token.tokenType == ID){
		peekToken = scan->peek();
		if(peekToken.tokenType == LBRACKET){
			newNode->nodeType == ARRAY;
			newToken = scan->getToken();
			newToken = scan->getToken();
			//REVISIT
			newNode->C1 = expressionFunc(newToken);
			newToken = scan->getToken();
			if(newToken.tokenType != RBRACKET){
				//ERROR
				cout << "Line Number " << newNode->lineNumber << " ERROR: ARRAY is not closed properly" << endl;
			}
			peekToken = scan->peek();
		}else if(peekToken.tokenType == COMMA){
			newNode->nodeType == VARIABLE;
		}

		if(peekToken.tokenType == COMMA){
			newToken = scan->getToken();
			newToken = scan->getToken();
			newNode->sibling = argsFunc(newToken);
		}else if(peekToken.tokenType == RPAREN){
			cout << "Leaving argsFunc" << endl;
			return newNode;
		}
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: there is something wrng with your arguments" << endl;
	}
	cout << "Leaving argsFunc" << endl;

	return newNode;
}

TreeNode* Parser::argsListFunc(Token token)
{
	cout << "argsListFunc" << endl;

	TreeNode* newNode = new TreeNode();

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = ARGUMENTS;

	newNode->sibling = argsFunc(token);

	return newNode;

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
	char* myFile;
	Token token;
	
	if(checkArguments(argc, argv, &myFile) == -1) {
		return -1;
	}

	Parser* parse = new Parser(myFile);

	cout << myFile << endl;
	
	parse->Parse();

	cout << "End of File" << endl;

	return 0;
}


