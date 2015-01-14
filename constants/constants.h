/*
 * constants.h
 *
 *  Created on: 29.12.2014
 *      Author: vaphen
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
#include <gtkmm.h>


enum class ERRORS { ERROR_OPEN_DB, ERROR_DB_NOT_PREPARABLE, ERROR_QUERY_EXECUTION, ERROR_CREATE_COLUMN };

namespace TABLE_LABELS {
	static const std::string UNTIL = "<b>Aufgabe zu erledigen bis</b>";
	static const std::string REACHED_POINTS = "<b>Erreichte Punkte</b>";
	static const std::string TOTAL_POINTS = "<b>Maximale Punktzahl</b>";
	static const std::string EXERCISE_FINISHED = "<b>Erledigt</b>";
	static const std::string EXERCISE_COMMENT = "<b>Kommentar</b>";
	static const std::string DELETE_EXERCISE = "<b>Löschen</b>";
}

namespace COLUMN_ID {
	static const int ID = 0;
	static const int UNTIL = 1;
	static const int REACHED_POINTS = 2;
	static const int TOTAL_POINTS = 3;
	static const int EXERCISE_FINISHED = 4;
	static const int EXERCISE_COMMENT = 5;
};

namespace Database {
	static const bool SUCCESS = true;
	static const bool ERROR = false;
	static const std::string LESSON_DB = "faecher.db";
	static const std::string SPECIFIC_LESSON_UNTIL_COLUMN = "until";
	static const std::string SPECIFIC_LESSON_REACHED_POINTS = "points_reached";
	static const std::string SPECIFIC_LESSON_TOTAL_POINTS = "points_total";
	static const std::string SPECIFIC_LESSON_FINISHED = "finished";
	static const std::string SPECIFIC_LESSON_COMMENT = "comment";
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
