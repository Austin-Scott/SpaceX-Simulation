#pragma once
#include <string>

using namespace std;

sql::Driver *driver=nullptr;
sql::Connection *con=nullptr;
sql::Statement *stmt = nullptr;

void printSQLError(sql::SQLException &e) {
	cout << "# ERR: " << e.what();
	cout << " (MySQL error code: " << e.getErrorCode();
	cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}

void connectToDatabase(string address, string schema, string username, string password) {
	try {
		driver = get_driver_instance();
		con = driver->connect(address, username, password);
		con->setSchema(schema);
		stmt = con->createStatement();
	} catch (sql::SQLException &e) {
		printSQLError(e);
	}
}

bool isSQLConnected() {
	return con != nullptr;
}

void executeSQL(string statement) {
	try {
		stmt->execute(statement);
	}
	catch (sql::SQLException &e) {
		printSQLError(e);
	}
}

void disconnectFromDatabase() {
	delete stmt;
	delete con;
}

string dropTable(string name) {
	return "DROP TABLE IF EXISTS " + name;
}

string dropTuples(string name) {
	return "DELETE FROM " + name;
}

string insertTuples(string table, string tuples) {
	return "INSERT INTO " + table + " VALUES " + tuples;
}

string createTable(string name, string structure) {
	return "CREATE TABLE " + name + " " + structure;
}