// tree_of_search.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TreeOfSearch.h"
#include "BalancedTreeOfSearch.h"
#include "BaseTree.h"
#include <iostream>

int main()
{
	int x[9] = {50,20,60,70,10,40,5,30,45};
	TreeOfSearch<int> tree(x, 10);
	TreeOfSearch<int> tree2(x[1]);
	BalancedTreeOfSearch<int> tree3(x,9);
	BalancedTreeOfSearch<int> tree4(x[0]);

	int y = 25;
	tree3.AddNode(y);
	y = 18;
	tree3.AddNode(y);
	y = 40;
	tree3.DelNode(y);

	tree3 = tree4;

	tree2 = tree;

	tree2.ClearTree();

	return 0;
}

