#pragma once
#include <string>

using namespace std;

sql::Driver *driver=nullptr;
sql::Connection *con=nullptr;
sql::Statement *stmt = nullptr;

class Relationship {
private:

public:
	virtual string insertTuple() = 0;
};

void connectToDatabase(string address, string schema, string username, string password) {
	try {
		driver = get_driver_instance();
		con = driver->connect(address, username, password);
		con->setSchema(schema);
		stmt = con->createStatement();
	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

//Remember to delete when finished!!!
sql::ResultSet* executeQuery(string query) {
	try {
		return stmt->executeQuery(query);
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	return nullptr;
}

bool isSQLConnected() {
	return con != nullptr;
}

void executeSQL(string statement) {
	try {
		stmt->execute(statement);
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

void disconnectFromDatabase() {
	delete stmt;
	delete con;
}

