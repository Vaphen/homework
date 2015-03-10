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
#include "../requiredDependencies/sqlite/sqlite3.h"

class SQLiteConnect {
public:
	SQLiteConnect();
	virtual ~SQLiteConnect();

	void addNewLesson(std::string);
	void deleteLesson(std::string);
	void createAllLessonDb();
	void createSpecificLessonTable(std::string);
	void deleteSpecificLessonTable(std::string);
	void addNewExercise(std::string, std::string, unsigned int);
	void deleteExercise(std::string, unsigned int);
	void updateExercise(std::string const&, int const&, int const&, int const&, bool const&, std::string const&);
	std::vector<std::vector<std::string>> getExercises(std::string);
	std::vector<std::string> getLessons();
	std::vector<std::vector<std::string>> getPoints(std::string&);
private:
	sqlite3 *database;
	sqlite3_stmt *queryStatement;
	bool open_db(std::string);
	void close_db();
	void executeQuery(std::string);
	bool isTableExistant(std::string&);
};

#endif /* SQLITECONNECT_H_ */
