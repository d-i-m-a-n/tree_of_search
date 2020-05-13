// tree_of_search.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TreeOfSearch.h"
#include "BalancedTreeOfSearch.h"
#include "BaseTree.h"
#include <iostream>

int main()
{
	int x[10] = {1,2,3,4,5,6,7,8,9,0};
	TreeOfSearch<int> tree(x, 10);
	TreeOfSearch<int> tree2(x[1]);
	BalancedTreeOfSearch<int> tree3(x[1]);
	BalancedTreeOfSearch<int> tree4(x[0]);

	tree3 = tree4;

	tree2 = tree;

	tree2.ClearTree();

	return 0;
}

