#pragma once
#include <string>

using namespace std;

class Relationship {
private:
	static string relationshipName;
public:
	Relationship(string relationName) {
		relationshipName = relationName;
	}
	static string getRelationName() {
		return relationshipName;
	}
	virtual string insertTuple() = 0;
	virtual string createTable() = 0;
};