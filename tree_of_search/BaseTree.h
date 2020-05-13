#pragma once
#include <stack>

template<class X> class BaseTree
{
protected:
	class BaseNode
	{
	public:
		BaseNode(X&);

		BaseNode(BaseNode&);

		virtual ~BaseNode();
		
		BaseNode& operator = (BaseNode& obj);

		X data;
		BaseNode* lt;
		BaseNode* rt;
	};

    BaseNode* root;
    BaseNode* iterator;
	// стек для организации обхода
    std::stack<BaseNode*> r_nodes;

public:
	BaseTree();

	BaseTree(BaseTree<X>& obj);

	BaseTree(X&);

	virtual ~BaseTree();

	virtual bool AddNode(X&) = 0;

	virtual bool DelNode(X&) = 0;

	virtual bool FindNode(X&) = 0;

	virtual X FindMax() = 0;

	virtual X FindMin() = 0;

    virtual int Height() = 0;

    bool GetData(X&);

    bool Next();

    bool MoveToRoot();

	void ClearTree();
};

// BaseNode
template<class X> BaseTree<X>::BaseNode::BaseNode(X& Data)
{
	data = Data;
    rt = lt = nullptr;
}

// рекурсивно копируются все вершины, с сохранением связей
template<class X> BaseTree<X>::BaseNode::BaseNode(BaseNode& obj)
{
	data = obj.data;
	lt = rt = nullptr;

	if (obj.lt)
    {
		lt = new BaseNode(*(obj.lt));
	}
    if (obj.rt)
    {
		rt = new BaseNode(*(obj.rt));
    }
}

// рекурсивно удаляем все вершины
template<class X> BaseTree<X>::BaseNode::~BaseNode()
{
	if (lt)
		delete lt;
	if (rt)
		delete rt;
    lt = rt = nullptr;
}

template<class X> typename BaseTree<X>::BaseNode& BaseTree<X>::BaseNode::operator = (BaseNode& obj)
{
	data = obj.data;

	// проверяем наличие левой части для копирования
	if (obj.lt)
	{
		// смотрим, есть ли готовая вершина, чтобы не делать лишние операции удаления и создания вершины
		if (lt)
			*lt = *(obj.lt);
		else
			lt = new BaseNode(*(obj.lt));
	}
	else
		// если нечего копировать, то удаляем все, что слева
		if (lt)
		{
			delete lt;
			lt = nullptr;
		}
	// аналогично
	if (obj.rt)
	{
		if (rt)
			*rt = *(obj.rt);
		else
			rt = new BaseNode(*(obj.rt));
	}
	else
		if (rt)
		{
			delete rt;
			rt = nullptr;
		}
	return *this;
}

// BaseTree
template<class X> BaseTree<X>::BaseTree()
{
	root = nullptr;
    iterator = nullptr;
    r_nodes.push(nullptr);
}

template<class X> BaseTree<X>::BaseTree(BaseTree<X>& obj)
{
	root = new BaseNode(obj.root);
    iterator = root;
    r_nodes.push(nullptr);
}

template<class X> BaseTree<X>::BaseTree(X& data)
{
	root = new BaseNode(data);
    iterator = root;
    r_nodes.push(nullptr);
}

template<class X> BaseTree<X>::~BaseTree()
{
	if (root)
		delete root;
    root = iterator = nullptr;
}

template<class X> void BaseTree<X>::ClearTree()
{
	if (root)
        delete root;
	while (r_nodes.top())
		r_nodes.pop();
    root = iterator = nullptr;
}

template<class X> bool BaseTree<X>::GetData(X& data)
{
	if (iterator)
	{
		data = iterator->data;
		return true;
	}
	return false
}

// обход Node-Left-Right
// вообще можно сделать указатель на функцию для обхода и в приватной зоне описать все 3 обхода, а для выбора отдельную функцию вынести
template<class X> bool BaseTree<X>::Next()
{
	if (iterator)
	{
		if (iterator->rt)
			r_nodes.push(iterator->rt);
		if (iterator->lt)
			iterator = iterator->lt;
		else
			iterator = r_nodes.top();

		if (iterator)
		{
			r_nodes.pop();
			return true;
		}
	}
    return false;
}

template<class X> bool BaseTree<X>::MoveToRoot()
{
    while(r_nodes.top())
        r_nodes.pop();
    iterator = root;
    if(iterator)
        return true;
    return false;
}