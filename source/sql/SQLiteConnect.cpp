/*
 * SQLiteConnect.cpp
 *
 *  Created on: 27.12.2014
 *      Author: vaphen
 */

#include "SQLiteConnect.h"
#include "../constants/constants.h"
#include "../helpers/TimeConvert.h"
#include <string>
#include <vector>
#include "../requiredDependencies/sqlite/sqlite3.h"

#if defined(_WIN32) || defined(WIN32)
	#include "../helpers/conversions.h"
	#define nullptr 0x00
#endif

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
		executeQuery(deleteQuery, Database::LESSON_DB);
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
							  std::string(Database::COLUMN_LESSON_DB["ID"]) +
							  " INTEGER PRIMARY KEY, " +
							  Database::COLUMN_LESSON_DB["TO_DO_UNTIL"] +
							  " TIME, " +
							  Database::COLUMN_LESSON_DB["REACHED_POINTS"] +
							  " INTEGER, " +
							  Database::COLUMN_LESSON_DB["TOTAL_POINTS"] +
							  " INTEGER, " +
							  Database::COLUMN_LESSON_DB["IS_FINISHED"] +
							  " BOOLEAN, " +
							  Database::COLUMN_LESSON_DB["LESSON_COMMENT"] +
							  " TEXT);";
	try {
		executeQuery(createQuery, Database::LESSON_DB);
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
void SQLiteConnect::executeQuery(std::string &query, std::string databaseName) {
	if(open_db(databaseName) != Database::SUCCESS) {
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
void SQLiteConnect::createAllLessonTable() {
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
									  Database::LESSON_TABLE +
									  "(id INTEGER PRIMARY KEY, " +
									  Database::LESSON_TABLE_LESSON_COLUMN +
									  " TEXT);";
	try {
		executeQuery(createQuery, Database::LESSON_DB);
	}catch(ERRORS &error) {
		throw error;
	}
}


/// Creates a new Table for saved marks
/**
 * If the table does'nt exists, it creates a new one.
 * If it exists, nothing happens.
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::createMarkTable() {
	// closes the db if its already open
	close_db();
	open_db(Database::MARK_DB);
	std::string createQuery = "CREATE TABLE IF NOT EXISTS " +
									  Database::MARK_TABLE +
									  "(id INTEGER PRIMARY KEY, " +
									  Database::COLUMN_MARK_DB["DATE_OF_EXAM"] +
									  " TIME, " +
									  Database::COLUMN_MARK_DB["LESSON_NAME"] +
									  " TEXT, " +
									  Database::COLUMN_MARK_DB["SEMESTER"] +
									  " INTEGER, " +
									  Database::COLUMN_MARK_DB["IS_ADMITTED"] +
									  " BOOLEAN, " +
									  Database::COLUMN_MARK_DB["HAS_PASSED"] +
									  " BOOLEAN, " +
									  Database::COLUMN_MARK_DB["MARK"] +
									  " FLOAT);";

	try {
		executeQuery(createQuery, Database::MARK_DB);
	}catch(ERRORS &error) {
		throw error;
	}
	close_db();
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
 * @throws ERRORS::ERROR_TABLE_ALREADY_EXISTS if table is already existant
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::addNewLesson(std::string lessonName) {
	std::string insertQuery = "INSERT INTO " + Database::LESSON_TABLE +
						" VALUES (NULL, '" +
						lessonName + "');";
	try {
		executeQuery(insertQuery, Database::LESSON_DB);
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
void SQLiteConnect::addNewExercise(std::string lessonName, unsigned int unixTimeStamp) {
	std::string insertQuery = "INSERT INTO " + lessonName + " VALUES (" +
							  "NULL, " +
							  "'" + std::to_string(unixTimeStamp) + "', " +
							  "0, " + ""
							  "0, " +
							  "0, " +
							  "'');";

	try {
		executeQuery(insertQuery, Database::LESSON_DB);
	}catch(ERRORS &error) {
		throw error;
	}
}

void SQLiteConnect::addNewExam(const unsigned int semester, const std::string lesson) {
	unsigned int currentUnixTimestamp = timeConverter.getUnixTimeFormat(timeConverter.getCurDay(),
			timeConverter.getCurMonth(), timeConverter.getCurYear());
	std::string insertQuery = "INSERT INTO " + Database::MARK_TABLE + " VALUES (" +
							  "NULL, " +
							  "'0', " +
							  "'" + lesson + "', " +
							  std::to_string(semester) + ", " +
							  "0, " +
							  "0, "+
							  "0.0);";

	try {
		executeQuery(insertQuery, Database::MARK_DB);
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
		executeQuery(deleteQuery, Database::LESSON_DB);
	}catch(ERRORS &error) {
		throw error;
	}
}

/// Deletes an exam from the marktable by its exerciseId
/**
 * @param examId id of the exam which should be deleted
 * @throws ERRORS a SQL-execution error
 */
void SQLiteConnect::deleteExam(const unsigned int examId) {
	std::string deleteQuery = "DELETE FROM " +
							  Database::MARK_TABLE +
							  " WHERE id='" +
							  std::to_string(examId) + "';";
	try {
		executeQuery(deleteQuery, Database::MARK_DB);
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
		executeQuery(deleteQuery, Database::LESSON_DB);
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
							  lessonName + " ORDER BY " + Database::COLUMN_LESSON_DB["TO_DO_UNTIL"] + " DESC;";

	if(sqlite3_prepare(database, selectQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	int res = 0;
	std::vector<std::vector<std::string> > lessons;

	// initialize the vector with the amount of columns (+1 for lessonName column)
	for(unsigned int i = 0; i < Database::COLUMN_LESSON_DB.size() + 1; i++)
		lessons.push_back(std::vector<std::string>());

	while((res = sqlite3_step(queryStatement)) != SQLITE_DONE)
	{
	    for( unsigned int i = 0; i < Database::COLUMN_LESSON_DB.size(); i++ ) {
	    	if((char*)sqlite3_column_text(queryStatement, i) != nullptr) {
	    		// a value is set; push it into lessons vec
	    		lessons.at(i).push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    	}else{
	    		//if not, we have to push back an empty string
	    		lessons.at(i).push_back("");
	    	}
	    }
	    // append to the end of the vector the current lesson
	    lessons.at(Database::COLUMN_LESSON_DB.size()).push_back(lessonName);
	}

	if(res == SQLITE_ERROR) {
		throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
	return lessons;
}


/// read the exercise name (until), reached Points and total Points
/**
 * The outer vector is the column of the table. The inner vector
 * contains the row.
 * @param lessonName name of the lesson table
 * @throws ERRORS a SQL-execution error
 * @returns a 2-dimensional vector that contains a unix-timestamp
 * until when the exercise was finished, total Points and reached points
 */
std::vector<std::vector<std::string>> SQLiteConnect::getPoints(std::string &lessonName) {

	if(open_db(Database::LESSON_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string selectQuery = "SELECT " +
							   Database::COLUMN_LESSON_DB["TO_DO_UNTIL"] + ", " +
							   Database::COLUMN_LESSON_DB["REACHED_POINTS"] + ", " +
							   Database::COLUMN_LESSON_DB["TOTAL_POINTS"] +
							   " FROM " +
							  lessonName + ";";

	if(sqlite3_prepare(database, selectQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	int res = 0;
	std::vector<std::vector<std::string>> nameAndPointsVec;

	/*
	 * TODO: check if reachedPointsVec, totalPointsVec, untilVec are not useless
	 * (Eg u can push the values directly into the nameAndPointsVec).
	 */
	std::vector<std::string> reachedPointsVec;
	std::vector<std::string> totalPointsVec;
	std::vector<std::string> untilVec;
	while((res = sqlite3_step(queryStatement)) != SQLITE_DONE)
	{
	    for( int i = 0; i < 3; i++ ) {
	    	if((char*)sqlite3_column_text(queryStatement, i) != nullptr) {
	    		// if an entry exists, push it back
	    		if(i % 3 == 0) {
	    			untilVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 3 == 1) {
	    			reachedPointsVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 3 == 2) {
	    			totalPointsVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}
	    	}else{
	    		//if not, we have to push back an empty string
	    		if(i % 3 == 0) {
	    			untilVec.push_back("");
	    		}else if(i % 3 == 1) {
	    			reachedPointsVec.push_back("");
	    		}else if(i % 3 == 2) {
	    			totalPointsVec.push_back("");
	    		}
	    	}
	    }
	}

	// it should not push anything back if there is no entry. if there is none, we return the
	// vector without inner vectors
	if(untilVec.size() != 0 && totalPointsVec.size() != 0 && reachedPointsVec.size() != 0) {
		nameAndPointsVec.push_back(untilVec);
		nameAndPointsVec.push_back(reachedPointsVec);
		nameAndPointsVec.push_back(totalPointsVec);
	}

	if(res == SQLITE_ERROR)
		throw ERRORS::ERROR_QUERY_EXECUTION;

	sqlite3_finalize(queryStatement);
	close_db();
	return nameAndPointsVec;
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

		if(res == SQLITE_ERROR)
			throw ERRORS::ERROR_QUERY_EXECUTION;
	}

	sqlite3_finalize(queryStatement);
	close_db();
	return lessons;
}

/// Updates an existing exercise
/**
 * @param lesson the lesson the exercise belongs to
 * @param exerciseId the existing id of the exercise
 * @param reachedPoints new reached Points
 * @param totalPoints new total Points
 * @param isFinished new state (true if finished)
 * @throws ERRORS a sql-error in case of failure
 */
void SQLiteConnect::updateExercise(std::string const &lesson,
								   int const &exerciseId,
								   int const &reachedPoints,
								   int const &totalPoints,
								   bool const &isFinished,
								   std::string const &comment) {
	std::string updateQuery = "UPDATE " + lesson + " SET " +
								Database::COLUMN_LESSON_DB["REACHED_POINTS"] + "='" + std::to_string(reachedPoints) + "', " +
							  Database::COLUMN_LESSON_DB["TOTAL_POINTS"] + "='" + std::to_string(totalPoints) + "', " +
							  Database::COLUMN_LESSON_DB["IS_FINISHED"] + "='" + std::to_string(isFinished) + "', " +
							  Database::COLUMN_LESSON_DB["LESSON_COMMENT"] + "='" + comment + "' " +
							  "WHERE " + Database::COLUMN_LESSON_DB["ID"] + "='" +
							  std::to_string(exerciseId) + "';";
	try {
		executeQuery(updateQuery, Database::LESSON_DB);
	}catch(ERRORS &error) {
		throw error;
	}
}

/// Updates an existing exam
/**
 * @param lesson the lesson the exercise belongs t
 * @param exerciseId the existing id of the exercise
 * @param reachedPoints new reached Points
 * @param totalPoints new total Points
 * @param isFinished new state (true if finished)
 * @throws ERRORS a sql-error in case of failure
 */
void SQLiteConnect::updateExam(unsigned int examId,
							   unsigned int dateOfExamTimestamp,
							   unsigned short semester,
							   bool isAdmitted,
							   bool hasPassed,
							   float mark) {

	std::string updateQuery = "UPDATE " + Database::MARK_TABLE + " SET " +
									Database::COLUMN_MARK_DB["DATE_OF_EXAM"] + "='" + std::to_string(dateOfExamTimestamp) + "', " +
								  Database::COLUMN_MARK_DB["SEMESTER"] + "='" + std::to_string(semester) + "', " +
								  Database::COLUMN_MARK_DB["IS_ADMITTED"] + "='" + std::to_string(isAdmitted) + "', " +
								  Database::COLUMN_MARK_DB["HAS_PASSED"] + "='" + std::to_string(hasPassed) + "', " +
								  Database::COLUMN_MARK_DB["MARK"] + "='" + std::to_string(mark) + "' " +
								  "WHERE " + Database::COLUMN_MARK_DB["ID"] + "='" +
								  std::to_string(examId) + "';";
	try {
		executeQuery(updateQuery, Database::MARK_DB);
	}catch(ERRORS &error) {
		throw error;
	}
}


std::vector<std::vector<std::string> > SQLiteConnect::getMarks() {

	if(open_db(Database::MARK_DB) == Database::ERROR)
		throw ERRORS::ERROR_OPEN_DB;

	std::string selectQuery = "SELECT id, " +
							   Database::COLUMN_MARK_DB["DATE_OF_EXAM"] + ", " +
							   Database::COLUMN_MARK_DB["LESSON_NAME"] + ", " +
							   Database::COLUMN_MARK_DB["SEMESTER"] + ", " +
							   Database::COLUMN_MARK_DB["IS_ADMITTED"] + ", " +
							   Database::COLUMN_MARK_DB["HAS_PASSED"] + ", " +
							   Database::COLUMN_MARK_DB["MARK"] +
							   " FROM " +
							  Database::MARK_TABLE + ";";

	if(sqlite3_prepare(database, selectQuery.c_str(), -1, &queryStatement, NULL) != SQLITE_OK) {
		throw ERRORS::ERROR_DB_NOT_PREPARABLE;
	}

	int res = 0;
	std::vector<std::vector<std::string> > marksAndStateVec;

	/*
	 * TODO: check if reachedPointsVec, totalPointsVec, untilVec are not useless
	 * (Eg u can push the values directly into the nameAndPointsVec).
	 */
	std::vector<std::string> idVec;
	std::vector<std::string> untilVec;
	std::vector<std::string> lessonVec;
	std::vector<std::string> semesterVec;
	std::vector<std::string> isAdmittedVec;
	std::vector<std::string> hasPassedVec;
	std::vector<std::string> markVec;
	while((res = sqlite3_step(queryStatement)) != SQLITE_DONE)
	{
	    for( int i = 0; i < 7; i++ ) {
	    	if((char*)sqlite3_column_text(queryStatement, i) != nullptr) {
	    		// if an entry exists, push it back
	    		if(i % 7 == 0) {
	    			idVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 7 == 1) {
	    			untilVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 7 == 2) {
	    			lessonVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 7 == 3) {
	    			semesterVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 7 == 4) {
	    			isAdmittedVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 7 == 5) {
	    			hasPassedVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}else if(i % 7 == 6) {
	    			markVec.push_back(std::string((char *)sqlite3_column_text(queryStatement, i)));
	    		}
	    	}else{
	    		//if not, we have to push back an empty string
	    		if(i % 7 == 0) {
					idVec.push_back("");
				}else if(i % 7 == 1) {
					untilVec.push_back("");
				}else if(i % 7 == 2) {
					lessonVec.push_back("");
				}else if(i % 7 == 3) {
					semesterVec.push_back("");
				}else if(i % 7 == 4) {
					isAdmittedVec.push_back("");
				}else if(i % 7 == 5) {
					hasPassedVec.push_back("");
				}else if(i % 7 == 6) {
					markVec.push_back("");
				}
	    	}
	    }
	}

	// it should not push anything back if there is no entry. if there is none, we return the
	// vector without inner vectors
	if(idVec.size() != 0 &&untilVec.size() != 0 && lessonVec.size() != 0 && semesterVec.size() != 0 &&
			isAdmittedVec.size() != 0 && hasPassedVec.size() != 0 && markVec.size() != 0) {
		marksAndStateVec.push_back(idVec);
		marksAndStateVec.push_back(untilVec);
		marksAndStateVec.push_back(lessonVec);
		marksAndStateVec.push_back(semesterVec);
		marksAndStateVec.push_back(isAdmittedVec);
		marksAndStateVec.push_back(hasPassedVec);
		marksAndStateVec.push_back(markVec);
	}

	if(res == SQLITE_ERROR)
		throw ERRORS::ERROR_QUERY_EXECUTION;

	sqlite3_finalize(queryStatement);
	close_db();
	return marksAndStateVec;
}
