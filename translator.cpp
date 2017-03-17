#include"token.h"
#include"treenode.h"
#include"enum.h"
#include"scanner.h"
#include"parser.h"

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<time.h>
#include<unistd.h>

using namespace std;

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


