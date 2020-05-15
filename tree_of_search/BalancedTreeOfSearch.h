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

// аналогично с BaseNode
template<class X> typename BalancedTreeOfSearch<X>::BalancedNode& BalancedTreeOfSearch<X>::BalancedNode::operator = (BalancedNode& obj)
{
    balance = obj.balance;
    data = obj.data;

    if (obj.lt)
    {
        if (lt)
            *lt = *(obj.lt);
        else
            // делаем явное преобразование в BalancedNode* т.к. левый и правый указатели на BaseNode
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
    if(mas)
        for (int i = 0; i < n; i++)
            AddNode(mas[i]);
    else
        root = nullptr;
}

// аналогично с TreeOfSearch
template<class X> BalancedTreeOfSearch<X>& BalancedTreeOfSearch<X>::operator = (BalancedTreeOfSearch<X>& obj)
{
    if (obj.root)
    {
        if (root)
            // делаем явное преобразование в BalancedNode* т.к. корень указатель на BaseNode
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
    AddTo(data, (BalancedNode*&)root);

    return false;
}

// рекурсивная функция, при возвращении из рекурсии true означает, что в текущей вершине нужен пересчет баланса, false - не нужен
// пересчитываем баланс пока в одной из вершин он не станет 0, либо +-2
// в случае +-2 делаем перебалансировку и дальше баланс не пересчитываем
template<class X> bool BalancedTreeOfSearch<X>::AddTo(X& data, BalancedNode*& root)
{
    if (root)
    {
        // проверяем в какое поддерево надо добавть вершину и сразу пересчитываем баланс
		if (root->data > data)
        {
            if (AddTo(data, (BalancedNode*&)(root->lt)))
                root->balance--;
            else
                return false;
        }
        else
            if (AddTo(data, (BalancedNode*&)(root->rt)))
                root->balance++;
            else
                return false;

        switch (root->balance)
        {
            // если баланс стал равен нулю, то заканчиваем пересчет
        case 0:
            return false;
            // если баланс стал равен +-2, то необходимо выполнить перебалансировку
        case 2:
        {
            // правило выполнения поворотов
			// если справа 1, то делаем левый поворот, если справа -1, то делаем правый-левый поворот
			// случая, когда справа 0 при добавлении вершины быть не может
            if (((BalancedNode*)(root->rt))->balance == 1)
                root = L_turn(root);
            else
                root = RL_turn(root);
            // закончили перебалансировку, вернули false
            return false;
        }
        case -2:
        {
            if (((BalancedNode*)(root->lt))->balance == -1)
                root = R_turn(root);
            else
                root = LR_turn(root);

            return false;
        }
        // случай +-1, перебалансировка не нужна, но баланс продолжаем пересчитывать
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
    // стек с вершинами, в которых потенциально придется пересчитывать баланс
	// двойной указатель нужен при поворотах
    std::stack<BalancedNode**> balance_recount;
    balance_recount.push(nullptr);

    /*
    *del_node - удаляемая вершина
    **par_node - отец удаляемой
    *rep_node - замещающая вершина
    *rep_par_node - отец замещающей вершины
    *current - при пересчете баланса, текущая вершина, в которой надо пересчитать баланс
    *current_child - при пересчете баланса позволит узнать в каком поддереве изменилась высота
    */
    BalancedNode *del_node, **par_node, *rep_node = nullptr, **current = nullptr, *current_child = nullptr, *rep_par_node = nullptr;

    par_node = ((BalancedNode**)&root);
    del_node = (BalancedNode*)root;

    // ищем удаляемую вершину, как в методе SearchNode, но параллельно добавляем в стек родителей
    while (del_node && del_node->data != data)
    {
        balance_recount.push(par_node);
        if (del_node->data > data)
        {
            par_node = ((BalancedNode**)&(del_node->lt));
            del_node = (BalancedNode*)(del_node->lt);
        }
        else
        {
            par_node = ((BalancedNode**)&(del_node->rt));
            del_node = (BalancedNode*)(del_node->rt);
        }
    }

    if (del_node)
    {
        // поиск замещающей вершины, аналогично с TreeOfSearch, но добавляется инициализация current_child
        if (!del_node->lt)
        {
            rep_node = (BalancedNode*)(del_node->rt);
            current_child = del_node;
        }
        else
            if (!del_node->rt)
            {
                rep_node = (BalancedNode*)(del_node->lt);
                current_child = del_node;
            }
            else
            {
                rep_par_node = del_node;
                rep_node = (BalancedNode*)(del_node->lt);

                //  ищем замещающий элемент и добавляем родителей
                balance_recount.push(&del_node);

                while (rep_node->rt)
                {
                    rep_par_node = rep_node;
                    rep_node = (BalancedNode*)(rep_node->rt);

                    balance_recount.push(&rep_par_node);
                }

                rep_node->rt = del_node->rt;

                current_child = rep_node;
            }



        // пересчитываем баланс
        // флаг, что при очередном использовании поворотов установился баланс
		bool tree_balanced = false;

        current = balance_recount.top();
        balance_recount.pop();
		// либо деверо было сбалансировано после поворотов, либо дазбалансных ситуаций во всем дереве не произошло
        while (!balance_recount.empty() && !tree_balanced)
        {
            // смотри в каком поддереве изменилась высота
            if ((*current)->lt == current_child)
                (*current)->balance++;
            else
                (*current)->balance--;

            switch ((*current)->balance)
            {
            // если баланс стал 0, то повороты не нужны, но баланс продолжаем пересчитывать
            case 0:
            {
                current_child = *current;
                current = balance_recount.top();
                balance_recount.pop();
                break;
            }
            // в случае +-2 необходимо использовать повороты
			// в 1 из 3 случаев продолжать пересчет балансов не нужно
            case 2:
            {
                // правило использования поворотов
                // если баланс справа 0, то необходимо выполнить левый поворот, но продолжать пересчет баланса не нужно
                if (((BalancedNode*)(*current)->rt)->balance == 0)
                {
                    *current = L_turn(*current);
                    tree_balanced = true;
                }
                // в случаях справа +-1, необходимо выполнить повороты и продолжить пересчет балансов
                else
                    if (((BalancedNode*)(*current)->rt)->balance == 1)
                    {
                        *current = L_turn(*current);
                        current_child = *current;
                        current = balance_recount.top();
                        balance_recount.pop();
                    }
                    else
                    {
                        *current = RL_turn(*current);
                        current_child = *current;
                        current = balance_recount.top();
                        balance_recount.pop();
                    }
                break;
            }
            case -2:
            {
                if (((BalancedNode*)(*current)->lt)->balance == 0)
                {
                    *current = R_turn(*current);
                    tree_balanced = true;
                }
                else
                    if (((BalancedNode*)(*current)->lt)->balance == -1)
                    {
                        *current = R_turn(*current);
                        current_child = *current;
                        current = balance_recount.top();
                        balance_recount.pop();
                    }
                    else
                    {
                        *current = LR_turn(*current);
                        current_child = *current;
                        current = balance_recount.top();
                        balance_recount.pop();
                    }
                break;
            }
            // случай +-1, дальше пересчитывать балансы не нужно
            default:
                tree_balanced = true;
                break;
            }
        }

		// т.к. для пересчета баланса необходимо было сохранить связи, поэтому делаем перенос указателей после балансировки
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

// turn_node - вершина, относительно которой будем производить поворот
// возвращаем указатель на вершину, которая будет на месте turn_node после пооротов
// баланс во всех вершинах считается здесь же
template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::L_turn(BalancedNode* turn_node)
{
    BalancedNode *s = (BalancedNode*)(turn_node->rt);
    turn_node->rt = (BalancedNode*)(s->lt);
    s->lt = turn_node;

    // ????? ??????
    if (turn_node->rt && !turn_node->lt)
    {
        turn_node->balance = 1;
        s->balance = -1;
    }
    else
    {
        turn_node->balance = 0;
        s->balance = 0;
    }
    return s;
}

template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::R_turn(BalancedNode* turn_node)
{
    BalancedNode *s = (BalancedNode*)(turn_node->lt);
    turn_node->lt = (BalancedNode*)(s->rt);
    s->rt = turn_node;

    // ????? ??????
    if (turn_node->lt && !turn_node->rt)
    {
        turn_node->balance = -1;
        s->balance = 1;
    }
    else
    {
        turn_node->balance = 0;
        s->balance = 0;
    }
    return s;
}

template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::LR_turn(BalancedNode* turn_node)
{
    BalancedNode *s, *p;
    s = (BalancedNode*)(turn_node->lt);
    p = (BalancedNode*)(s->rt);

    // L_turn ???????????? s
    s->rt = (BalancedNode*)(p->lt);
    p->lt = s;

    // R_turn ???????????? turn_node
    turn_node->lt = p->rt;
    p->rt = turn_node;

    // ????? ??????
    turn_node->balance = s->balance = 0;
    if (p->balance == -1)
        turn_node->balance = 1;
    if (p->balance == 1)
        s->balance = -1;

    p->balance = 0;

    return p;
}

template<class X> typename BalancedTreeOfSearch<X>::BalancedNode* BalancedTreeOfSearch<X>::RL_turn(BalancedNode* turn_node)
{
    BalancedNode *s, *p;
    s = (BalancedNode*)(turn_node->rt);
    p = (BalancedNode*)(s->lt);

    // R_turn ???????????? s
    s->lt = p->rt;
    p->rt = s;

    // L_turn ???????????? turn_node
    turn_node->rt = p->lt;
    p->lt = turn_node;

    // ????? ??????
    turn_node->balance = s->balance = 0;
    if (p->balance == -1)
        s->balance = 1;
    if (p->balance == 1)
        turn_node->balance = -1;

    p->balance = 0;

    return p;
}
