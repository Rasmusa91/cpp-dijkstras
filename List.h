#ifndef LIST_H
#define LIST_H

template <typename T>
class List
{
private:
	class Node
	{
	public:
		T data;
		Node * next;

		Node(const T pData)
		{
			data = pData;
			next = nullptr;
		}
	};

	Node * mFirst;
	int mSize;

	void copyAll(const List<T> & pList);
	void freeAll();

public:
	List();
	List(const List<T> & pList);
	~List();
	List & operator=(const List<T> & pList);
	T operator[](const int pIndex) const;

	void insertAt(const int pos, const T element);
	T elementAt(const int pos) const;
	T removeAt(const int pos);
	int size() const;
};

template <typename T>
List<T>::List()
{
	mFirst = nullptr;
	mSize = 0;
}

template <typename T>
List<T>::List(const List<T> & pList)
{
	copyAll(pList);
}

template <typename T>
List<T>::~List()
{
	freeAll();
}

template <typename T>
List<T> & List<T>::operator=(const List & pList) 
{
	if (*this != pList)
	{
		freeAll();
		copyAll(pList);
	}

	return *this;
}

template <typename T>
T List<T>::operator[](const int pIndex) const
{
	return elementAt(pIndex);
}

template <typename T>
void List<T>::insertAt(const int pos, const T element)
{
	if (pos < 0 || pos > mSize) {
		throw "Exception: pos out of range";
	}

	if (mFirst == nullptr || pos == 0)
	{
		Node * temp = new Node(element);
		temp->next = mFirst;
		mFirst = temp;
	}
	else
	{
		Node * walker = mFirst;

		for (int i = 0; i < pos - 1; i++)
		{
			walker = walker->next;
		}

		Node * temp = new Node(element);
		temp->next = walker->next;
		walker->next = temp;
	}

	mSize++;
}

template <typename T>
T List<T>::elementAt(const int pos) const
{
	Node * walker = mFirst;

	if (pos < 0 || pos > mSize) {
		throw "Exception: pos out of range";
	}

	for (int i = 0; i < pos; i++)
	{
		walker = walker->next;
	}

	return walker->data;
}


template <typename T>
T List<T>::removeAt(const int pos)
{
	T removedValue = T();
	Node * removedNode = nullptr;

	if (pos < 0 || pos >= mSize) {
		throw "Exception: pos out of range";
	}

	if (pos == 0)
	{
		removedNode = mFirst;
		mFirst = mFirst->next;
	}
	else
	{
		Node * walker = mFirst;

		for (int i = 0; i < pos - 1; i++)
		{
			walker = walker->next;
		}

		removedNode = walker->next;
		walker->next = removedNode->next;
	}

	removedValue = removedNode->data;
	delete removedNode;
	mSize--;

	return removedValue;
}

template <typename T>
int List<T>::size() const
{
	return mSize;
}

template <typename T>
void List<T>::copyAll(const List<T> & pList)
{
	mSize = pList.mSize;
	mFirst = nullptr;

	if (pList.mFirst != nullptr)
	{
		mFirst = new node(pList.mFirst->data);

		Node * walker = pList.mFirst;
		Node * myWalker = mFirst;

		while (walker != nullptr)
		{
			walker = walker->next;
			myWalker->next = new Node(walker->data);
			myWalker = myWalker->next;
		}
	}
}

template <typename T>
void List<T>::freeAll()
{
	Node * walker = mFirst;

	while (walker != nullptr)
	{
		mFirst = mFirst->next;
		delete walker;

		walker = mFirst;
	}
}

#endif