#pragma once
#include <stdio.h>
#include <time.h>
#include <random>

//Linked List implementation
template<class T> struct Node {
	Node<T>* next;
	T data;
	Node(const T &initialData) {
		next = nullptr;
		data = initialData;
	}
};

template<class T> class LinkedListIterator {
	Node<T>* currentNode;
	void next() {
		if (hasNext())
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
		return currentNode != nullptr;
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
		newNode->next = nullptr;
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
//End Linked List implementation

string to_string(char* c, int size) {
	string result(c, size);
	if (size > 0 && c[0] == '\0') return NullIndicator;
	return result;
}
string to_string(char* c) {
	string result(c);
	if (c[0] == '\0') return NullIndicator;
	return result;
}
string to_string(char c) {
	string result = "";
	result.push_back(c);
	if (c == '\0') return NullIndicator;
	return result;
}
string V() {
	return to_string(ValueDeliminator);
}
string T() {
	return to_string(TupleDeliminator);
}
string sanitize(string dirty) {
	string result = "";
	for (int i = 0; i < dirty.size(); i++) {
		if (dirty[i] == '\'') {
			result.push_back('\'');
			result.push_back('\'');
		}
		else {
			result.push_back(dirty[i]);
		}
	}
	return result;
}
void strcpy(string src, char* dest, int n, bool varlength = true) {
	int end = varlength ? n - 1 : n;
	for (int i = 0; i < end; i++) {
		if (i < src.size()) {
			dest[i] = src[i];
		}
		else {
			dest[i] = '\0';
		}
	}
	if (varlength)
		dest[n - 1] = '\0';
}
void strmov(const char* src, char* dest, int n) {
	for (int i = 0; i < n; i++) {
		dest[i] = src[i];
	}
}
bool stringcmp(string a, char* b, int n, bool varlength = true) {
	for (int i = 0; i < n; i++) {
		if (varlength && (i >= a.size() || i == n - 1) && b[i] == '\0') return true;
		if (!varlength&&i == n - 1 && a.size() == n&&a[i] == b[i]) return true;
		if (i >= a.size() || a[i] != b[i]) return false;
	}
	return false;
}
string boolToString(int boolean) {
	if (boolean == 1) return "true";
	else if (boolean == 0) return "false";
	else return "NULL";
}
string boolToListString(int boolean) {
	if (boolean == 1) return "1";
	else if (boolean == 0) return "0";
	else return NullIndicator;
}

int getDaysInMonth(int month, int year) {
	if (month != 2) {
		if (month < 8) {
			if (month % 2 == 0) {
				return 30;
			}
			else {
				return 31;
			}
		}
		else {
			if (month % 2 == 0) {
				return 31;
			}
			else {
				return 30;
			}
		}
	}
	else {
		if (year % 4 == 0 && (year % 100 != 0 || (year % 100 == 0 && year % 400 == 0))) { //If it is a leap year
			return 29;
		}
		else {
			return 28;
		}
	}
}
struct Date { //Meta-Structure for holding dates
	int day;
	int month;
	int year; //Such as: 2018
	Date() {}
	Date(string date) { //Format YYYY-MM-DD
		if (date.size() == 10) {
			year = atoi(date.substr(0, 4).c_str());
			month = atoi(date.substr(5, 2).c_str());
			day = atoi(date.substr(8, 2).c_str());
		}
		else {
			year = 0;
			month = 0;
			day = 0;
		}
	}
	Date(int year, int month, int day) {
		this->year = year;
		this->month = month;
		this->day = day;
	}
	Date operator+(int days) { //Advance time by "days" days
		Date result = *this;
		for (int i = 0; i < days; i++) {
			result.day++;
			if (result.day > getDaysInMonth(result.month, result.year)) {
				result.day = 1;
				result.month++;
				if (result.month > 12) {
					result.month = 1;
					result.year++;
				}
			}
		}
		return result;
	}
	void operator+=(int days) {
		Date result = operator+(days);
		day = result.day;
		month = result.month;
		year = result.year;
	}
	bool operator==(Date other) {
		return year == other.year&&month == other.month&&day == other.day;
	}
	bool operator!=(Date other) {
		return !operator==(other);
	}
	bool operator>(Date other) {
		if (year > other.year) {
			return true;
		}
		else if (year < other.year) {
			return false;
		}
		else {
			if (month > other.month) {
				return true;
			}
			else if (month < other.month) {
				return false;
			}
			else {
				if (day > other.day) {
					return true;
				}
				else if (day < other.day) {
					return false;
				}
				else {
					return false;
				}
			}
		}
	}
	int operator-(Date other) { //Get number of days between this date and other date
		int result = 0;
		if (operator>(other)) {
			while (other != *this) {
				result++;
				other += 1;
			}
		}
		else {
			Date temp = *this;
			while (temp != other) {
				result++;
				temp += 1;
			}
		}
		return result;
	}
};
string padWithZeros(string str, int width) {
	if (str.size() < width) {
		int zeros = width - str.size();
		for (int i = 0; i < zeros; i++) {
			str = '0' + str;
		}
	}
	return str;
}
string to_string(Date date) {
	string result;
	string sYear = to_string(date.year);
	sYear = padWithZeros(sYear, 4);
	string sMonth = to_string(date.month);
	sMonth = padWithZeros(sMonth, 2);
	string sDay = to_string(date.day);
	sDay = padWithZeros(sDay, 2);
	result = sYear + "-" + sMonth + "-" + sDay;
	return result;
}

const string getCurrentDate() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return buf;
}

//Used for interacting with Curl to download SpaceX api responses
string executeSystemCommand(string cmd) {
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");
	stream = _popen(cmd.c_str(), "r");
	if (stream) {
		while (!feof(stream)) {
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		}
		_pclose(stream);
	}
	return data;
}

bool chanceOutOf1000(int n, default_random_engine &e) {
	uniform_int_distribution<int> dist(1, 1000);
	int guess = dist(e);
	return guess <= n;
}

bool chanceOutOf30(int n, default_random_engine &e) {
	uniform_int_distribution<int> dist(1, 30);
	int guess = dist(e);
	return guess <= n;
}