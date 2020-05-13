#pragma once
#include "TreeOfSearch.h"
#include <stack>

template<class X> class BalancedTreeOfSearch :public TreeOfSearch<X>
{
public:

	BalancedTreeOfSearch();

	BalancedTreeOfSearch(BalancedTreeOfSearch<X>&);

	BalancedTreeOfSearch(X* mas, int n);

	BalancedTreeOfSearch(X&);

	BalancedTreeOfSearch<X>& operator = (BalancedTreeOfSearch<X>&);

	bool AddNode(X&);

	bool DelNode(X&);

private:
	class BalancedNode : public TreeOfSearch<X>::BaseNode
	{
	public:
		BalancedNode(X&);

		BalancedNode(BalancedNode&);

		BalancedNode& operator = (BalancedNode&);

		int balance;
	};

	bool AddTo(X&, BalancedNode*&);

	BalancedNode* L_turn(BalancedNode*);

	BalancedNode* R_turn(BalancedNode*);

	BalancedNode* LR_turn(BalancedNode*);

	BalancedNode* RL_turn(BalancedNode*);
};

// BalancedNode

template<class X> BalancedTreeOfSearch<X>::BalancedNode::BalancedNode(X& data):BaseNode(data)
{
	balance = 0;
}

template<class X> BalancedTreeOfSearch<X>::BalancedNode::BalancedNode(BalancedNode& obj):BaseNode(obj)
{
	balance = obj.balance;
}

// ���������� � BaseNode
template<class X> typename BalancedTreeOfSearch<X>::BalancedNode& BalancedTreeOfSearch<X>::BalancedNode::operator = (BalancedNode& obj)
{
	balance = obj.balance;
	data = obj.data;

	if (obj.lt)
	{
		if (lt)
			*lt = *(obj.lt);
		else
			// ���������� ����� �������������� ����������, ����� ���������� ������� ��� BalancedNode
			lt = new BalancedNode(*((BalancedNode*)obj.lt));
	}
	else
		if (lt)
		{
			delete lt;
			lt = nullptr;
		}

	if (obj.rt)
	{
		if (rt)
			*rt = *(obj.rt);
		else
			rt = new BalancedNode(*((BalancedNode*)obj.rt));
	}
	else
		if (rt)
		{
			delete rt;
			rt = nullptr;
		}

	return *this;
}

// BalancedTreeOfSearch

template<class X> BalancedTreeOfSearch<X>::BalancedTreeOfSearch()
{
	root = nullptr;
}

template<class X> BalancedTreeOfSearch<X>::BalancedTreeOfSearch(X& data)
{
	root = new BalancedNode(data);
}

template<class X> BalancedTreeOfSearch<X>::BalancedTreeOfSearch(BalancedTreeOfSearch<X>& obj)
{
	root = new BalancedNode(obj.root);
}

template<class X> BalancedTreeOfSearch<X>::BalancedTreeOfSearch(X* mas, int n)
{

}

// ���������� � TreeOfSearch
template<class X> BalancedTreeOfSearch<X>& BalancedTreeOfSearch<X>::operator = (BalancedTreeOfSearch<X>& obj)
{
	if (obj.root)
	{
		if (root)
			// ���������� ����� �������������� ����������, ����� ���������� ������� ��� BalancedNode
			*((BalancedNode*)root) = *((BalancedNode*)obj.root);
		else
			root = new BalancedNode(*((BalancedNode*)obj.root));
	}
	else
		if (root)
		{
			delete root;
			root = nullptr;
		}
	return *this;
}

template<class X> bool BalancedTreeOfSearch<X>::AddNode(X& data)
{
	AddTo(data, (BalancedNode*)root);

	return false;
}

// ����������� �������, ���������� �������, ��� ����������� �� �������� true ��������, ��� ��������� �������� ������� � ������� �������, false ��������
// �������� ������� ������ ���������� ��������� �� ������� ������������ ���������, �� ���� �� ������������� � �����-���� �� ������ ������� +-2
// ����� ������������ ���������, ������������� ������� ������ �� �����
template<class X> bool BalancedTreeOfSearch<X>::AddTo(X& data, BalancedNode*& root)
{
	if (root)
	{
		// ��������� ������� � ����� ��� ������ ���������, ����� ������������ ������ �� ��������� �������, � ������� ������ ����������� �������
		if (root->data > data)
		{
			if (AddTo(data, root->lt))
				root->balance--;
			else
				return false;
		}
		else
			if (AddTo(data, root->rt))
				root->balance++;
			else
				return false;
		
		switch (root->balance)
		{
			// ���� ������ ���������, �� � ���������� ������������� ��� �� �����
		case 0:
			return false;
			// ���� ������ ������� +-2, �� �������� ������������ ������������ ������� �������
		case 2:
		{
			// ������� ������������� ���������
			if (root->rt->balance == 1)
				root = L_turn(root);
			else
				root = RL_turn(root);
			// ����� ���������� ������������ � ���������� ������ �� �������������
			return false;
		}
		case -2:
		{
			if (root->lt->balance == -1)
				root = R_turn(root);
			else
				root = LR_turn(root);

			return false;
		}
		// ������, ����� ������ ���� ����� +-1, �� ����� ������ ������������, �� ���������� ������������
		default:
			break;
		}
	}
	else
		root = new BalancedNode(data);
	return true;
}

