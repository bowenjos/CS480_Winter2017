#ifndef _TREENODE_H
#define _TREENODE_H

#include <string>

using namespace std;

/*****
 * treenode.h
 * Author: Joshua Bowen
 * Purpose: This object represents a singular node in a Parse Tree.
 * This node is referenced and stores data given to it by the Parser
*****/

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

#endif
