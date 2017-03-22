#include"token.h"
#include"treenode.h"
#include"enum.h"
#include"scanner.h"
#include"parser.h"
#include"checker.h"

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
 * translator
 * Author: Joshua Bowen
 * Purpose: The main program for the translator
 * This program will take in a sourceFile, tokenize it, make a parse tree from it, then make a Symbol Table
*****/

/*****
 * Function: checkArguments
 * Purpose: checks to make sure there are enough arguments
*****/

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

/*****
 * Function: main
 * Purpose: Main driving function of the Translator
*****/

int main(int argc, char *argv[])
{
	char* myFile;
	Token token;
	TreeNode* root;
	
	if(checkArguments(argc, argv, &myFile) == -1) {
		return -1;
	}

	Parser* parse = new Parser(myFile);
	Checker* check = new Checker();

	cout << myFile << endl;
	
	//Run the parse Function
	root = parse->Parse();
	cout << "End of File" << endl;

	check->check(root);
	parse->printParseTree();
	check->printSymbolTable();

	return 0;
}


