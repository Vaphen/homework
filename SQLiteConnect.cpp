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
		queryStatement(nullptr) {
}

SQLiteConnect::~SQLiteConnect() {
	sqlite3_close(database);
}

/**
 * @param dbName: the name of the database; usually given in constants.h
 * checks if the db exists. if not, it creates one.
 * returns Database-errors if an error occures
 * TODO: change return to throw and make more precise exceptions (like addNewLesson function)
 */
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
	return Database::SUCCESS;
}

/*
 * closes a database connection
 */
void SQLiteConnect::close_db() {
	sqlite3_close(database);
}

/**
 * @param lessonName: name (string) of the lesson which should be added
 * writes a new lesson to the db
 * throws db-errors if an error occures
 */
void SQLiteConnect::addNewLesson(std::string lessonName) {
	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

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
	close_db();
}

/**
 * @param lessonName: name (string) of the lesson which should be deleted
 * this method deletes an entry (if existant) from the Database.
 */
void SQLiteConnect::deleteLesson(std::string lessonName) {
	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string deleteQuery = "DELETE FROM " +
							  Database::LESSON_TABLE +
							  " WHERE lesson='" +
							  lessonName +
							  "';";

	if(sqlite3_prepare(database, deleteQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	if(sqlite3_step(queryStatement) != SQLITE_DONE) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
}

/**
 * returns all lessons given in the Database
 */
std::vector<std::string> SQLiteConnect::getLessons() {

	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string selectQuery = "SELECT " +
							  Database::LESSON_TABLE_LESSON_COLUMN +
							  " FROM " +
							  Database::LESSON_TABLE + ";";

	if(sqlite3_prepare(database, selectQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	int res = 0;
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
	close_db();
	return lessons;
}