template<class X> bool BalancedTreeOfSearch<X>::DelNode(X& data)
{
	// ���� ������, � ������� ������������ �������� ����������� ������
	std::stack<BalancedNode*> balance_recount;
	
	/*
	*del_node - ��������� �� ��������� �������
	**par_node - ��������� �� ��������� ����� ��������� �������
	*rep_node - ��������� �� ���������� �������
	*rep_par_node - ��������� �� �������� ����������� (�� ����� nullptr, ���� � del_node ���� ��� ���������)
	*current - ��� ��������� ������� ������� �������, � ������� �������� ������
	*current_child - ��� ��������� ������� �������� � ����� ��������� ��������� ������
	*/
	BalancedNode *del_node, **par_node, *rep_node = nullptr, *current = nullptr, *current_child = nullptr, *rep_par_node = nullptr;

	par_node = &((BalancedNode*)root);
	del_node = (BalancedNode*)root;

	// ���� ��������� �������, �������� � ���� ���������
	while (del_node && del_node->data != data)
	{
		balance_recount.push(*par_node);
		if (del_node->data > data)
		{
			parent_node = &(del_node->lt);
			del_node = del_node->lt;
		}
		else
		{
			parent_node = &(del_node->rt);
			del_node = del_node->rt;
		}
	}

	if (del_node)
	{
		// ���� �������, ������� ����� �������� ���������, ���������� � TreeOfSearch
		if (!del_node->lt)
		{
			rep_node = del_node->rt;
			current_child = del_node;
		}
		else
			if (!del_node->rt)
			{
				rep_node = del_node->lt;
				current_child = del_node;
			}
			else
			{
				rep_par_node = del_node;
				rep_node = del_node->lt;

				//  ���� ���������� ������� � ��������� � ���� ���������
				balance_recount.push(del_node);

				while (rep_node->rt)
				{
					rep_par_node = rep_node;
					rep_node = rep_node->rt;

					balance_recount.push(rep_par_node);
				}
				
				rep_node->rt = del_node->rt;

				current_child = rep_node;
			}



		// ������������� ������
		bool tree_balanced = false;
		
		current = balance_recount.top();
		while (!balance_recount.empty() && !tree_balanced)
		{
			// ���������, � ����� ��������� ���������� ������
			if (current->lt == current_child)
				current->balance++;
			else
				current->balance--;

			switch (current->balance)
			{
			// �������� �� �����, ��������� �� ���������� �������� � ������������� ������
			case 0:
			{
				current_child = current;
				current = balance_recount.top();
				balance_recount.pop();
				break;
			}
			// � ������ +-2 ������ �������� � ������ ������������� ������
			case 2:
			{
				// ������� ������������� ���������
				// ���� ������ ������ ��� 0, �� ����� �������� ������ ������ �� ���������
				if (root->rt->balance == 0)
				{
					root = L_turn(root);
					tree_balanced = true;
				}
				// � ��������� ������� ����� �������� ������ ����������, ������������� ������������� ������ ������
				else
					if (root->rt->balance == 1)
					{
						root = L_turn(root);
						current_child = current;
						current = balance_recount.top();
						balance_recount.pop();
					}
					else
					{
						root = RL_turn(root);
						current_child = current;
						current = balance_recount.top();
						balance_recount.pop();
					}
				break;
			}
			case -2:
			{
				if (root->lt->balance == 0)
				{
					root = R_turn(root);
					tree_balanced = true;
				}
				else
					if (root->lt->balance == 1)
					{
						root = R_turn(root);
						current_child = current;
						current = balance_recount.top();
						balance_recount.pop();
					}
					else
					{
						root = LR_turn(root);
						current_child = current;
						current = balance_recount.top();
						balance_recount.pop();
					}
				break;
			}
			// ������ +-1, ���� ����� ������ +-1, �� ���������� �������� �� �����
			default:
				tree_balanced = true;
				break;
			}
		}

		if (rep_par_node && rep_par_node != del_node)
		{
			rep_par_node->rt = rep_node->lt;
			rep_node->lt = del_node->lt;
		}

		*par_node = rep_node;
		del_node->lt = del_node->rt = nullptr;
		delete del_node;
		return true;
	}

	return false;
}

// turn_node - ��������� �� ������� ������������ ������� ����� ����������� �������
// ���������� ��������� �� �������, ������� ���� ���������� �� ����� turn_node
template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::L_turn(BalancedNode* turn_node)
{
	BalancedNode *s = turn_node->rt;
	turn_node->rt = s->lt;
	s->lt = turn_node;

	// ����� ������
	if (turn_node->rt)
		turn_node->balance = 1;
	else
		turn_node->balance = 0;
	s->balance = 0;
	
	return s;
}

template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::R_turn(BalancedNode* turn_node)
{
	BalancedNode *s = turn_node->lt;
	turn_node->lt = s->rt;
	s->rt = turn_node;
	
	// ����� ������
	if (turn_node->lt)
		turn_node->balance = -1;
	else
		turn_node->balance = 0;
	s->balance = 0;
	
	return s;
}

template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::LR_turn(BalancedNode* turn_node)
{
	BalancedNode *s, *p;
	s = turn_node->lt;
	p = s->rt;

	// L_turn ������������ s
	s->rt = p->lt;
	p->lt = s;

	// R_turn ������������ turn_node
	turn_node->lt = p->rt;
	p->rt = turn_node;

	// ����� ������
	turn_node->balance = s->balance = 0;
	if (p->balance = -1)
		turn_node->balance = 1;
	if (p->balance = 1)
		s->balance = -1;

	p->balance = 0;

	return p;
}

template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::RL_turn(BalancedNode* turn_node)
{
	BalancedNode *s, *p;
	s = turn_node->rt;
	p = s->lt;

	// R_turn ������������ s
	s->lt = p->rt;
	p->rt = s;

	// L_turn ������������ turn_node
	turn_node->rt = p->lt;
	p->lt = turn_node;

	// ����� ������
	turn_node->balance = s->balance = 0;
	if (p->balance = -1)
		s->balance = 1;
	if (p->balance = 1)
		turn_node->balance = -1;

	p->balance = 0;

	return p;
}