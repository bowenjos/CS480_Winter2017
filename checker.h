#ifndef _CHECKER_H
#define _CHECKER_H

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

/*****
 * checker.h
 * Author: Joshua Bowen
 * Purpose: This generates a symbol table that is used by the code generator (Which will not exist)
 * This file contains the symbol and table structs, as well as the checker class
*****/

typedef struct
{
	string name;
	int type;
	int size;
	Symbol* sibling;
} Symbol;

typedef struct
{
	Table* parent;
	Symbol* originSymbol;
	Table children[];
} SymbolTable;

class Checker
{
	public:
		Checker();
		~Checker();

		void insertSymbolTable(string name, int type, int size, SymbolTable currentTable);
		Symbol checkSymbolTable(Symbol symbol, SymbolTable currentTable);
	
	private:

		SymbolTable* globalTable;
		SymbolTable* currentTable;

};

