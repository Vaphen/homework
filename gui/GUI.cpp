/*  * gui.cpp  *  *  Created on: 25.12.2014  *      Author: vaphen  */

#include "GUI.h"
#include "LessonPage.h"
#include "SettingsPage.h"
#include "../sql/SQLiteConnect.h"
#include "../constants/constants.h"
#include "../folderOperations/BasicFileOps.h"

#include <gtkmm.h>
#include <iostream>


GUI::GUI() :
		notebook(new Gtk::Notebook()),
	//	welcome_frame(new Gtk::Frame()),
	//	welcome_text(new Gtk::Label(WELCOME_TEXT)),
		settings_frame(nullptr) {

	//welcome_frame->add(*welcome_text);

	// add start page
	//notebook->append_page(*welcome_frame, NOTEBOOK_WELCOME, true);

	// add all lessons
	std::vector<std::string> lessons = doSqlLessonRequest();
	for(std::string &lesson : lessons) {
		addLessonPage(lesson);
	}

	// could not use initialization list because if theres a sql-error,
	// it shows it twice. thats why its initialized here.
	settings_frame = new SettingsPage(notebook);

	// add settings page
	notebook->append_page(*settings_frame, NOTEBOOK_SETTINGS, false);
	BasicFileOps fileManager;
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
	std::vector<std::string> lessons;
	try {
		// create table if not exists (just on startup)
		connection.createAllLessonDb();
		lessons = connection.getLessons();
	} catch (ERRORS &error) {
		Dialogs::showErrorDialog(error);
		exit(0);
	}
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
