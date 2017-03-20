#include "checker.h"

Checker::Checker()
{
}

Checker::~Checker()
{
}

vector<Symbol> Checker::check(TreeNode node)
{
	
}

void Checker::checkNode(int insertOrCheck)
{

}

void Checker::insertSymbolTable(string name, int incomingEntryType, int incomingDataType, TreeNode* parameterList, int incomingArrayMax)
{
	Symbol newSymbol;

	newSymbol.ID = name;
	newSymbol.entryType = incomingEntryType;
	if(incomingEntryType == VARIABLE){
		newSymbol.dataType = incomingDataType;
	}else if(incomingEntryType == ARRAY){
		newSymbol.dataType = incomingDataType;
		newSymbol.arrayMax = incomingArrayMax;
	}else if(incomingEntryType == FUNCTION){
		newSymbol.returnType = incomingDataType;
	}else{
		cout << "Something is wrong with this Symbol" << endl;
	}
	newSymbol.blockLevel = globalBlockLevel;

	if(checkForSymbol(newSymbol)){
		cout << "This Symbol has already been declared in this scope or a higher scope" << endl;
	}

	globalTable.push_back(newSymbol);
	localTable.push_back(newSymbol);

	return;
}

int Checker::checkForSymbol(Symbol symbol)
{
	for(int i = 0; i < localTable.size(); i++){
		if(localTable[i].ID == symbol.ID){
			return 1;
		}
	}
	
	return 0;
	
}

vector<Symbol> Checker::getGlobalTable()
{
	return globalTable;
}

vector<Symbol> Checker::getLocalTable()
{
	return localTable;
}

void Checker::increaseBlockLevel()
{
	globalBlockLevel = globalBlockLevel + 1;

	return;
}

void Checker::decreaseBlockLevel()
{
	while(localTable.back().blockLevel == globalBlockLevel){
		localTable.pop_back();
	}

	globalBlockLevel = globalBlockLevel - 1;

	return;
}

void Checker::printSymbolTable()
{
	
}
