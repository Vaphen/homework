/*
 * SettingsPage.cpp
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#include "SettingsPage.h"
#include "../../constants/constants.h"
#include "../../constants/Labels.h"
#include "../../helpers/HelpDialogs.h"
#include "../../sql/SQLiteConnect.h"
#include "../../fileOperations/ConfigFileParser.h"
#include "../../fileOperations/BasicFileOps.h"
#include "../Exercises/ExerciseFrame.h"
#include "../Exams/ExamPage.h"
#include "SettingsLessonTable.h"
#include <gtkmm.h>
#include <boost/regex.hpp>
#include "../Exercises/ExercisePage.h"

/**
 * SettingsPage: Frame, which contains settings
 * @param exerciseFrame a reference to the exerciseFrame. needed to add some
 * 		new lessons to the notebook.
 */
SettingsPage::SettingsPage(ExerciseFrame &exerciseFrame, ExamPage &examPage) :
	exerciseFrame(exerciseFrame),
	examFrame(examPage),
	lessonTable(new SettingsLessonTable) {

	set_border_width(10);

	// allocates "global" widgets
	initWidgets();

	// newLessonBox and widgets creation
	initializeNewLessonBox();

	// environmentSettingsFrame and widgets creation
	initializeEnvironmentSettings();

	// onlineSettingsFrame and widgets creation
	initializeOnlineSettings();

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
	onlineSettingsFrame = Gtk::manage(new Gtk::Frame);
	enableOnlineSync = Gtk::manage(new Gtk::CheckButton(SettingsPageLabels::ENABLE_ONLINE_SYNC));
	usernameEdit = Gtk::manage(new Gtk::Entry);
	passwordEdit = Gtk::manage(new Gtk::Entry);
	stayLoggedIn = Gtk::manage(new Gtk::CheckButton(SettingsPageLabels::STAY_LOGGED_IN));
	loginButton = Gtk::manage(new Gtk::Button(SettingsPageLabels::LOGIN_BUTTON));
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

void SettingsPage::initializeOnlineSettings() {
	onlineSettingsFrame->set_border_width(10);
	onlineSettingsFrame->set_label(SettingsPageLabels::ONLINE_SETTINGS_HEADING);
	onlineSettingsFrame->set_size_request(400, -1);

	Gtk::Label *usernameLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::USERNAME_LABEL));
	Gtk::Label *passwordLabel = Gtk::manage(new Gtk::Label(SettingsPageLabels::PASSWORD_LABEL));
	Gtk::VBox *onlineVBox = Gtk::manage(new Gtk::VBox);

	enableOnlineSync->signal_clicked().connect(sigc::mem_fun(*this, &SettingsPage::enableOnlineSyncClicked));

	usernameEdit->set_sensitive(false);
	passwordEdit->set_sensitive(false);
	stayLoggedIn->set_sensitive(false);
	loginButton->set_sensitive(false);
	passwordEdit->set_visibility(false);

	onlineVBox->pack_start(*enableOnlineSync, Gtk::PACK_SHRINK, false, 20);
	onlineVBox->pack_start(*usernameLabel, Gtk::PACK_SHRINK, false, 0);
	onlineVBox->pack_start(*usernameEdit, Gtk::PACK_SHRINK, false, 0);
	onlineVBox->pack_start(*passwordLabel, Gtk::PACK_SHRINK, false, 0);
	onlineVBox->pack_start(*passwordEdit, Gtk::PACK_SHRINK, false, 0);
	onlineVBox->pack_start(*stayLoggedIn, Gtk::PACK_SHRINK, false, 10);
	onlineVBox->pack_start(*loginButton, Gtk::PACK_SHRINK, false, 20);

	onlineSettingsFrame->add(*onlineVBox);
}

void SettingsPage::showCenteredWidgets() {
	Gtk::HBox *mainBox = Gtk::manage(new Gtk::HBox);
	Gtk::HBox *shrinkTogetherBox = Gtk::manage(new Gtk::HBox);
	Gtk::VBox *expandVBox = Gtk::manage(new Gtk::VBox);

	shrinkTogetherBox->pack_start(*settingsVBox, Gtk::PACK_SHRINK, false, 20);
	shrinkTogetherBox->pack_start(*environmentSettingsFrame, Gtk::PACK_SHRINK, false, 20);
	shrinkTogetherBox->pack_start(*onlineSettingsFrame, Gtk::PACK_SHRINK, false, 20);
	mainBox->pack_start(*shrinkTogetherBox, Gtk::PACK_EXPAND_WIDGET, false, 0);

	expandVBox->pack_start(*mainBox, Gtk::PACK_EXPAND_WIDGET, false, 0);

	// add the box to the frame
	this->add(*expandVBox);
}

