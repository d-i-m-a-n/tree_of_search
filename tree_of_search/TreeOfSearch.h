#pragma once
#include "BaseTree.h"

template<class X> class TreeOfSearch :public BaseTree<X>
{
public:

	TreeOfSearch();

	TreeOfSearch(TreeOfSearch<X>&);

	TreeOfSearch(X&);

	TreeOfSearch(X*, int);

	TreeOfSearch<X>& operator = (TreeOfSearch<X>&);

	virtual bool AddNode(X&);

	virtual bool DelNode(X&);

	// функции поиска не виртуальные, т.к. для всех деревьев поиска одинаковые
	bool FindNode(X&);

	X FindMax();

	X FindMin();

private:
    typename TreeOfSearch<X>::BaseNode** SearchNode(X&, typename TreeOfSearch<X>::BaseNode*&);
};

template<class X> TreeOfSearch<X>::TreeOfSearch(): TreeOfSearch<X>::BaseTree()
{

}

template<class X> TreeOfSearch<X>::TreeOfSearch(TreeOfSearch& obj): TreeOfSearch<X>::BaseTree(obj)
{

}

template<class X> TreeOfSearch<X>::TreeOfSearch(X& data) : TreeOfSearch<X>::BaseTree(data)
{

}

template<class X> TreeOfSearch<X>::TreeOfSearch(X* mas, int n)
{
    BaseTree<X>::root = nullptr;

	if (mas)
	{
		for (int i = 0; i < n; i++)
		{
            typename TreeOfSearch<X>::BaseNode** addto = &BaseTree<X>::root;

			while (*addto)
			{
				if ((*addto)->data > mas[i])
					addto = &((*addto)->lt);
				else
					addto = &((*addto)->rt);
			}

			*addto = new typename TreeOfSearch<X>::BaseNode(mas[i]);
		}

	}
}

template<class X> TreeOfSearch<X>& TreeOfSearch<X>::operator = (TreeOfSearch<X>& obj)
{
	if (obj.root)
	{
        if (BaseTree<X>::root)
            *BaseTree<X>::root = *(obj.root);
		else
            BaseTree<X>::root = new typename TreeOfSearch<X>::BaseNode(*(obj.root));
	}
	else
        if (BaseTree<X>::root)
		{
            delete BaseTree<X>::root;
            BaseTree<X>::root = nullptr;
		}

	return *this;
}

template<class X> bool TreeOfSearch<X>::AddNode(X& data)
{
    typename TreeOfSearch<X>::BaseNode** addto = &BaseTree<X>::root;

	while (*addto)
	{
		if ((*addto)->data > data)
			addto = &((*addto)->lt);
		else
			addto = &((*addto)->rt);
	}

	*addto = new typename TreeOfSearch<X>::BaseNode(data);
	if(*addto)
		return true;
	return false;
}

template<class X> bool TreeOfSearch<X>::DelNode(X& data)
{
	typename TreeOfSearch<X>::BaseNode *del_node, **par_node, *rep_node;
	del_node = rep_node = nullptr;
	par_node = SearchNode(data, del_node);

	if (del_node)
	{
		if (!del_node->lt)
			rep_node = del_node->rt;
		else
			if (!del_node->rt)
				rep_node = del_node->lt;
			else
			{
				typename TreeOfSearch<X>::BaseNode *rep_par_node = del_node;
				rep_node = del_node->lt;

				while (rep_node->rt)
				{
					rep_par_node = rep_node;
					rep_node = rep_node->rt;
				}


				if (del_node != rep_par_node)
				{
					rep_par_node->rt = rep_node->lt;
					rep_node->lt = del_node->lt;
				}

				rep_node->rt = del_node->rt;
			}

		*par_node = rep_node;
		del_node->lt = del_node->rt = nullptr;
		delete del_node;
		return true;
	}
	return false;
}

template<class X> bool TreeOfSearch<X>::FindNode(X& data)
{
    typename TreeOfSearch<X>::BaseNode* ptr = BaseTree<X>::root;

	while (ptr && ptr->data != data)
	{
		if (ptr->data > data)
			ptr = ptr->lt;
		else
			ptr = ptr->rt;
	}

	if (!ptr)
		return false;
	return true;
}

template<class X> X TreeOfSearch<X>::FindMax()
{
	X max;
	typename TreeOfSearch<X>::BaseNode* ptr = nullptr;
    if (BaseTree<X>::root)
	{
        ptr = BaseTree<X>::root;
		max = ptr->data;
		while (ptr->rt)
		{
			ptr = ptr->rt;
			max = ptr->data;
		}
	}
	return max;
}

template<class X> X TreeOfSearch<X>::FindMin()
{
	X min;
	typename TreeOfSearch<X>::BaseNode* ptr = nullptr;
    if (BaseTree<X>::root)
	{
        ptr = BaseTree<X>::root;
		min = ptr->data;
		while (ptr->lt)
		{
			ptr = ptr->lt;
			min = ptr->data;
		}
	}
	return min;
}

template<class X> typename TreeOfSearch<X>::BaseNode** TreeOfSearch<X>::SearchNode(X& data, typename TreeOfSearch<X>::BaseNode*& found_node)
{
    typename TreeOfSearch<X>::BaseNode** parent_node = &BaseTree<X>::root;
    found_node = BaseTree<X>::root;

	while(found_node && found_node->data != data)
	{
		if (found_node->data > data)
		{
			parent_node = &(found_node->lt);
			found_node = found_node->lt;
		}
		else
		{
			parent_node = &(found_node->rt);
			found_node = found_node->rt;
		}
	}

	if (!found_node)
		parent_node = nullptr;

	return parent_node;
}
