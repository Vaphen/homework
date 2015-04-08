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
#include "../helpers/TimeConvert.h"

class SQLiteConnect {
public:
	SQLiteConnect();
	virtual ~SQLiteConnect();

	void addNewLesson(std::string);
	void deleteLesson(std::string);
	void createAllLessonTable();
	void createMarkTable();
	void createSpecificLessonTable(std::string);
	void deleteSpecificLessonTable(std::string);
	void addNewExercise(std::string, unsigned int);
	void deleteExercise(std::string, unsigned int);
	void updateExercise(std::string const&, int const&, int const&, int const&, bool const&, std::string const&);
	void addNewExam(const unsigned int, const std::string);
	void deleteExam(const unsigned int);
	void updateExam(unsigned int, unsigned int, unsigned short, bool, bool, float);
	std::vector<std::vector<std::string> > getExercises(std::string);
	std::vector<std::vector<std::string> > getMarks();
	std::vector<std::string> getLessons();
	std::vector<std::vector<std::string> > getPoints(std::string&);
private:
	sqlite3 *database;
	sqlite3_stmt *queryStatement;
	bool open_db(std::string);
	void close_db();
	void executeQuery(std::string&, std::string);
	TimeConvert timeConverter;
};

#endif /* SQLITECONNECT_H_ */
