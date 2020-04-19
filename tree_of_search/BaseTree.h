#pragma once

template<class X> class BaseTree
{
public:

	class Node
	{
	public:
		Node(X&);

		Node(Node&);

		~Node();

	private:
		X data;
		Node* lt;
		Node* rt;
	};

	BaseTree();

	BaseTree(BaseTree&);

	virtual bool AddNode(X&) = 0;

	virtual bool DelNode(X&) = 0;

	virtual bool FindNode(X&) = 0;

	virtual X FindMax() = 0;

	virtual X FindMin() = 0;

	virtual void ClearTree() = 0;

	~BaseTree();

protected:

	Node* root;
};

template<class X> BaseTree<X>::Node::Node(X& DATA)
{
	data = DATA;
	lt = rt = nullptr;
}

template<class X> BaseTree<X>::Node::Node(Node& obj)
{
	data = obj.data;
	lt = rt = nullptr;

	if (obj.lt)
		lt = new Node(obj.lt);

	if (obj.rt)
		rt = new Node(obj.rt);
}

template<class X> BaseTree<X>::Node::~Node()
{
	if (lt)
		delete lt;
	if (rt)
		delete rt;
}

template<class X> BaseTree<X>::BaseTree()
{
	//printf_s("constr tree\n");
	root = nullptr;
}

template<class X> BaseTree<X>::BaseTree(BaseTree& obj)
{
	root = new Node(obj.root);
}

template<class X> BaseTree<X>::~BaseTree()
{
	//printf_s("destr tree\n");
	if(root)
		delete root;
}