#include "checker.h"

/*****
 * Function: Checker (Constructor)
 * Purpose: Constructs the Checker
 * At the moment it does nothing
*****/

Checker::Checker()
{
}

/*****
 * Function: Checker (Deconstructor)
 * Purpose: Deconstructs the Checker
 * At the moment it does nothing
*****/

Checker::~Checker()
{
}

/*****
 * Function: check
 * Purpose: Begins the process of checking all the nodes
 * Returns the finished Global SymbolTable
*****/

vector<Symbol> Checker::check(TreeNode* currentNode)
{
	checkNode(currentNode);

	return globalTable;	
}

/*****
 * Function: checkNode
 * Purpose: This is the meat of the checker it:
 * adds new variables, arrays, and functions to the table
 * Checks to see if something is in the table
 * moves from node to node
*****/

void Checker::checkNode(TreeNode* currentNode)
{
	Symbol newSymbol;
	int blockLevelUp = 0;
	int checkResult;
	if(currentNode->typeSpecifier == INT || currentNode->typeSpecifier == VOID){
		if(currentNode->nodeType == VARIABLE || currentNode->nodeType == ARRAY){
			insertSymbolTable(currentNode->sValue, currentNode->nodeType, currentNode->typeSpecifier, NULL, currentNode->nValue);
		}else if(currentNode->nodeType == FUNCTION){
			insertSymbolTable(currentNode->sValue, currentNode->nodeType, currentNode->typeSpecifier, currentNode->C1, currentNode->nValue);
		}
	}else if(currentNode->nodeType == VARIABLE || currentNode->nodeType == ARRAY){
		checkResult = checkForSymbol(currentNode->sValue);
		if(checkResult == 0){
			cout << "Variable used has not been declared" << endl;
		}else{
			currentNode->typeSpecifier = getVarType(currentNode->sValue);
		}	
	}

	if(currentNode->nodeType == STATEMENT || currentNode->nodeType == FUNCTION)
	{
		blockLevelUp = 1;
		increaseBlockLevel();
	}

	if(currentNode->C1 != nullptr){
		checkNode(currentNode->C1);
	}

	if(currentNode->C2 != nullptr){
		checkNode(currentNode->C2);
	}

	if(currentNode->C3 != nullptr){
		checkNode(currentNode->C3);
	}

	if(blockLevelUp == 1){
		decreaseBlockLevel();
	}

	if(currentNode->sibling != nullptr){
		checkNode(currentNode->sibling);
	}

}

/*****
 * Function: insertSymbolTable
 * Purpose: Takes data from the Node and puts it in the SymbolTable
*****/

void Checker::insertSymbolTable(string name, int incomingEntryType, int incomingDataType, TreeNode* parameterList, int incomingArrayMax)
{
	Symbol newSymbol;

	newSymbol.ID = name;
	newSymbol.entryType = incomingEntryType;
	if(incomingEntryType == VARIABLE){
		newSymbol.dataType = incomingDataType;
		newSymbol.arrayMax = 0;
		newSymbol.returnType = 0;
	}else if(incomingEntryType == ARRAY){
		newSymbol.dataType = incomingDataType;
		newSymbol.arrayMax = incomingArrayMax;
		newSymbol.returnType = 0;
	}else if(incomingEntryType == FUNCTION){
		newSymbol.returnType = incomingDataType;
		newSymbol.arrayMax = 0;
		newSymbol.dataType = 0;
	}else{
		cout << "Something is wrong with this Symbol" << endl;
	}
	newSymbol.blockLevel = globalBlockLevel;

	if(checkForSymbol(newSymbol.ID)){
		cout << "This Symbol has already been declared in this scope or a higher scope" << endl;
	}

	globalTable.push_back(newSymbol);
	localTable.push_back(newSymbol);

	return;
}

/*****
 * Function: checkForSymbol
 * Purpose: Checks the local table to see if a variable exists or not
 * This is used to check if something is being declared twice
 * It's also used to check to make sure something is declared at all
*****/

int Checker::checkForSymbol(string name)
{
	for(int i = 0; i < localTable.size(); i++){
		if(localTable[i].ID == name){
			return 1;
		}
	}
	
	return 0;
	
}

int Checker::getVarType(string name)
{
	for(int i = 0; i < localTable.size(); i++){
		if(localTable[i].ID == name){
			if(localTable[i].entryType == FUNCTION){
				return localTable[i].returnType;
			}else{
				return localTable[i].dataType;
			}
		}
	}
}

/*****
 * Function: getGlobalTable
 * Purpose; returns the global symbol table
*****/

vector<Symbol> Checker::getGlobalTable()
{
	return globalTable;
}

/*****
 * Function: getLocalTable
 * Purpose: returns the local symbol table
*****/

vector<Symbol> Checker::getLocalTable()
{
	return localTable;
}

/*****
 * Function: increaseBlockLevel
 * Purpose: ups the block level by one
*****/

void Checker::increaseBlockLevel()
{
	globalBlockLevel = globalBlockLevel + 1;

	return;
}

/*****
 * Function: decreaseBlockLevel
 * Purpose: deletes all things from the localtable that are on the current block level
 * decreases the global block level by one
*****/

void Checker::decreaseBlockLevel()
{
	while(localTable.back().blockLevel == globalBlockLevel){
		localTable.pop_back();
	}

	globalBlockLevel = globalBlockLevel - 1;

	return;
}

string Checker::enumToString(int enumValue)
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

/*****
 * Function: printSymbolTable
 * Purpose: iterates accross the global symbol table and prints it's contents
*****/

void Checker::printSymbolTable()
{
	symbolTableFile.open("symbolTable.txt");

	symbolTableFile << "ID blockLevel entryType dataType returnType arrayMax" << endl;

	for(int i = 0; i < globalTable.size(); i++){
		symbolTableFile << globalTable[i].ID << " ";
		symbolTableFile << globalTable[i].blockLevel << " ";
		symbolTableFile << enumToString(globalTable[i].entryType)<< " ";
		symbolTableFile << enumToString(globalTable[i].dataType) << " ";
		symbolTableFile << enumToString(globalTable[i].returnType) << " ";
		symbolTableFile << globalTable[i].arrayMax << " ";
		symbolTableFile << endl;
	}

	return;

}
