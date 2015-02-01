/*
 * constants.h
 *
 *  Created on: 29.12.2014
 *      Author: vaphen
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include <gtkmm.h>

#define PROGRAM_NAME "homework"
#define DELETE_ICO "./src/delete.png"
#define OPENDIR_ICO "./src/openDir.png"
#define OPENDIR_ICO_SMALL "./src/openDirSmall.png"
#define SAVE_ICO "./src/save.png"
#define RESET_CHANGES_ICO "./src/resetChanges.png"
#define OPEN_PDF_ICO "./src/openPdf.png"
#define EXERCISE_PDF_FILE "Aufgabenblatt.pdf"

static const std::string CONFIG_FILE = "./homework.ini";

#ifdef __unix__
	static const std::string FILEMANAGER = "xdg-open";
	static const std::string FOLDER_PATH = "./uni/";
#elif defined(_WIN32) || defined(WIN32)
	static const std::string FILEMANAGER = "explorer";
	static const std::string FOLDER_PATH = "\\uni\\";
#endif

enum class ERRORS { ERROR_OPEN_DB, ERROR_DB_NOT_PREPARABLE, ERROR_QUERY_EXECUTION, ERROR_CREATE_COLUMN };
enum class FILE_ERRORS { FOLDER_NOT_CREATABLE, FOLDER_NOT_DELETABLE };

namespace COLUMN_ID {
	static const int ID = 0;
	static const int UNTIL = 1;
	static const int REACHED_POINTS = 2;
	static const int TOTAL_POINTS = 3;
	static const int EXERCISE_FINISHED = 4;
	static const int EXERCISE_COMMENT = 5;
	static const int LESSON = 6;
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
			{"IS_FINISHED", "finished"},
			{"LESSON_COMMENT", "comment"}
	};
	static const std::string LESSON_TABLE = "lesson";
	static const std::string LESSON_TABLE_LESSON_COLUMN = "lesson";
};

#endif /* CONSTANTS_H_ */
