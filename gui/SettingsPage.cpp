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
#include "../sql/SQLiteConnect.h"
#include "LessonPage.h"
#include "SettingsLessonTable.h"
#include <gtkmm.h>
#include <iostream>
#include "../fileOperations/ConfigFileParser.h"


/**
 * SettingsPage: Frame, which contains settings
 */
SettingsPage::SettingsPage(Gtk::Notebook* guiNotebook) :
		lessonTable(new LessonTable),
		notebook(guiNotebook) {

	set_border_width(10);

	// allocates "global" widgets
	initWidgets();

	// newLessonBox and widgets creation
	initializeNewLessonBox();

	// environmentSettingsFrame and widgets creation
	initializeEnvironmentSettings();

	// center all widgets in window
	showCenteredWidgets();
}

SettingsPage::~SettingsPage() {
	delete lessonTable;
}

void SettingsPage::initWidgets() {
	newLessonEdit = Gtk::manage(new Gtk::Entry);
	environmentSettingsFrame = Gtk::manage(new Gtk::Frame);
	fileDirPathEdit = Gtk::manage(new Gtk::Entry);
	pdfExecutablePathEdit = Gtk::manage(new Gtk::Entry);
	fileManagerPathEdit = Gtk::manage(new Gtk::Entry);
	settingsVBox = Gtk::manage(new Gtk::VBox);
}

void SettingsPage::initializeNewLessonBox() {
	Gtk::HSeparator *separatorToNewLessonButtons = Gtk::manage(new Gtk::HSeparator);
	Gtk::Label *newLessonLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::NEW_LESSON_LABEL));
	Gtk::Button *deleteLessonButton = Gtk::manage(new Gtk::Button(SettingsPageLabels::DELETE_BUTTON));
	Gtk::Button *saveNewLessonButton = Gtk::manage(new Gtk::Button(SettingsPageLabels::NEW_LESSON_BUTTON));

	lessonTable->set_size_request(300, 300);

	separatorToNewLessonButtons->set_size_request(300, 30);

	deleteLessonButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::deleteButtonClicked));
	saveNewLessonButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::saveNewLessonButtonClicked));

	// set outer border to 10 (better look)
	settingsVBox->set_border_width(10);
	// Table with Lessons
	settingsVBox->pack_start(*lessonTable, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*deleteLessonButton, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*separatorToNewLessonButtons, Gtk::PACK_SHRINK, false, 0);
	// New Lesson Elements
	settingsVBox->pack_start(*newLessonLabel, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*newLessonEdit, Gtk::PACK_SHRINK, false, 0);
	settingsVBox->pack_start(*saveNewLessonButton, Gtk::PACK_SHRINK, false, 0);
}

/// Initializes fundamental settings of the environment-settings widgets
void SettingsPage::initializeEnvironmentSettings() {
	Gtk::Label *fileDirPathLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::PATH_TO_DIR_LABEL));
	Gtk::Button *chooseFileDirPathButton = Gtk::manage(new Gtk::Button);
	Gtk::Image *openDirIco = Gtk::manage(new Gtk::Image(OPENDIR_ICO_SMALL));
	Gtk::Button *saveEnvironmentSettings = Gtk::manage(new Gtk::Button(SettingsPageLabels::SAVE_ENVIRONMENT_SETTINGS));
	Gtk::HBox *fileDirPathBox = Gtk::manage(new Gtk::HBox);

	Gtk::HSeparator *firstSeparator = Gtk::manage(new Gtk::HSeparator);
	Gtk::Label *pdfExecutableLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::PDF_EXECUTABLE_LABEL));
	Gtk::Button *choosePdfExecutableButton = Gtk::manage(new Gtk::Button);
	Gtk::Image *choosePdfExecutableImage = Gtk::manage(new Gtk::Image(OPENDIR_ICO_SMALL));
	Gtk::HBox *pdfExecutableBox = Gtk::manage(new Gtk::HBox);

	Gtk::HSeparator *secondSeparator = Gtk::manage(new Gtk::HSeparator);
	Gtk::Label *fileManagerLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::FILE_MANAGER_LABEL));
	Gtk::Button *chooseFileManagerButton = Gtk::manage(new Gtk::Button);
	Gtk::Image *chooseFileManagerImage = Gtk::manage(new Gtk::Image(OPENDIR_ICO_SMALL));
	Gtk::HBox *fileManagerBox = Gtk::manage(new Gtk::HBox);

	Gtk::VBox *environmentSettingsVBox = Gtk::manage(new Gtk::VBox);


	ConfigFileParser configParser;

	environmentSettingsFrame->set_border_width(10);
	environmentSettingsFrame->set_label(SettingsPageLabels::ENVIRONMENT_SETTINGS_HEADING);
	environmentSettingsFrame->set_size_request(400, -1);

	fileDirPathEdit->set_sensitive(false);
	fileDirPathEdit->set_size_request(320, -1);
	fileDirPathEdit->set_text(configParser.getSaveDirectoryPath());

	chooseFileDirPathButton->set_image(*openDirIco);
	chooseFileDirPathButton->set_relief(Gtk::RELIEF_NONE);
	chooseFileDirPathButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::chooseFileDirButtonClicked));

	fileDirPathBox->pack_start(*fileDirPathEdit, Gtk::PACK_START, false, 0);
	fileDirPathBox->pack_start(*chooseFileDirPathButton, Gtk::PACK_SHRINK, false, 0);

	firstSeparator->set_size_request(300, 10);

	pdfExecutablePathEdit->set_sensitive(false);
	pdfExecutablePathEdit->set_size_request(320, -1);
	pdfExecutablePathEdit->set_text(configParser.getPdfExecutablePath());

	choosePdfExecutableButton->set_image(*choosePdfExecutableImage);
	choosePdfExecutableButton->set_relief(Gtk::RELIEF_NONE);
	choosePdfExecutableButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::choosePdfExecutableButtonClicked));

	pdfExecutableBox->pack_start(*pdfExecutablePathEdit, Gtk::PACK_SHRINK, false, 0);
	pdfExecutableBox->pack_start(*choosePdfExecutableButton, Gtk::PACK_SHRINK, false, 0);

	secondSeparator->set_size_request(300, 10);

	fileManagerPathEdit->set_sensitive(false);
	fileManagerPathEdit->set_size_request(320, -1);
	fileManagerPathEdit->set_text(configParser.getFileManagerPath());

	chooseFileManagerButton->set_image(*chooseFileManagerImage);
	chooseFileManagerButton->set_relief(Gtk::RELIEF_NONE);
	chooseFileManagerButton->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::chooseFileManagerButtonClicked));

	fileManagerBox->pack_start(*fileManagerPathEdit, Gtk::PACK_SHRINK, false, 0);
	fileManagerBox->pack_start(*chooseFileManagerButton, Gtk::PACK_SHRINK, false, 0);


	saveEnvironmentSettings->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::saveEnvironmentSettingsClicked));

	environmentSettingsVBox->set_border_width(10);
	environmentSettingsVBox->pack_start(*fileDirPathLabel, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*fileDirPathBox, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*firstSeparator, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*pdfExecutableLabel, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*pdfExecutableBox, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*secondSeparator, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*fileManagerLabel, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*fileManagerBox, Gtk::PACK_SHRINK, false, 0);
	environmentSettingsVBox->pack_start(*saveEnvironmentSettings, Gtk::PACK_SHRINK, false, 20);
	environmentSettingsFrame->add(*environmentSettingsVBox);
}

