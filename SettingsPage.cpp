/*
 * SettingsPage.cpp
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#include "SettingsPage.h"
#include "constants.h"
#include "LessonPage.h"
#include "GUI.h"
#include "SettingsLessonTable.h"
#include <gtkmm.h>



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
		notebook(guiNotebook) {

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
	mainBox->pack_start(*LessonBox, Gtk::PACK_SHRINK, false, 0);
	this->add(*mainBox);
}

SettingsPage::~SettingsPage() {
	delete lessonTable;
	delete newLessonEdit;
	delete newLessonLabel;
	delete saveNewLessonButton;
	delete mainBox;
	delete LessonBox;
}

/**
 * if the save-button is clicked, we try to connect to the database
 * and save the input value; then, we update the notebook and the LessonTable
 */
void SettingsPage::saveButtonClicked() {
	std::string newLesson = newLessonEdit->get_text();
	if(newLesson == "") {
		showErrorDialog("Fehlendes Fach", "Das Textfeld 'Fach' darf nicht leer sein.");
		return;
	}
	this->connection.open_db(Database::LESSON_DB);
	try {
		this->connection.addNewLesson(newLesson);
		newLessonEdit->set_text("");
		LessonPage *newLessonPage = Gtk::manage(new LessonPage(newLesson));
		notebook->insert_page(*newLessonPage, newLesson, notebook->get_n_pages() - 1);
		notebook->show_all();
		lessonTable->appendLesson(newLesson);
		showSuccessDialog("Speichern erfolgreich", "Das neue Fach wurde der Liste hinzugefügt.");
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
	}
	this->connection.close_db();
}


/**
 * Delete-Button clicked
 */
void SettingsPage::deleteButtonClicked() {
	lessonTable->deleteSelectedLesson();
	//notebook->remove_page();
}

/*
 * TODO: this function is redundant with same function in GUI.cpp. Make it better.
 */
void SettingsPage::showErrorDialog(std::string title, std::string message) {
	Gtk::MessageDialog dialog(title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
	dialog.set_title("Ein Fehler ist aufgetreten.");
	dialog.set_secondary_text(message);
	dialog.run();
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
