/*
 * SettingsPage.cpp
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#include "SettingsPage.h"
#include "../constants/constants.h"
#include "LessonPage.h"
/**
 * TODO: why gui.h is included? there must be a better solution
 */
#include "GUI.h"
#include "SettingsLessonTable.h"
#include "../sql/SQLiteConnect.h"
#include <gtkmm.h>
#include <iostream>



#define TABLE_LESSON_HEADER "Fach"
#define TABLE_DELETE_HEADER "Löschen"
#define TABLE_SETTINGS_POINT "Einstellungen"

/**
 * SettingsPage: Frame, which contains settings
 */
SettingsPage::SettingsPage(Gtk::Notebook* guiNotebook) :
		lessonTable(new LessonTable),
		newLessonLabel(new Gtk::Label()),
		newLessonEdit(new Gtk::Entry()),
		saveNewLessonButton(new Gtk::Button("Speichern")),
		mainBox(new Gtk::HBox()),
		LessonBox(new Gtk::VBox),
		expandBox(new Gtk::VBox),
		notebook(guiNotebook){

	newLessonLabel->set_text("Neues Fach:");
	saveNewLessonButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::saveButtonClicked));
	lessonTable->set_size_request(300, 300);
	LessonBox->pack_start(*lessonTable, Gtk::PACK_SHRINK, false, 0);
	Gtk::Button *deleteButton = Gtk::manage(new Gtk::Button("Löschen"));
	LessonBox->pack_start(*deleteButton, Gtk::PACK_SHRINK, false, 0);
	deleteButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::deleteButtonClicked));
	Gtk::HSeparator *newSeparator = Gtk::manage(new Gtk::HSeparator);
	newSeparator->set_size_request(300, 30);
	LessonBox->pack_start(*newSeparator, Gtk::PACK_SHRINK, false, 0);
	LessonBox->pack_start(*newLessonLabel, Gtk::PACK_SHRINK, false, 0);
	LessonBox->pack_start(*newLessonEdit, Gtk::PACK_SHRINK, false, 0);
	LessonBox->pack_start(*saveNewLessonButton, Gtk::PACK_SHRINK, false, 0);
	// add the table
	mainBox->pack_start(*LessonBox, Gtk::PACK_EXPAND_WIDGET, false, 0);
	expandBox->pack_start(*mainBox, Gtk::PACK_EXPAND_WIDGET, false, 0);
	this->add(*expandBox);
}

SettingsPage::~SettingsPage() {
	delete lessonTable;
	delete newLessonEdit;
	delete newLessonLabel;
	delete saveNewLessonButton;
	delete mainBox;
	delete LessonBox;
	delete expandBox;
}

/**
 * if the save-button is clicked, we try to connect to the database
 * and save the input value; then, we update the notebook and the LessonTable
 */
void SettingsPage::saveButtonClicked() {
	std::string newLesson = newLessonEdit->get_text();
	if(newLesson == "") {
		Dialogs::showErrorDialog("Fehlendes Fach", "Das Textfeld 'Fach' darf nicht leer sein.");
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
		showSuccessDialog("Speichern erfolgreich", "Das neue Fach wurde der Liste hinzugefügt.");
	} catch (ERRORS &error) {
		Dialogs::showErrorDialog(error);
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
		Dialogs::showErrorDialog(error);
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
		std::cout << lessons.at(i) << "|" << selectedLesson << std::endl;
		std::cout << lessons.size() << std::endl;
		if(lessons.at(i) == selectedLesson) {
			notebook->remove_page(i);
			break;
		}
	}

	notebook->show_all();
}

/*
 * @param title: title of the messagedialog
 * @param message: message of the messagedialog
 * Shows a dialog with a success-image
 */
void SettingsPage::showSuccessDialog(std::string title, std::string message) {
	Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
	dialog.set_title(title);
	dialog.set_secondary_text(message);
	dialog.run();
}
