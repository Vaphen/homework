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

/*
 * TODO: change saved date to unix timestamp
 * TODO: rename this file + header.
 */

/// Initialize the database and the queryStatement with nullptr
SQLiteConnect::SQLiteConnect() :
		database(nullptr),
		queryStatement(nullptr) {
}

/// Close the connection if it's still open.
SQLiteConnect::~SQLiteConnect() {
	sqlite3_close(database);
}

/// Delets the table of a lesson
/**
 * @param lessonName name of the table that should be deleted.
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::deleteSpecificLessonTable(std::string lessonName) {
	std::string deleteQuery = "DROP TABLE IF EXISTS " + lessonName;
	try {
		executeQuery(deleteQuery);
	}catch (ERRORS &error) {
		throw error;
	}
}

/// Creates a new lesson table
/**
 * @param lessonName name of the table that should be created
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::createSpecificLessonTable(std::string lessonName) {
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
							  lessonName + "(" +
							  std::string(Database::COLUMN["ID"]) +
							  " INTEGER PRIMARY KEY, " +
							  Database::COLUMN["TO_DO_UNTIL"] +
							  " TIME, " +
							  Database::COLUMN["REACHED_POINTS"] +
							  " INTEGER, " +
							  Database::COLUMN["TOTAL_POINTS"] +
							  " INTEGER, " +
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

/// Executes the given query
/**
 * The given query must be a not-returning-query (e.g. Drop, Insert, etc.)
 * Do not use returning querys like 'Select', etc.
 * @param query: a sqlite query
 * @throws ERRORS a SQL-execution error
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


/// Creates a new Table for all saved Lessons
/**
 * If the table does'nt exists, it creates a new one.
 * If it exists, nothing happens.
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::createAllLessonDb() {
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
									  Database::LESSON_TABLE +
									  "(id INTEGER PRIMARY KEY, " +
									  Database::LESSON_TABLE_LESSON_COLUMN +
									  " TEXT);";
	try {
		executeQuery(createQuery);
	}catch(ERRORS &error) {
		throw error;
	}
}


/// Opens a database connection
/**
 * If the database does'nt exists, it creates a new one.
 * @param dbName Name of the database; usually given in constants.h
 * @returns Database::ERROR or Database::Success
 */
bool SQLiteConnect::open_db(std::string dbName) {
	return (sqlite3_open(dbName.c_str(), &database) != SQLITE_OK) ?
		Database::ERROR :  Database::SUCCESS;
}

/// Closes a database connection
void SQLiteConnect::close_db() {
	sqlite3_close(database);
}

/// Writes a new lesson to 'lesson' table
/**
 * @param lessonName name of the lesson which should be added
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::addNewLesson(std::string lessonName) {
	std::string insertQuery = "INSERT INTO " + Database::LESSON_TABLE +
						" VALUES (NULL, '" +
						lessonName + "');";
	try {
		executeQuery(insertQuery);
	}catch(ERRORS &error) {
		throw error;
	}
}

// Writes a new exercise to a specific lesson table
/**
 * @param lessonName name of the lesson the exercise belongs to
 * @param folderPath path to the folder which contains the files belonging to the exercise
 * @param date the date until the exercise should be finished given as english-formatted date (YYYY-MM-DD)
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::addNewExercise(std::string lessonName, std::string folderPath, std::string date) {
	std::string insertQuery = "INSERT INTO " + lessonName + " VALUES (" +
							  "NULL, " +
							  "strftime('%d.%m.%Y', '" + date + "'), " +
							  "NULL, " + ""
							  "NULL, " +
							  "0, " +
							  "'');";

	try {
		executeQuery(insertQuery);
	}catch(ERRORS &error) {
		throw error;
	}
}

/// Deletes an exercise from a specific lessonTable by its exerciseId
/**
 * @param lessonName name of the Lesson the exercise belongs to
 * @param exerciseId id of the exercise which should be deleted
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::deleteExercise(std::string lessonName, unsigned int exerciseId) {
	std::string deleteQuery = "DELETE FROM " +
							  lessonName +
							  " WHERE id='" +
							  std::to_string(exerciseId) + "';";
	try {
		executeQuery(deleteQuery);
	}catch(ERRORS &error) {
		throw error;
	}
}

/// Deletes the table of a given lessons
/**
 * @param lessonName name of the lesson which should be deleted
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::deleteLesson(std::string lessonName) {
	std::string deleteQuery = "DELETE FROM " +
							  Database::LESSON_TABLE +
							  " WHERE lesson='" +
							  lessonName +
							  "';";
	try {
		executeQuery(deleteQuery);
	} catch(ERRORS &error) {
		throw error;
	}
}

/// Read all important informations from a specific lesson table
/**
 * The outer vector is the column of the table. The inner vector
 * contains the row.
 * @param lessonName name of the lesson table
 * @throws ERRORS a SQL-execution error
 * @returns a 2-dimensional vector that contains all exercises and
 * their informations like id, until, reached points, total points, etc.
 */
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

	// initialize the vector with the amount of columns (+1 for lessonName column)
	for(int i = 0; i < Database::COLUMN.size() + 1; i++)
	    lessons.push_back(std::vector< std::string >());



	while((res = sqlite3_step(queryStatement)) != SQLITE_DONE)
	{
	    for( int i = 0; i < Database::COLUMN.size(); i++ ) {
	    	if((char*)sqlite3_column_text(queryStatement, i) != nullptr) {
	    		// if an entry exists, push it back
	    		lessons.at(i).push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    	}else{
	    		//if not, we have to push back an empty string
	    		lessons.at(i).push_back("");
	    	}
	    }
	    // append to the end of the vector the current lesson
	    lessons.at(Database::COLUMN.size()).push_back(lessonName);
	}

	if(res == SQLITE_ERROR) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
	return lessons;
}

/// Read all lessons out of the all-lesson table
/**
 * @throws ERRORS a SQL-execution error
 * @returns vector containing all lessons
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
