/*
 * constants.h
 *
 *  Created on: 29.12.2014
 *      Author: vaphen
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PROGRAM_NAME "homework"
#define DELETE_ICO "./src/delete.png"
#define OPENDIR_ICO "./src/openDir.png"
#define SAVE_ICO "./src/save.png"
#define RESET_CHANGES_ICO "./src/resetChanges.png"

#ifdef __unix__
	#define FILEMANAGER "xdg-open"
	#define FOLDER_PATH "./uni/"
#elif defined(_WIN32) || defined(WIN32)
	#define FILEMANAGER "explorer"
	#define FOLDER_PATH "\\uni\\"
#endif

#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include <gtkmm.h>


enum class ERRORS { ERROR_OPEN_DB, ERROR_DB_NOT_PREPARABLE, ERROR_QUERY_EXECUTION, ERROR_CREATE_COLUMN };
enum class CONFIG_ERRORS { ERROR_UNKNOWN_PARAMETER, ERROR_FILE_NOT_CREATABLE};
namespace ConfigParams {
	static const std::string SAVE_DIR_PATH_TEXT = "SAVE_DIRECTORY_PATH=";
	static const int SAVE_DIRECTORY_PATH = 0;
}

namespace COLUMN_ID {
/**
 * TODO: not working because of wrong database. delete it and everything should work
 * its because old tables just have 5 columns.
 */
	static const int ID = 0;
	static const int UNTIL = 1;
	static const int REACHED_POINTS = 2;
	static const int TOTAL_POINTS = 3;
	static const int DIR_PATH = 4;
	static const int EXERCISE_FINISHED = 5;
	static const int EXERCISE_COMMENT = 6;
};


namespace Database {
	static const bool SUCCESS = true;
	static const bool ERROR = false;
	static const std::string LESSON_DB = "faecher.db";
	static std::map<std::string, std::string> COLUMN = {
			{"ID", "id"},
			{"TO_DO_UNTIL", "until"},
			{"REACHED_POINTS", "points_reached"},
			{"TOTAL_POINTS", "points_total"},
			{"FOLDER_PATH", "dir_path"},
			{"IS_FINISHED", "finished"},
			{"LESSON_COMMENT", "comment"}
	};
	static const std::string LESSON_TABLE = "lesson";
	static const std::string LESSON_TABLE_LESSON_COLUMN = "lesson";
};

#endif /* CONSTANTS_H_ */
