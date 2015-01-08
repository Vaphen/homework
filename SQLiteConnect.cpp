/*
 * SQLiteConnect.cpp
 *
 *  Created on: 27.12.2014
 *      Author: vaphen
 */

#include "SQLiteConnect.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

/*
 * TODO: Add special error for detailed feedback (e.g. PREPARE_ERROR)
 */

SQLiteConnect::SQLiteConnect() :
		database(nullptr),
		queryStatement(nullptr),
		isDbOpen(false){
}

SQLiteConnect::~SQLiteConnect() {
	sqlite3_close(database);
}

bool SQLiteConnect::open_db(std::string dbName) {
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
								  Database::LESSON_TABLE +
								  "(id INTEGER PRIMARY KEY, " +
								  Database::LESSON_TABLE_LESSON_COLUMN +
								  " TEXT);";

	if(sqlite3_open(dbName.c_str(), &database) != SQLITE_OK) {
		return Database::ERROR;
	}
	if(sqlite3_prepare_v2(database, createQuery.c_str(), -1, &queryStatement, 0) != SQLITE_OK) {
		return Database::ERROR;
	}
	if(sqlite3_step(queryStatement) != SQLITE_DONE) {
		return Database::ERROR;
	}

	sqlite3_finalize(queryStatement);
	isDbOpen = true;
	return Database::SUCCESS;
}

void SQLiteConnect::close_db() {
	isDbOpen = false;
	sqlite3_close(database);
}

void SQLiteConnect::addNewLesson(std::string lessonName) {
	if(!isDbOpen)
		throw ERRORS::ERROR_DB_NOT_OPEN;

	std::string insertQuery = "INSERT INTO " + Database::LESSON_TABLE +
						" VALUES (NULL, '" +
						lessonName + "');";

	if(sqlite3_prepare(database, insertQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	if(sqlite3_step(queryStatement) != SQLITE_DONE) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
}

std::vector<std::string> SQLiteConnect::getLessons() {

	if(!isDbOpen)
		throw ERRORS::ERROR_DB_NOT_OPEN;

	std::string selectQuery = "SELECT " +
							  Database::LESSON_TABLE_LESSON_COLUMN +
							  " FROM " +
							  Database::LESSON_TABLE + ";";

	if(sqlite3_prepare(database, selectQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}



	int res = 0;
	// int ctotal = sqlite3_column_count(queryStatement);
	std::vector<std::string> lessons;

	while ((res = sqlite3_step(queryStatement)) != SQLITE_DONE) {


		if(res == SQLITE_ROW) {
			// if there is no entry, there would be an error, if we dont check this here.
			if((char*)sqlite3_column_text(queryStatement, 0) != nullptr) {
				std::string curLesson = (char*)sqlite3_column_text(queryStatement, 0);
				lessons.push_back(curLesson);
			}
		}

		if(res == SQLITE_ERROR) {
			throw ERRORS::ERROR_QUERY_EXECUTION;
		}
	}

	sqlite3_finalize(queryStatement);
	return lessons;
}


