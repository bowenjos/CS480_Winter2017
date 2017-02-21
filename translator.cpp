#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

using namespace std;

class scanner
{
	public:
		int setUpFile(char *myFile);
		void getToken(); 	
	private:
		ifstream sourceFile;
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

void scanner::getToken()
{
	string line;
	char cline[100];
	char *token;

	while(!sourceFile.eof()) {
		getline(sourceFile, line);
		strcpy(cline, line.c_str());
		token = strtok(cline, " \t\r\a\n");
		while(token != NULL) {
			cout << chk << endl;
			token = strtok(NULL, " \t\r\a\n");
		}
	}

	return;
}



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

	if(checkArguments(argc, argv, &myFile) == -1) {
		return -1;
	}

	cout << myFile << endl;

	if(scan.setUpFile(myFile) == -1) {
		return -1;
	}

	scan.getToken();


	return 0;
}


