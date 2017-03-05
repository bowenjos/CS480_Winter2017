#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

using namespace std;

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

class scanner
{
	public:
		int setUpFile(char *myFile);
		char* getToken();
		int checkSourceFile();	
	private:
		ifstream sourceFile;
		char cline[100];
};

int scanner::setUpFile(char *myFile)
{
	int success = 0;
	sourceFile.open(myFile);
	if(sourceFile.is_open()){
		cout << "File Openned" << endl;
		return 0;
	}else{
		cout << "Error Opening File" << endl;
		return -1;
	}
}

char* scanner::getToken()
{
	string line;
	char *token;

	token = strtok(NULL, " \t\r\a\n");
	if(token != NULL){
		return token;
	}else{
		getline(sourceFile, line);
		strcpy(cline, line.c_str());
		token = strtok(cline, " \t\r\a\n");
		return token;
	}	
}

int scanner::checksourcefile()
{
	if(!sourcefile.eof())
		return 1;
	else
		return 0;
}

class parser
{
	public:	
		TreeNode programFunc(); //declListFunc
		TreeNode declListFunc(); //declListFunc declFunc | declFunc 
		TreeNode declFunc(); //varDeclFunc | funDeclFunc
		TreeNode varDeclFunc(); //typeSpec ID ; | typeSpec ID [ NUM ] ;
		// typeSpec -> int | void
		TreeNode funDeclFunc(); //typeSpec ID ( params ) compoundStmt
		TreeNode paramsFunc(); //paramListFunc | void
		TreeNode paramListFunc(); //paramListFunc, paramFunc | paramFunc
		TreeNode paramFunc(); //typeSpec ID | typeSpec ID [ ]
		TreeNode compoundStmtFunc(); // { localDeclFunc stmtListFunc }
		TreeNode localDeclFunc(); // localDeclFunc varDeclFunc | empty
		TreeNode stmtListFunc(); // stmtListFunc stmtFunc | empty
		TreeNode stmtFunc(); // expressStmtFunc | compoundStmtFunc | selectStmtFunc | iterStmtFunc | returnStmtFunc
		TreeNode expressStmtFunc(); //expressionFunc ; | ;
		TreeNode selectStmtFunc(); //if ( expressionFunc ) stmtFunc | if ( expressionFunc ) stmtFunc else stmtFunc
		TreeNode iterStmtFunc(); //while ( expressionFunc ) stmtFunc
		TreeNode returnStmtFunc(); //return ; | return expressionFunc ;
		TreeNode expressionFunc(); //varFunc = expressionFunc | simpleExprFunc 
		TreeNode varFunc(); //ID | ID [ expressionFunc ]
		TreeNode simpleExprFunc(); //addExprFunc relop addExprFunc | addExprFunc
		// relop -> <= | < | > | >= | == | !=
		TreeNode addExprFunc(); //addExprFun addop termFunc | termFunc
		// addop -> + | -
		TreeNode termFunc(); //termFunc mulop factorFunc | factorFunc
		// mulop -> * | /
		TreeNode factorFun(); //( expressionFunc ) | varFunc | callFunc | num
		TreeNode callFunc(); //ID ( argsFunc )
		TreeNode argsFunc(); //argsListFunc | empty
		TreeNode argsListFunc(); //argsListFunc , expressionFunc | expressionFunc
	private:
		TreeNode root; 

};

int checkArguments( int argc, char *argv[], char **myFile)
{
	if(argc > 2) {
		cout << "You have too many command line arguments" << endl;
		return -1;
	} else if(argc == 1) {
		cout << "What is the file you wish to translate: ";
		cin >> *myFile;
		return 0;
	} else if(argc == 2) {
		*myFile = argv[1];
		cout << "opening file: " << *myFile << endl;
		return 0;
	}
}

int main(int argc, char *argv[])
{

	scanner scan;
	char* myFile;
	char* token;

	if(checkArguments(argc, argv, &myFile) == -1) {
		return -1;
	}

	cout << myFile << endl;

	if(scan.setUpFile(myFile) == -1) {
		return -1;
	}
	
	
	while(scan.checkSourceFile()) {
		token = scan.getToken();
		cout << token << endl;
	}


	return 0;
}


