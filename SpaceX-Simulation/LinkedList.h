#pragma once

template<class T> struct Node {
	Node<T>* next;
	T data;
	Node(const T &initialData) {
		next = nullptr;
	}
};

template<class T> class LinkedListIterator {
	Node<T>* currentNode;
	void next() {
		if(hasNext())
			currentNode = currentNode->next;
	}
public:
	LinkedListIterator(Node<T>* ptr) {
		currentNode = ptr;
	}
	T& operator*() {
		return currentNode->data;
	}
	T* operator&() {
		return &currentNode->data;
	}
	bool hasNext() {
		return currentNode!=nullptr && currentNode->next != nullptr;
	}
	LinkedListIterator<T> operator+(int offset) {
		LinkedListIterator<T> result(currentNode);
		for (int i = 0; i < offset; i++) {
			result.next();
		}
		return result;
	}
	void operator++() {
		this->next();
	}
};

template<class T> class LinkedList {
	Node<T>* Head;
	Node<T>* Tail;
	int size;
public:
	LinkedList() {
		Head = nullptr;
		Tail = nullptr;
		size = 0;
	}
	void push_back(const T &item) {
		Node<T>* newNode = new Node<T>(item);
		if (size == 0) {
			Head = newNode;
		}
		else {
			Tail->next = newNode;
		}
		Tail = newNode;
		size++;
	}
	T* operator[](int c) {
		if (c >= size || c < 0) return nullptr;
		Node<T>* currentNode = Head;
		for (int i = 0; i < c; i++) {
			currentNode = currentNode->next;
		}
		return &(currentNode->data);
	}
	T* back() {
		if (size == 0) return nullptr;
		return &(Tail->data);
	}
	LinkedListIterator<T> begin() {
		LinkedListIterator<T> result(Head);
		return result;
	}
	int Size() {
		return size;
	}
	void deleteAll() {
		Node<T>* ptr = Head;
		if (size == 0) return;
		while (ptr->next != nullptr) {
			Node<T>* nodeToDelete = ptr;
			ptr = ptr->next;
			delete nodeToDelete;
		}
		delete ptr;
		Head = nullptr;
		Tail = nullptr;
		size = 0;
	}
};