// Triggered when the save new lesson button was clicked
/**
 * If the save-button is clicked we try to connect to the database
 * and save the input value; then, we update the notebook of the ExerciseFrame and the LessonTable
 */
void SettingsPage::saveNewLessonButtonClicked() {
	std::string newLesson = newLessonEdit->get_text();
	boost::regex alphabeticStringRegex("^[A-z]+$");

	if(newLesson == "") {
		HelpDialogs::showErrorDialog(SettingsPageLabels::NEW_LESSON_ERROR_TITLE,
				SettingsPageLabels::NEW_LESSON_ERROR_MESSAGE);
		return;
	}else if(!boost::regex_match(newLesson, alphabeticStringRegex)) {
		HelpDialogs::showErrorDialog("Das eingegebene Fach ist unzulässig.", "Es dürfen nur alphabetische Lettern im Fach vorkommen (Zahlen, Sonderzeichen etc. sind nicht erlaubt).");
		return;
	}

	try {
		this->connection.addNewLesson(newLesson);
		this->connection.createSpecificLessonTable(newLesson);

		BasicFileOps fileOps;
		fileOps.createFolder(fileOps.callConfigParser().getSaveDirectoryPath() + "/" + newLesson);
	} catch (ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
		return;
	} catch(FILE_ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
	newLessonEdit->set_text("");
	exerciseFrame.addLessonPage(newLesson);
	examFrame.appendLessonToLessonCombo(newLesson);
	lessonTable->appendLesson(newLesson);
	HelpDialogs::showInfoDialog(SettingsPageLabels::SAVING_SUCCESS_TITLE,
			SettingsPageLabels::SAVING_SUCCESS_MESSAGE);
}


/// Triggered when delete button was clicked
void SettingsPage::deleteButtonClicked() {
	Glib::ustring selectedLesson = lessonTable->getSelectedLesson();
	// nothing has been selected
	if(selectedLesson == "") {
		HelpDialogs::showInfoDialog("Kein Fach ausgewählt", "Das zu löschende Fach muss zuerst selektiert werden.");
		return;
	}
	switch(HelpDialogs::showMultipleDeleteDialog(SettingsPageLabels::MULTIPLE_DELETE_TITLE, SettingsPageLabels::MULTIPLE_DELETE_MESSAGE)) {
	case int(ANSWERS::CANCEL):
			return;
	case int(ANSWERS::DELETE_ALL): {
		// this case runs through to case delete (no break).
		// That's why it deletes all, inclusive the cell.
		BasicFileOps fileOps;
		try {
			fileOps.deleteFolder(fileOps.callConfigParser().getSaveDirectoryPath() + "/" + selectedLesson);
		}catch(const FILE_ERRORS &error) {
			HelpDialogs::showErrorDialog(error);
		}
	}
	case int(ANSWERS::DELETE): {
		/**
		 * connect to the Database and try to delete the lesson
		 */
		SQLiteConnect connection;

		try {
			connection.deleteLesson(std::string(selectedLesson));
			connection.deleteSpecificLessonTable(std::string(selectedLesson));
		} catch (ERRORS &error) {
			HelpDialogs::showErrorDialog(error);
			return;
		}


		// if sql-delete was successfull, we can delete it from the table
		lessonTable->deleteSelectedLesson();

		// remove page from notebook
		exerciseFrame.removeLessonPage(selectedLesson);

		// remove lesson from combobox of exams
		examFrame.removeLessonFromLessonCombo(selectedLesson);

		break;
	}
	default:
		return;
	}
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

void SettingsPage::enableOnlineSyncClicked() {

	if(enableOnlineSync->get_active()) {
		usernameEdit->set_sensitive(true);
		passwordEdit->set_sensitive(true);
		stayLoggedIn->set_sensitive(true);
		loginButton->set_sensitive(true);

	}else{
		usernameEdit->set_sensitive(false);
		passwordEdit->set_sensitive(false);
		stayLoggedIn->set_sensitive(false);
		loginButton->set_sensitive(false);
		passwordEdit->set_text("");
		stayLoggedIn->set_active(false);
	}

}
