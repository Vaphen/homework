/*  * gui.cpp  *  *  Created on: 25.12.2014  *      Author: vaphen  */

#include "GUI.h"
#include "LessonPage.h"
#include "SettingsPage.h"
#include "SQLiteConnect.h"
#include "constants.h"

#include <gtkmm.h>
#include <iostream>


GUI::GUI() :
		notebook(new Gtk::Notebook()),
	//	welcome_frame(new Gtk::Frame()),
	//	welcome_text(new Gtk::Label(WELCOME_TEXT)),
		settings_frame(new SettingsPage(notebook)) {

	//welcome_frame->add(*welcome_text);

	// add start page
	//notebook->append_page(*welcome_frame, NOTEBOOK_WELCOME, true);

	// add all lessons
	std::vector<std::string> lessons = doSqlLessonRequest();
	for(std::string &lesson : lessons) {
		addLessonPage(lesson);
	}

	// add settings page
	notebook->append_page(*settings_frame, NOTEBOOK_SETTINGS, false);

	set_title(WINDOW_TITLE);
	add(*notebook);
	show_all();
	maximize();
}

/*
 * shows an Dialog with infoicon
 * @param title: titel
 * @param message: nachricht
 */
void GUI::showErrorDialog(std::string title, std::string message) {
	Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
	dialog.set_title("Ein Fehler ist aufgetreten.");
	dialog.set_secondary_text(message);
	dialog.run();
}

/*
 * makes SQL-Request to get saved lessons.
 * Shows an errordialog if an error occures and exits the program
 */
std::vector<std::string> GUI::doSqlLessonRequest() {
	if(this->connection.open_db(Database::LESSON_DB) == Database::ERROR) {
		showErrorDialog("Verbindung zur Datenbank konnte nicht hergestellt werden.",
						"Bitte überprüfen Sie die Berechtigungen des Programms.");
		exit(0);
	}

	std::vector<std::string> lessons;
	try {
		lessons = this->connection.getLessons();
	} catch (ERRORS &error) {
		switch(error) {
			case ERRORS::ERROR_DB_NOT_OPEN:
				showErrorDialog("Verbindung zur Datenbank konnte nicht hergestellt werden.",
								"Bitte überprüfen Sie die Berechtigungen des Programms.");
				break;
			case ERRORS::ERROR_DB_NOT_PREPARABLE:
				showErrorDialog("Die Datenbankanfrage konnte nicht vorbereitet werden.",
								"Möglicherweise gibt es einen Fehler in dem Abfrage-Query.");
				break;
			case ERRORS::ERROR_QUERY_EXECUTION:
				showErrorDialog("Die Datenbankabfrage konnte nicht ausgeführt werden.",
								"Möglicherweise gibt es einen Fehler in dem Abfrage-Query oder die Datenbank existiert nicht.");
				break;
			default:
				showErrorDialog("Ein unbekannter Fehler ist aufgetreten.",
								"Sorry, das hätte nicht passieren dürfen.");
				break;
		}
		this->connection.close_db();
		exit(0);
	}
	this->connection.close_db();
	return lessons;
}


void GUI::addLessonPage(std::string newLesson) {
	LessonPage *newFrame = Gtk::manage(new LessonPage(newLesson));
	notebook->append_page(*newFrame, newLesson, true);
}


/**
 * delete all widget-pointer
 */
GUI::~GUI() {
	delete notebook;
	//delete welcome_frame;
	//delete welcome_text;
	delete settings_frame;
}