void SettingsPage::showCenteredWidgets() {
	Gtk::HBox *mainBox = Gtk::manage(new Gtk::HBox);
	Gtk::HBox *shrinkTogetherBox = Gtk::manage(new Gtk::HBox);
	Gtk::VBox *expandVBox = Gtk::manage(new Gtk::VBox);

	shrinkTogetherBox->pack_start(*settingsVBox, Gtk::PACK_SHRINK, false, 20);
	shrinkTogetherBox->pack_start(*environmentSettingsFrame, Gtk::PACK_SHRINK, false, 20);
	mainBox->pack_start(*shrinkTogetherBox, Gtk::PACK_EXPAND_WIDGET, false, 0);

	expandVBox->pack_start(*mainBox, Gtk::PACK_EXPAND_WIDGET, false, 0);

	// add the box to the frame
	this->add(*expandVBox);
}

// Triggered when the save new lesson button was clicked
/**
 * If the save-button is clicked we try to connect to the database
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
		HelpDialogs::showInfoDialog(SettingsPageLabels::SAVING_SUCCESS_TITLE,
				SettingsPageLabels::SAVING_SUCCESS_MESSAGE);
	} catch (ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
}


/// Triggered when delete button was clicked
void SettingsPage::deleteButtonClicked() {
	Glib::ustring selectedLesson = lessonTable->getSelectedLesson();
	// nothing has been selected
	if(selectedLesson == "") {
		HelpDialogs::showInfoDialog("Kein Fach ausgewählt", "Das zu löschende Fach muss zuerst selektiert werden.");
		return;
	}
	if(HelpDialogs::showConfirmDialog(HelpDialogs::CONFIRM_DELETION,
			HelpDialogs::CONFIRM_LESSON_DELETION_SUBTEXT) != HelpDialogs::CONFIRMED) {
		return;
	}



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

/// Triggered when choose file button was clicked
void SettingsPage::chooseFileDirButtonClicked() {
	std::string chosenPath = "";
	// if the cancel-button is clicked, it returns an empty string
	if((chosenPath = HelpDialogs::showFolderChooser()) != "") {
		fileDirPathEdit->set_text(chosenPath);
	}
}

/// Triggered when save settings button was clicked
void SettingsPage::saveEnvironmentSettingsClicked() {
	ConfigFileParser configParser;
	configParser.setSaveDirectoryPath(fileDirPathEdit->get_text());
	configParser.setPdfExecutablePath(pdfExecutablePathEdit->get_text());
	configParser.setFileManagerPath(fileManagerPathEdit->get_text());
}

void SettingsPage::choosePdfExecutableButtonClicked() {
	std::string chosenPath = "";
	// if the cancel-button is clicked, it returns an empty string
	if((chosenPath = HelpDialogs::showExecutableChooser("Anwendung zum Öffnen von PDF-Dateien auswählen")) != "") {
		pdfExecutablePathEdit->set_text(chosenPath);
	}
}

void SettingsPage::chooseFileManagerButtonClicked() {
	std::string chosenPath = "";
	// if the cancel-button is clicked, it returns an empty string
	if((chosenPath = HelpDialogs::showExecutableChooser("Anwendung zum Öffnen von Ordnern auswählen")) != "") {
		fileManagerPathEdit->set_text(chosenPath);
	}
}
