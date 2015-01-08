/*
 * constants.h
 *
 *  Created on: 29.12.2014
 *      Author: vaphen
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
namespace Database {
	static const bool SUCCESS = true;
	static const bool ERROR = false;
	static std::string LESSON_DB = "faecher.db";
	static std::string LESSON_TABLE = "lesson";
	static std::string LESSON_TABLE_LESSON_COLUMN = "lesson";
};
enum class ERRORS { ERROR_DB_NOT_OPEN, ERROR_DB_NOT_PREPARABLE, ERROR_QUERY_EXECUTION };

#endif /* CONSTANTS_H_ */
