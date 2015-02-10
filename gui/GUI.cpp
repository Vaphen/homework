/*  * gui.cpp  *  *  Created on: 25.12.2014  *      Author: vaphen  */

#include "GUI.h"
#include "LessonPage.h"
#include "SettingsPage.h"
#include "../sql/SQLiteConnect.h"
#include "../constants/constants.h"
#include "../constants/Labels.h"
#include "../helpers/HelpDialogs.h"
#include "../fileOperations/BasicFileOps.h"
#include <gtkmm.h>
#include <iostream>


GUI::GUI() :
		notebook(new Gtk::Notebook()),
		settings_frame(nullptr) {

	BasicFileOps fileManager;
	if(!fileManager.isFileExistant(CONFIG_FILE)) {
		fileManager.callConfigParser().createDefaultConfigFile();
	}

	// add all lessons
	for(std::string &lesson : doSqlLessonRequest()) {
		addLessonPage(lesson);
	}

	// could not use initialization list because if theres a sql-error,
	// it shows it twice. thats why its initialized here.
	Gtk::Label *settingsLabel = Gtk::manage(new Gtk::Label(GuiLabels::SETTINGS));
	settings_frame = new SettingsPage(notebook);
	statistics_frame = new StatisticsPage;


	// add settings page
	notebook->append_page(*settings_frame, GuiLabels::SETTINGS, false);
	notebook->append_page(*statistics_frame, GuiLabels::STATISTICS, false);

	set_title(WINDOW_TITLE);
	add(*notebook);
	show_all();
	maximize();
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
		HelpDialogs::showErrorDialog(error);
		exit(0);
	}
	return lessons;
}


void GUI::addLessonPage(std::string newLesson) {
	LessonPage *newFrame = Gtk::manage(new LessonPage(newLesson, notebook));
	notebook->append_page(*newFrame, newLesson, true);
}


/**
 * delete all widget-pointer
 */
GUI::~GUI() {
	delete notebook;
	delete settings_frame;
	delete statistics_frame;
}
