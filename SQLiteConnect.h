/*
 * SQLiteConnect.h
 *
 *  Created on: 27.12.2014
 *      Author: vaphen
 */

#ifndef SQLITECONNECT_H_
#define SQLITECONNECT_H_

#include <string>
#include <vector>
#include <iostream>
#include <sqlite3.h>

class SQLiteConnect {
public:
	SQLiteConnect();
	virtual ~SQLiteConnect();

	void addNewLesson(std::string);
	void deleteLesson(std::string);
	std::vector<std::string> getLessons();
private:
	sqlite3 *database;
	sqlite3_stmt *queryStatement;
	bool open_db(std::string);
	void close_db();
};

#endif /* SQLITECONNECT_H_ */
