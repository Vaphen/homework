/*
 * LessonTableRow.cpp
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#include "LessonTableRow.h"
#include "LessonPage.h"
#include "../constants/constants.h"
#include <string>
#include <gtkmm.h>
#include <iostream>
#include <thread>

/**
 * TODO: chageState doesnt work!!
 */

/**
 * default constructor; called to create an row-object from sql-database
 */
LessonTableRow::LessonTableRow(std::vector<std::string> row) :
		stateChanged(false) {
	idInSqlDB = atoi(row.at(COLUMN_ID::ID).c_str());
	toDoUntil = row.at(COLUMN_ID::UNTIL);
	reachedPoints = atoi(row.at(COLUMN_ID::REACHED_POINTS).c_str());
	totalPoints = atoi(row.at(COLUMN_ID::TOTAL_POINTS).c_str());
	folderPath = row.at(COLUMN_ID::DIR_PATH);
	isExerciseFinished = (row.at(COLUMN_ID::EXERCISE_FINISHED) == "1") ? true : false;
	exerciseComment = row.at(COLUMN_ID::EXERCISE_COMMENT);

	initializeWidgets();
}

/**
 * This constructor should only be called if a new exercise has been added
 * all values are set to 0, except the until-value and the id-value
 */
LessonTableRow::LessonTableRow(std::string until, int id) :
			stateChanged(false) {
	/**
	 * TODO: add folderPath to this function and set it(as parameter etc.)
	 */
	idInSqlDB = id;
	toDoUntil = until;
	reachedPoints = 0;
	totalPoints = 0;
	folderPath = "";
	isExerciseFinished = false;
	exerciseComment = "";
	initializeWidgets();
}

void LessonTableRow::initializeWidgets() {
	untilLabel = Gtk::manage(new Gtk::Label(toDoUntil));
	reachedPointsSpin = Gtk::manage(new Gtk::SpinButton);
	totalPointsSpin = Gtk::manage(new Gtk::SpinButton);
	openFolderButton = Gtk::manage(new Gtk::Button);
	openFolderButtonImage = Gtk::manage(new Gtk::Image(OPENDIR_ICO));
	exerciseFinishedButton = Gtk::manage(new Gtk::CheckButton);
	commentTextView = Gtk::manage(new Gtk::TextView);

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
	openFolderButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);
	openFolderButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonTableRow::openFolderButtonClicked));

	exerciseFinishedButton->set_active(isExerciseFinished);
	exerciseFinishedButton->signal_toggled().connect(sigc::mem_fun(*this, &LessonTableRow::changeState));

	Glib::RefPtr<Gtk::TextBuffer> defaultText = Gtk::TextBuffer::create();
	defaultText->set_text(exerciseComment);
	commentTextView->set_size_request(130, 50);
	commentTextView->set_border_width(1);
	commentTextView->set_justification(Gtk::JUSTIFY_FILL);
	commentTextView->set_buffer(defaultText);
	commentTextView->signal_grab_focus().connect(sigc::mem_fun(*this, &LessonTableRow::changeState));
}

Gtk::Label* LessonTableRow::getUntilLabel() {
	return untilLabel;
}

Gtk::SpinButton* LessonTableRow::getReachedPointsSpin() {
	return reachedPointsSpin;
}

Gtk::SpinButton* LessonTableRow::getTotalPointsSpin() {
	return totalPointsSpin;
}

Gtk::Button* LessonTableRow::getOpenFolderButton() {
	return openFolderButton;
}

Gtk::CheckButton* LessonTableRow::getExerciseFinishedButton() {
	return exerciseFinishedButton;
}

Gtk::TextView* LessonTableRow::getCommentTextView() {
	return commentTextView;
}

int LessonTableRow::getID() {
	return idInSqlDB;
}

unsigned int LessonTableRow::getReachedPoints() const {
	return reachedPoints;
}

unsigned int LessonTableRow::getTotalPoints() const {
	return totalPoints;
}

bool LessonTableRow::getIsExerciseFinished() const {
	return isExerciseFinished;
}

std::string LessonTableRow::getComment() const {
	return exerciseComment;
}

bool LessonTableRow::getStateChanged() const {
	return stateChanged;
}

void LessonTableRow::setStateChanged(bool state) {
	stateChanged = state;
}

void LessonTableRow::openFolderButtonClicked() {
	std::thread ownThread([](){
		// just for test-issues. must be changed
		system(std::string(std::string(FILEMANAGER) + " " + std::string(FOLDER_PATH) + "PSE/").c_str());
	});
	ownThread.detach();
}

void LessonTableRow::changeState() {
	stateChanged = true;
}

LessonTableRow::~LessonTableRow() { }

