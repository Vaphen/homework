/*
 * SettingsPage.cpp
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#include "SettingsPage.h"
#include "../constants/constants.h"
#include "../constants/Labels.h"
#include "../helpers/HelpDialogs.h"
#include "LessonPage.h"
/**
 * TODO: why gui.h is included? there must be a better solution
 */
#include "GUI.h"
#include "SettingsLessonTable.h"
#include "../sql/SQLiteConnect.h"
#include <gtkmm.h>
#include <iostream>

/**
 * SettingsPage: Frame, which contains settings
 */
SettingsPage::SettingsPage(Gtk::Notebook* guiNotebook) :
		lessonTable(new LessonTable),
		notebook(guiNotebook) {

	initWidgets();

	// set additional options
	lessonTable->set_size_request(300, 300);
	separatorToNewLessonButtons->set_size_request(300, 30);
	separatorToSavePath->set_size_request(300, 30);

	deleteLessonButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::deleteButtonClicked));
	saveNewLessonButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::saveNewLessonButtonClicked));

	//saveFileDirPathButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::deleteButtonClicked));

	// Table with Lessons
	settingsVBox->pack_start(*lessonTable, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*deleteLessonButton, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*separatorToNewLessonButtons, Gtk::PACK_SHRINK, false, 0);
	// New Lesson Elements
	settingsVBox->pack_start(*newLessonLabel, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*newLessonEdit, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*saveNewLessonButton, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*separatorToSavePath, Gtk::PACK_SHRINK, false, 0);
	// exercise directory path settings area
	settingsVBox->pack_start(*fileDirPathLabel, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*fileDirPathEdit, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*saveFileDirPathButton, Gtk::PACK_SHRINK, false, 0);

	// center settingsVBox in window
	mainBox->pack_start(*settingsVBox, Gtk::PACK_EXPAND_WIDGET, false, 0);
	expandBox->pack_start(*mainBox, Gtk::PACK_EXPAND_WIDGET, false, 0);

	 // add the box to the frame
	this->add(*expandBox);
}

SettingsPage::~SettingsPage() {
	delete lessonTable;
}

void SettingsPage::initWidgets() {
	/**
	 * TODO: add constants for button string labels and bring the widgets to a understandable order
	 */
	lessonTable = Gtk::manage(new LessonTable);
	separatorToNewLessonButtons = Gtk::manage(new Gtk::HSeparator);
	newLessonLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::NEW_LESSON_LABEL));
	newLessonEdit = Gtk::manage(new Gtk::Entry);
	saveNewLessonButton = Gtk::manage(new Gtk::Button(SettingsPageLabels::NEW_LESSON_BUTTON));
	deleteLessonButton = Gtk::manage(new Gtk::Button(SettingsPageLabels::DELETE_BUTTON));
	separatorToSavePath = Gtk::manage(new Gtk::HSeparator);
	fileDirPathLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::PATH_TO_DIR_LABEL));
	fileDirPathEdit = Gtk::manage(new Gtk::Entry);
	saveFileDirPathButton = Gtk::manage(new Gtk::Button(SettingsPageLabels::SAVE_PATH_TO_DIR_BUTTON));
	settingsVBox = Gtk::manage(new Gtk::VBox);
	mainBox = Gtk::manage(new Gtk::HBox());
	expandBox = Gtk::manage(new Gtk::VBox);
}

/**
 * if the save-button is clicked, we try to connect to the database
 * and save the input value; then, we update the notebook and the LessonTable
 */
void SettingsPage::saveNewLessonButtonClicked() {
	std::string newLesson = newLessonEdit->get_text();
	if(newLesson == "") {
		HelpDialogs::showErrorDialog(SettingsPageLabels::NEW_LESSON_ERROR_TITLE,
				SettingsPageLabels::NEW_LESSON_ERROR_MESSAGE);
		return;
	}
	try {
		this->connection.addNewLesson(newLesson);
		this->connection.createSpecificLessonTable(newLesson);
		newLessonEdit->set_text("");
		LessonPage *newLessonPage = Gtk::manage(new LessonPage(newLesson));
		notebook->insert_page(*newLessonPage, newLesson, notebook->get_n_pages() - 1);
		notebook->show_all();
		lessonTable->appendLesson(newLesson);
		HelpDialogs::showSuccessDialog(SettingsPageLabels::SAVING_SUCCESS_TITLE,
				SettingsPageLabels::SAVING_SUCCESS_MESSAGE);
	} catch (ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
}


/**
 * Delete-Button clicked
 */
void SettingsPage::deleteButtonClicked() {
	Glib::ustring selectedLesson = lessonTable->getSelectedLesson();
	// nothing has been selected
	if(selectedLesson == "")
		return;

	/**
	 * connect to the Database and try to delete the lesson
	 */
	SQLiteConnect connection;
	std::vector<std::string> lessons = {};

	try {
		lessons = connection.getLessons();
		connection.deleteLesson(std::string(selectedLesson));
		connection.deleteSpecificLessonTable(std::string(selectedLesson));
	} catch (ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
		return;
	}


	// if sql-delete was successfull, we can delete it from the table
	lessonTable->deleteSelectedLesson();

	/*
	 * there is no way to delete notebook-page by its label.
	 * we need to iterate through all lessons to check if it's
	 * the deleted one
	 */
	for(int i = 0; i < lessons.size(); i++) {
		if(lessons.at(i) == selectedLesson) {
			notebook->remove_page(i);
			break;
		}
	}

	notebook->show_all();
}
