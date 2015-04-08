/*
 * LessonTableRow.cpp
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#include "ExerciseTableRow.h"

#include "../../constants/constants.h"
#include "../../constants/Labels.h"
#include "../../fileOperations/BasicFileOps.h"
#include "../../fileOperations/ConfigFileParser.h"
#include "../../helpers/HelpDialogs.h"
#include "../../helpers/TimeConvert.h"
#include <string>
#include <gtkmm.h>
#include <iostream>
#include "ExercisePage.h"

/// Constructor for new row (from SQL-Query)
/**
 * Should be used rather than the other constructor.
 * @param row a vector containing all infos about the row that should be added
 */
ExerciseTableRow::ExerciseTableRow(std::vector<std::string> row) :
		stateChanged(false) {
	idInSqlDB = atoi(row.at(COLUMN_ID_LESSON::ID).c_str());
	TimeConvert timeConverter;
	toDoUntil = timeConverter.unixToGermanDateFormat(row.at(COLUMN_ID_LESSON::UNTIL));
	reachedPoints = atoi(row.at(COLUMN_ID_LESSON::REACHED_POINTS).c_str());
	totalPoints = atoi(row.at(COLUMN_ID_LESSON::TOTAL_POINTS).c_str());
	isExerciseFinished = (row.at(COLUMN_ID_LESSON::EXERCISE_FINISHED) == "1") ? true : false;
	exerciseComment = row.at(COLUMN_ID_LESSON::EXERCISE_COMMENT);
	lessonName = row.at(COLUMN_ID_LESSON::LESSON);

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
ExerciseTableRow::ExerciseTableRow(const std::string &until, int id, const std::string &lesson) :
			stateChanged(false) {
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
void ExerciseTableRow::initializeWidgets() {
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
	reachedPointsSpin->set_increments(1, 1);
	reachedPointsSpin->set_range(0, 1000);
	reachedPointsSpin->set_editable(false);
	reachedPointsSpin->set_value(reachedPoints);
	// changeState is called by the reachedPointsChanged function.
	reachedPointsSpin->signal_changed().connect(sigc::mem_fun(*this, &ExerciseTableRow::reachedPointsChanged));

	totalPointsSpin->set_size_request(80, 33);
	totalPointsSpin->set_max_length(4);
	totalPointsSpin->set_value(totalPoints);
	totalPointsSpin->set_increments(1, 1);
	totalPointsSpin->set_range(0, 1000);
	totalPointsSpin->set_editable(false);
	totalPointsSpin->set_value(totalPoints);
	// changeState is called by the totalPointsChanged function.
	totalPointsSpin->signal_changed().connect(sigc::mem_fun(*this, &ExerciseTableRow::totalPointsChanged));

	openFolderButton->set_image(*openFolderButtonImage);
	openFolderButton->set_size_request(50, 50);
	openFolderButton->set_relief(Gtk::RELIEF_NONE);
	openFolderButton->set_tooltip_text(LessonTableRowLabels::OPEN_FOLDER_TOOLTIP);
	openFolderButton->signal_clicked().connect(sigc::mem_fun(*this, &ExerciseTableRow::openFolderButtonClicked));

	openExercisePDFButton->set_image(*openExercisePDFImage);
	openExercisePDFButton->set_size_request(50, 50);
	openExercisePDFButton->set_relief(Gtk::RELIEF_NONE);
	openExercisePDFButton->set_tooltip_text(LessonTableRowLabels::OPEN_PDF_TOOLTIP);
	openExercisePDFButton->signal_clicked().connect(sigc::mem_fun(*this, &ExerciseTableRow::openExercisePDFButtonClicked));

	exerciseFinishedButton->set_active(isExerciseFinished);
	exerciseFinishedButton->signal_toggled().connect(sigc::mem_fun(*this, &ExerciseTableRow::changeState));


	defaultText->set_text(exerciseComment);
	commentTextView->set_size_request(130, 50);
	commentTextView->set_border_width(1);
	commentTextView->set_justification(Gtk::JUSTIFY_FILL);
	commentTextView->set_buffer(defaultText);
	commentTextView->signal_grab_focus().connect(sigc::mem_fun(*this, &ExerciseTableRow::changeState));

	commentScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	commentScrolledWindow->add(*commentTextView);
}

Gtk::Label* ExerciseTableRow::getUntilLabel() const {
	return untilLabel;
}

Gtk::SpinButton* ExerciseTableRow::getReachedPointsSpin() const {
	return reachedPointsSpin;
}

Gtk::SpinButton* ExerciseTableRow::getTotalPointsSpin() const {
	return totalPointsSpin;
}

Gtk::Button* ExerciseTableRow::getOpenFolderButton() const {
	return openFolderButton;
}

Gtk::Button* ExerciseTableRow::getOpenExercisePDF() const {
	return openExercisePDFButton;
}

Gtk::CheckButton* ExerciseTableRow::getExerciseFinishedButton() const {
	return exerciseFinishedButton;
}

Gtk::ScrolledWindow* ExerciseTableRow::getCommentScrolledWindow() const {
	return commentScrolledWindow;
}

int ExerciseTableRow::getID() const {
	return idInSqlDB;
}

unsigned int ExerciseTableRow::getReachedPoints() const {
	return reachedPoints;
}

void ExerciseTableRow::setReachedPoints(unsigned int &newPoints) {
	this->reachedPoints = newPoints;
}

unsigned int ExerciseTableRow::getTotalPoints() const {
	return totalPoints;
}

void ExerciseTableRow::setTotalPoints(unsigned int &newPoints) {
	this->totalPoints = newPoints;
}

bool ExerciseTableRow::getIsExerciseFinished() const {
	return isExerciseFinished;
}

void ExerciseTableRow::setIsExerciseFinished(bool &newFinished) {
	this->isExerciseFinished = newFinished;
}

std::string ExerciseTableRow::getComment() const {
	return exerciseComment;
}

void ExerciseTableRow::setComment(std::string &newComment) {
	this->exerciseComment = newComment;
}

bool ExerciseTableRow::getStateChanged() const {
	return stateChanged;
}

void ExerciseTableRow::setStateChanged(bool state) {
	stateChanged = state;
}

std::string ExerciseTableRow::getUntil() const {
	return toDoUntil;
}

/// Opens the lesson specific folder.
void ExerciseTableRow::openFolderButtonClicked() {
	BasicFileOps fileOps;
	ConfigFileParser configParser;
	std::string exerciseFolderPath = configParser.getSaveDirectoryPath() + "/" + lessonName + "/" + toDoUntil;
	fileOps.openFileManager(exerciseFolderPath);
}

/// Opens the PDF-File if there is one selected before. If not, it shows a file chooser dialog.
void ExerciseTableRow::openExercisePDFButtonClicked() {
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

void ExerciseTableRow::changeState() {
	stateChanged = true;
}

void ExerciseTableRow::totalPointsChanged() {
	if(totalPointsSpin->get_value() < reachedPointsSpin->get_value()) {
		reachedPointsSpin->set_value(totalPointsSpin->get_value());
	}
	changeState();
}

void ExerciseTableRow::reachedPointsChanged() {
	if(reachedPointsSpin->get_value() > totalPointsSpin->get_value()) {
		totalPointsSpin->set_value(reachedPointsSpin->get_value());
	}
	changeState();
}

ExerciseTableRow::~ExerciseTableRow() { }

