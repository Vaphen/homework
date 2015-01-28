/*
 * HelpDialogs.h
 *
 *  Created on: 23.01.2015
 *      Author: john
 */

#ifndef CONSTANTS_HELPDIALOGS_H_
#define CONSTANTS_HELPDIALOGS_H_

#include "../constants/constants.h"

namespace HelpDialogs {
	const static bool CONFIRMED = true;
	const static std::string CONFIRM_DELETION = "Löschen bestätigen";
	const static std::string CONFIRM_DELETION_SUBTEXT = "Wollen sie die ausgewählte Aufgabe wirklich löschen?";
	const static std::string CONFIRM_LESSON_DELETION_SUBTEXT = "Soll das markierte Fach inklusive aller Aufgabenblätter, erreichter Punkte etc. gelöscht werden?";

	/**
	 * shows individual error-dialog.
	 * @param title: title of the message-box
	 * @param message: message of the message-box
	 */
	static void showErrorDialog(std::string const &title, std::string const &message) {
		Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		dialog.set_title("Ein Fehler ist aufgetreten.");
		dialog.set_secondary_text(message);
		dialog.run();
	}

	/**
	 * shows an error-dialog that gives out an sql-error
	 * @param error: enum Error-message. thrown by SQLiteConnect
	 */
	static void showErrorDialog(ERRORS& error) {
		std::string title, message;
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

		HelpDialogs::showErrorDialog(title, message);
	}

	/**
	 * shows individual error-dialog.
	 * @param title: title of the message-box
	 * @param message: message of the message-box
	 */
	static bool showConfirmDialog(std::string const &title, std::string const &message) {
		Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
		dialog.set_title(title);
		dialog.set_secondary_text(message);
		return (dialog.run() == Gtk::RESPONSE_YES) ? HelpDialogs::CONFIRMED : !HelpDialogs::CONFIRMED;
	}


	/*
	 * @param title: title of the messagedialog
	 * @param message: message of the messagedialog
	 * Shows a dialog with a success-image
	 */
	static void showInfoDialog(std::string const &title, std::string const& message) {
		Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
		dialog.set_title(title);
		dialog.set_secondary_text(message);
		dialog.run();
	}
}

#endif /* CONSTANTS_HELPDIALOGS_H_ */
