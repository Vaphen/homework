/*
 * SQLiteConnect.cpp
 *
 *  Created on: 27.12.2014
 *      Author: vaphen
 */

#include "SQLiteConnect.h"
#include "../constants/constants.h"
#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>


/**
 * TODO: rename this file + header.
 */
SQLiteConnect::SQLiteConnect() :
		database(nullptr),
		queryStatement(nullptr) {
}

SQLiteConnect::~SQLiteConnect() {
	sqlite3_close(database);
}

/**
 * deletes from Database::LESSON_DB the table with the name of the given lesson
 */
void SQLiteConnect::deleteSpecificLessonTable(std::string lessonName) {
	std::string deleteQuery = "DROP TABLE IF EXISTS " + lessonName;
	try {
		executeQuery(deleteQuery);
	}catch (ERRORS &error) {
		throw error;
	}
}

/**
 * creates to Database::LESSON_DB a new table with the name of the given lesson
 * @param lessonName: the name of the lesson (the new db name).
 */
void SQLiteConnect::createSpecificLessonTable(std::string lessonName) {
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
							  lessonName + "(" +
							  std::string(Database::COLUMN["ID"]) +
							  " INTEGER PRIMARY KEY, " +
							  Database::COLUMN["TO_DO_UNTIL"] +
							  " DATETIME, " +
							  Database::COLUMN["REACHED_POINTS"] +
							  " INTEGER, " +
							  Database::COLUMN["TOTAL_POINTS"] +
							  " INTEGER, " +
							  Database::COLUMN["FOLDER_PATH"] +
							  " TEXT, " +
							  Database::COLUMN["IS_FINISHED"] +
							  " BOOLEAN, " +
							  Database::COLUMN["LESSON_COMMENT"] +
							  " TEXT);";
	try {
		executeQuery(createQuery);
	}catch(ERRORS &error) {
		throw error;
	}
}

/**
 * executes the given query
 * @param query: a sqlite create, delete or drop query
 */
void SQLiteConnect::executeQuery(std::string query) {
	if(open_db(Database::LESSON_DB) != Database::SUCCESS) {
		throw ERRORS::ERROR_OPEN_DB;
	}

	if(sqlite3_prepare_v2(database, query.c_str(), -1, &queryStatement, 0) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	if(sqlite3_step(queryStatement) != SQLITE_DONE) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
}

/**
 * creates the lesson db with predefined db-structurer
 * @param lessonDB-Name
 */
void SQLiteConnect::createAllLessonDb() {
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
									  Database::LESSON_TABLE +
									  "(id INTEGER PRIMARY KEY, " +
									  Database::LESSON_TABLE_LESSON_COLUMN +
									  " TEXT);";
	if(open_db(Database::LESSON_DB) != Database::SUCCESS) {
		throw ERRORS::ERROR_OPEN_DB;
	}

	if(sqlite3_prepare_v2(database, createQuery.c_str(), -1, &queryStatement, 0) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	if(sqlite3_step(queryStatement) != SQLITE_DONE) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
}

/**
 * @param dbName: the name of the database; usually given in constants.h
 * checks if the db exists. if not, it creates one.
 * returns Database::ERROR if an error occures
 */
bool SQLiteConnect::open_db(std::string dbName) {
	return (sqlite3_open(dbName.c_str(), &database) != SQLITE_OK) ?
		Database::ERROR :  Database::SUCCESS;
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
 * adds a new exercise to a specific lesson table
 * @param lessonName the name of the lesson the exercise belongs to
 * @param folderPath path to the folder which contains the files which belong to the exercise
 */
void SQLiteConnect::addNewExercise(std::string lessonName, std::string folderPath) {
	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string insertQuery = "INSERT INTO " + lessonName +
						" VALUES (NULL, date('now'), NULL, NULL, '" + folderPath + "', 0, '');";

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
 * deletes an exercise from a specific lessonTable by its exerciseId
 * @param lessonName name of the Lesson the exercise belongs to
 * @param exerciseId id of the exercise which should be deleted
 */
void SQLiteConnect::deleteExercise(std::string lessonName, int exerciseId) {
	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string deleteQuery = "DELETE FROM " +
							  lessonName +
							  " WHERE id='" +
							  std::to_string(exerciseId) + "';";

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

std::vector<std::vector<std::string>> SQLiteConnect::getExercises(std::string lessonName) {

	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string selectQuery = "SELECT * FROM " +
							  lessonName + ";";

	if(sqlite3_prepare(database, selectQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	int res = 0;
	std::vector<std::vector<std::string>> lessons;

	// initialize the vector with the amount of columns
	for(int i = 0; i < Database::COLUMN.size(); i++)
	    lessons.push_back(std::vector< std::string >());



	while((res = sqlite3_step(queryStatement)) != SQLITE_DONE)
	{
	    for( int i = 0; i < Database::COLUMN.size(); i++ ) {
	    	if((char*)sqlite3_column_text(queryStatement, i) != nullptr) {
	    		// if an entry exists, push it back
	    		lessons.at(i).push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		std::cout << i << ":" << (char *)sqlite3_column_text(queryStatement, i) << std::endl;
	    	}else{
	    		//if not, we have to push back an empty string
	    		lessons.at(i).push_back("");
	    	}
	    }
	}

	if(res == SQLITE_ERROR) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
	return lessons;
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

