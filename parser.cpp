#include "parser.h"

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

void Parser::printParseTree()
{
	parseTreeFile.open("parseTree.txt");	
	parseTreeFile << "Root" << endl;
	printNode(root);
	parseTreeFile.close();

	return;
}

void Parser::printNode(TreeNode* currentNode)
{
	parseTreeFile << "Node Type: " << enumToString(currentNode->nodeType) << endl;
	parseTreeFile << "Line Number: " << currentNode->lineNumber << endl;
	parseTreeFile << "Name: " << currentNode->sValue << endl;
	parseTreeFile << "Value: " << currentNode->nValue << endl;
	parseTreeFile << "Data Type: " << enumToString(currentNode->typeSpecifier) << endl;
	parseTreeFile << endl;

	if(currentNode->C1 != nullptr){
		parseTreeFile << "Child 1" << endl;
		printNode(currentNode->C1);
	}

	if(currentNode->C2 != nullptr){
		parseTreeFile << "Child 2" << endl;
		printNode(currentNode->C2);
	}

	if(currentNode->C3 != nullptr){
		parseTreeFile << "Child 3" << endl;
		printNode(currentNode->C3);
	}

	if(currentNode->sibling != nullptr){
		parseTreeFile << "Sibling" << endl;
		printNode(currentNode->sibling);
	}

	return;
}

TreeNode* Parser::programFunc(Token token)
{
	TreeNode* newNode = new TreeNode();
	Token peekToken = scan->peek();
	Token peekFurtherToken = scan->peekFurther();

	newNode->nodeType = 1;
	newNode->nodeType = PROGRAM;
	newNode->lineNumber = token.tokenLineNumber;
	newNode->C1 = NULL;
	newNode->C2 = NULL;
	newNode->C3 = NULL;

	if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && ((peekFurtherToken.tokenType == SEMI) || peekFurtherToken.tokenType == LBRACKET)){
		newNode->sibling = varDeclFunc(token);
	}else if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && (peekFurtherToken.tokenType == LPAREN)){
		newNode->sibling = funDeclFunc(token);
	}

	return newNode;

}

TreeNode* Parser::varDeclFunc(Token token)
{
	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	Token peekFurtherToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->typeSpecifier = token.tokenType;
	newToken = scan->getToken();
	newNode->sValue = newToken.tokenValue;
	newToken = scan->getToken();
	newNode->C1 = NULL;
	newNode->C2 = NULL;
	newNode->C3 = NULL;

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
		newNode->sibling = varDeclFunc(newToken);
	else if((token.tokenType == INT || token.tokenType == VOID) && (peekToken.tokenType == ID) && (peekFurtherToken.tokenType == LPAREN))
		newNode->sibling = funDeclFunc(newToken);

	return newNode;
}

TreeNode* Parser::funDeclFunc(Token token)
{
	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	Token peekFurtherToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->typeSpecifier = token.tokenType;
	newToken = scan->getToken();
	newNode->sValue = newToken.tokenValue;
	newToken = scan->getToken();
	newNode->C3 = NULL;

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
		newNode->sibling = funDeclFunc(newToken);
	}

	return newNode;
}

TreeNode* Parser::paramsListFunc(Token token)
{
	TreeNode* newNode = new TreeNode();

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = STATEMENT_LIST;
	newNode->C1 = NULL;
	newNode->C2 = NULL;
	newNode->C3 = NULL;

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
	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->C1 = NULL;
	newNode->C2 = NULL;
	newNode->C3 = NULL;

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
	TreeNode* newNode = new TreeNode();
	Token newToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = COMPOUND;
	newNode->C3 = NULL;
	newNode->sibling = NULL;

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

	return newNode;
}

TreeNode* Parser::localDeclFunc(Token token)
{
	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = DECLARATION;
	newNode->C1 = NULL;
	newNode->C2 = NULL;
	newNode->C3 = NULL;

	if(token.tokenType == VOID || token.tokenType == INT){
		newNode->sibling = localVarDeclFunc(token);
	}else{
		newNode->sibling = NULL;
	}

	return newNode;
}

TreeNode* Parser::localVarDeclFunc(Token token)
{
	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;

	newNode->lineNumber = token.tokenLineNumber;
	newNode->C1 = NULL;
	newNode->C2 = NULL;
	newNode->C3 = NULL;

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
	}else{
		newNode->nodeType = VARIABLE;
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
		return newNode;
	}
	
	newToken = scan->getToken();
	newNode->sibling = stmtFunc(newToken);

	return newNode;
}

TreeNode* Parser::iterStmtFunc(Token token)
{
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

	return newNode;
}

TreeNode* Parser::returnStmtFunc(Token token)
{
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
	TreeNode* newNode = new TreeNode();
	Token newToken;
	Token peekToken;
	
	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = CALL;

	newNode->typeSpecifier = 0; //????

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

	return newNode;
}

TreeNode* Parser::varFunc(Token token)
{
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

	return newNode;
}

TreeNode* Parser::compareExprFunc(Token token)
{
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
		return tempNode;
		//cout << "Line Number " << newNode->lineNumber << " ERROR: Not a valid operand" << endl;
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

	return newNode;
}

TreeNode* Parser::numberFunc(Token token)
{
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
			newNode->nodeType = ARRAY;
			newNode->sValue = token.tokenValue;
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
			newNode->nodeType = VARIABLE;
			newNode->sValue = token.tokenValue;
		}

		if(peekToken.tokenType == COMMA){
			newToken = scan->getToken();
			newToken = scan->getToken();
			newNode->sibling = argsFunc(newToken);
		}else if(peekToken.tokenType == RPAREN){
			return newNode;
		}
	}else{
		//ERROR
		cout << "Line Number " << newNode->lineNumber << " ERROR: there is something wrng with your arguments" << endl;
	}
	
	return newNode;
}

TreeNode* Parser::argsListFunc(Token token)
{
	TreeNode* newNode = new TreeNode();

	newNode->lineNumber = token.tokenLineNumber;
	newNode->nodeType = ARGUMENTS;

	newNode->sibling = argsFunc(token);

	return newNode;

}

string Parser::enumToString(int enumValue)
{
	string enumString[] = { "NOTHING", "EOF", "ERROR", "ELSE", "IF", "INT", "RETURN",	"VOID",	"WHILE",
				"PLUS", "MINUS", "MULT", "DIV",	"LS", "LEQ", "GT", "GEQ", "EQ", 
				"NEQ", "ASSIGN", "SEMI", "COMMA", "LPAREN", "RPAREN", "LBRACKET", 
				"RBRACKET", "LBRACE", "RBRACE",	"READ", "WRITE", "NUMBER", "ID",
				"PROGRAM", "DECLARATION", "VARIABLE", "ARRAY", "FUNCTION",
				"EXPRESSION", "CALL", "COMPOUND", "TYPE_SPECIFIER",	"PARAMETER_LIST",
				"PARAMETER", "STATEMENT_LIST", "STATEMENT",	"ARGUMENTS", "END" };

	return enumString[enumValue];

}
