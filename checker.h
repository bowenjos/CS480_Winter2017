#ifndef _CHECKER_H
#define _CHECKER_H

#include "enum.h"
#include "treenode.h"

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>

using namespace std;

/*****
 * checker.h
 * Author: Joshua Bowen
 * Purpose: This generates a symbol table that is used by the code generator (Which will not exist)
 * This file contains the SymbolTable struct, as well as the checker class
*****/

typedef struct
{
	string ID; //Variable name
	int entryType; // variable, array, etc.
	int dataType; // INT or VOID
	int blockLevel; // The scope of the current item
	TreeNode* parameterList; //Just copied from the syntax tree
	int returnType; //For functions, either INT or VOID
	int arrayMax; //Size of an array
	string rename; //Each variable is given a unique name
} Symbol;

class Checker
{
	public:
		Checker();
		~Checker();

		vector<Symbol> check(TreeNode* currentNode);
		void checkNode(TreeNode* currentNode);
		int getVarType(string name);
		void insertSymbolTable(string name, int incomingEntryType, int incomingDataType, TreeNode* parameterList, int incomingArrayMax);
		int checkForSymbol(string name);
		vector<Symbol> getGlobalTable();
		vector<Symbol> getLocalTable();
		void increaseBlockLevel();
		void decreaseBlockLevel();
		string enumToString(int enumValue);
		void printSymbolTable();

	private:
		vector<Symbol> localTable;
		vector<Symbol> globalTable;
		int globalBlockLevel;
		TreeNode* root;
		int insertOrCheck;
		ofstream symbolTableFile;
};

#endif
