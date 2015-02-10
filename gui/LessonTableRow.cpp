/*
 * LessonTableRow.cpp
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#include "LessonTableRow.h"
#include "LessonPage.h"
#include "../constants/constants.h"
#include "../constants/Labels.h"
#include "../fileOperations/BasicFileOps.h"
#include "../fileOperations/ConfigFileParser.h"
#include "../helpers/HelpDialogs.h"
#include "../helpers/TimeConvert.h"
#include <string>
#include <gtkmm.h>
#include <iostream>

/// Constructor for new row (from SQL-Query)
/**
 * Should be used rather than the other constructor.
 * @param row a vector containing all infos about the row that should be added
 */
LessonTableRow::LessonTableRow(std::vector<std::string> row) :
		stateChanged(false) {
	idInSqlDB = atoi(row.at(COLUMN_ID::ID).c_str());
	TimeConvert timeConverter;
	toDoUntil = timeConverter.unixToGermanDateFormat(row.at(COLUMN_ID::UNTIL));
	reachedPoints = atoi(row.at(COLUMN_ID::REACHED_POINTS).c_str());
	totalPoints = atoi(row.at(COLUMN_ID::TOTAL_POINTS).c_str());
	isExerciseFinished = (row.at(COLUMN_ID::EXERCISE_FINISHED) == "1") ? true : false;
	exerciseComment = row.at(COLUMN_ID::EXERCISE_COMMENT);
	lessonName = row.at(COLUMN_ID::LESSON);

	initializeWidgets();
}
/// Constructor for new, not yet in database existing lesson
/**
 * This constructor should only be called if a new exercise has been added.
 * All values are set to 0, except the until-value, the id and the lessonName-value
 * @param until string of the date until the exercise should be finished
 * @param id The id of the exercise in the database
 * @param lesson name of the lesson the row was added to
 */
LessonTableRow::LessonTableRow(const std::string &until, int id, const std::string &lesson) :
			stateChanged(false) {
	/**
	 * TODO: add folderPath to this function and set it(as parameter etc.)
	 */
	idInSqlDB = id;
	toDoUntil = until;
	reachedPoints = 0;
	totalPoints = 0;
	isExerciseFinished = false;
	exerciseComment = "";
	lessonName = lesson;
	initializeWidgets();
}

/// Initializes all widgets for one row setting properties too.
void LessonTableRow::initializeWidgets() {
	untilLabel = Gtk::manage(new Gtk::Label(toDoUntil));
	reachedPointsSpin = Gtk::manage(new Gtk::SpinButton);
	totalPointsSpin = Gtk::manage(new Gtk::SpinButton);
	openFolderButton = Gtk::manage(new Gtk::Button);
	openFolderButtonImage = Gtk::manage(new Gtk::Image(OPENDIR_ICO));
	openExercisePDFButton = Gtk::manage(new Gtk::Button);
	openExercisePDFImage = Gtk::manage(new Gtk::Image(OPEN_PDF_ICO));
	exerciseFinishedButton = Gtk::manage(new Gtk::CheckButton);
	commentTextView = Gtk::manage(new Gtk::TextView);
	defaultText = Gtk::TextBuffer::create();
	commentScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow);

	reachedPointsSpin->set_size_request(80, 33);
	reachedPointsSpin->set_max_length(4);
	reachedPointsSpin->set_value(reachedPoints);
	reachedPointsSpin->set_increments(1, 1);
	reachedPointsSpin->set_range(0, 1000);
	reachedPointsSpin->set_editable(false);
	reachedPointsSpin->set_value(reachedPoints);
	reachedPointsSpin->signal_changed().connect(sigc::mem_fun(*this, &LessonTableRow::changeState));

	totalPointsSpin->set_size_request(80, 33);
	totalPointsSpin->set_max_length(4);
	totalPointsSpin->set_value(totalPoints);
	totalPointsSpin->set_increments(1, 1);
	totalPointsSpin->set_range(0, 1000);
	totalPointsSpin->set_editable(false);
	totalPointsSpin->set_value(totalPoints);
	totalPointsSpin->signal_changed().connect(sigc::mem_fun(*this, &LessonTableRow::changeState));

	openFolderButton->set_image(*openFolderButtonImage);
	openFolderButton->set_size_request(50, 50);
	openFolderButton->set_relief(Gtk::RELIEF_NONE);
	openFolderButton->set_tooltip_text(LessonTableRowLabels::OPEN_FOLDER_TOOLTIP);
	openFolderButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonTableRow::openFolderButtonClicked));

	openExercisePDFButton->set_image(*openExercisePDFImage);
	openExercisePDFButton->set_size_request(50, 50);
	openExercisePDFButton->set_relief(Gtk::RELIEF_NONE);
	openExercisePDFButton->set_tooltip_text(LessonTableRowLabels::OPEN_PDF_TOOLTIP);
	openExercisePDFButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonTableRow::openExercisePDFButtonClicked));

	exerciseFinishedButton->set_active(isExerciseFinished);
	exerciseFinishedButton->signal_toggled().connect(sigc::mem_fun(*this, &LessonTableRow::changeState));


	defaultText->set_text(exerciseComment);
	commentTextView->set_size_request(130, 50);
	commentTextView->set_border_width(1);
	commentTextView->set_justification(Gtk::JUSTIFY_FILL);
	commentTextView->set_buffer(defaultText);
	commentTextView->signal_grab_focus().connect(sigc::mem_fun(*this, &LessonTableRow::changeState));

	commentScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	commentScrolledWindow->add(*commentTextView);
}

