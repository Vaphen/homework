/*
 * constants.h
 *
 *  Created on: 29.12.2014
 *      Author: vaphen
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define DELETE_ICO "./src/delete.png"
#define OPENDIR_ICO "./src/openDir.png"

#ifdef __unix__
	#define FILEMANAGER "xdg-open"
	#define FOLDER_PATH "./uni/"
#elif defined(_WIN32) || defined(WIN32)
	#define FILEMANAGER "explorer"
	#define FOLDER_PATH "\\uni\\"
#endif

#include <string>
#include <ctime>
#include <map>
#include <gtkmm.h>


enum class ERRORS { ERROR_OPEN_DB, ERROR_DB_NOT_PREPARABLE, ERROR_QUERY_EXECUTION, ERROR_CREATE_COLUMN };


static bool isValidDate(std::string dateAsString) {
	// TODO: fix this function. it doesnt work
	// format: dd.mm.yyyy
	if(dateAsString.size() != 10) return false;
	struct tm timeinfo_now;
	return strptime(dateAsString.c_str(), "%d.%m.%y", &timeinfo_now);
}

namespace TABLE_LABELS {
	static const std::string UNTIL = "<b>Aufgabe zu erledigen bis</b>";
	static const std::string REACHED_POINTS = "<b>Erreichte Punkte</b>";
	static const std::string TOTAL_POINTS = "<b>Maximale Punktzahl</b>";
	static const std::string OPEN_DIR = "<b>Verzeichnis öffnen</b>";
	static const std::string EXERCISE_FINISHED = "<b>Erledigt</b>";
	static const std::string EXERCISE_COMMENT = "<b>Kommentar</b>";
	static const std::string DELETE_EXERCISE = "<b>Löschen</b>";
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

namespace Dialogs {
	/**
	 * shows an error-dialog that gives out an sql-error
	 * @param error: enum Error-message. thrown by SQLiteConnect
	 */
	static void showErrorDialog(ERRORS& error) {
		std::string title = "";
		std::string message = "";

		switch(error) {
			case ERRORS::ERROR_OPEN_DB:
				title = "Verbindung zur Datenbank konnte nicht hergestellt werden.";
				message = "Bitte überprüfen Sie die Berechtigungen des Programms.";
				break;
			case ERRORS::ERROR_DB_NOT_PREPARABLE:
				title = "Die Datenbankanfrage konnte nicht vorbereitet werden.";
				message = "Möglicherweise gibt es einen Fehler in dem Abfrage-Query.";
				break;
			case ERRORS::ERROR_QUERY_EXECUTION:
				title = "Die Datenbankabfrage konnte nicht ausgeführt werden.";
				message = "Möglicherweise gibt es einen Fehler in dem Abfrage-Query oder die Datenbank existiert nicht.";
				break;
			case ERRORS::ERROR_CREATE_COLUMN:
				title = "Eine Spalte konnte nicht gelesen werden.";
				message = "Es scheint einen Fehler in der Datenbank zu geben.";
				break;
			default:
				title = "Ein unbekannter Fehler ist aufgetreten.";
				message = "Sorry, das hätte nicht passieren dürfen.";
		}

		Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		dialog.set_title("Ein Fehler ist aufgetreten.");
		dialog.set_secondary_text(message);
		dialog.run();
	}

	/**
	 * shows individual error-dialog.
	 * @param title: title of the message-box
	 * @param message: message of the message-box
	 */
	static void showErrorDialog(std::string title, std::string message) {
		Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		dialog.set_title("Ein Fehler ist aufgetreten.");
		dialog.set_secondary_text(message);
		dialog.run();
	}
}

#endif /* CONSTANTS_H_ */