Gtk::Label* LessonTableRow::getUntilLabel() const {
	return untilLabel;
}

Gtk::SpinButton* LessonTableRow::getReachedPointsSpin() const {
	return reachedPointsSpin;
}

Gtk::SpinButton* LessonTableRow::getTotalPointsSpin() const {
	return totalPointsSpin;
}

Gtk::Button* LessonTableRow::getOpenFolderButton() const {
	return openFolderButton;
}

Gtk::Button* LessonTableRow::getOpenExercisePDF() const {
	return openExercisePDFButton;
}

Gtk::CheckButton* LessonTableRow::getExerciseFinishedButton() const {
	return exerciseFinishedButton;
}

Gtk::ScrolledWindow* LessonTableRow::getCommentScrolledWindow() const {
	return commentScrolledWindow;
}

int LessonTableRow::getID() const {
	return idInSqlDB;
}

unsigned int LessonTableRow::getReachedPoints() const {
	return reachedPoints;
}

void LessonTableRow::setReachedPoints(unsigned int &newPoints) {
	this->reachedPoints = newPoints;
}

unsigned int LessonTableRow::getTotalPoints() const {
	return totalPoints;
}

void LessonTableRow::setTotalPoints(unsigned int &newPoints) {
	this->totalPoints = newPoints;
}

bool LessonTableRow::getIsExerciseFinished() const {
	return isExerciseFinished;
}

void LessonTableRow::setIsExerciseFinished(bool &newFinished) {
	this->isExerciseFinished = newFinished;
}

std::string LessonTableRow::getComment() const {
	return exerciseComment;
}

void LessonTableRow::setComment(std::string &newComment) {
	this->exerciseComment = newComment;
}

bool LessonTableRow::getStateChanged() const {
	return stateChanged;
}

void LessonTableRow::setStateChanged(bool state) {
	stateChanged = state;
}

std::string LessonTableRow::getUntil() const {
	return toDoUntil;
}

/// Opens the lesson specific folder.
void LessonTableRow::openFolderButtonClicked() {
	BasicFileOps fileOps;
	ConfigFileParser configParser;
	std::string exerciseFolderPath = configParser.getSaveDirectoryPath() + "/" + lessonName + "/" + toDoUntil;
	fileOps.openFileManager(exerciseFolderPath);
}

/// Opens the PDF-File if there is one selected before. If not, it shows a file chooser dialog.
void LessonTableRow::openExercisePDFButtonClicked() {
	BasicFileOps fileOps;
	ConfigFileParser configParser;
	std::string exercise_file_path = configParser.getSaveDirectoryPath() + "/" + lessonName + "/" + toDoUntil + "/" + EXERCISE_PDF_FILE;
	if(fileOps.isFileExistant(exercise_file_path)) {
		fileOps.openPdfFile(exercise_file_path);
	}else {
		std::string chosenFile = "";
		if((chosenFile = HelpDialogs::showFileChooser()) != "") {
			fileOps.copyFile(chosenFile, configParser.getSaveDirectoryPath() + "/" + lessonName + "/" + toDoUntil + "/" + EXERCISE_PDF_FILE);
		}
	}
}

void LessonTableRow::changeState() {
	stateChanged = true;
}

LessonTableRow::~LessonTableRow() { }

