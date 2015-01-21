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
#include <sstream>
#include <iostream>
#include <functional>

/**
 * default constructor; called to create an row-object from sql-database
 */
LessonTableRow::LessonTableRow(std::vector<std::string> row) {
	idInSqlDB = atoi(row.at(COLUMN_ID::ID).c_str());
	toDoUntil = row.at(COLUMN_ID::UNTIL);
	reachedPoints = atoi(row.at(COLUMN_ID::REACHED_POINTS).c_str());
	totalPoints = atoi(row.at(COLUMN_ID::TOTAL_POINTS).c_str());
	exerciseFinished = (row.at(COLUMN_ID::EXERCISE_FINISHED) == "1") ? true : false;
	exerciseComment = row.at(COLUMN_ID::EXERCISE_COMMENT);

	initializeWidgets();
}

/**
 * This constructor should only be called if a new exercise has been added
 * all values are set to 0, except the until-value and the id-value
 */
LessonTableRow::LessonTableRow(std::string until, int id) {
	idInSqlDB = id;
	toDoUntil = until;
	reachedPoints = 0;
	totalPoints = 0;
	exerciseFinished = false;
	exerciseComment = "";
	initializeWidgets();
}

void LessonTableRow::initializeWidgets() {
	untilLabel = Gtk::manage(new Gtk::Label(toDoUntil));
	reachedPointsEntry = Gtk::manage(new Gtk::Entry);
	totalPointsEntry = Gtk::manage(new Gtk::Entry);
	openFolderButton = Gtk::manage(new Gtk::Button);
	openFolderButtonImage = Gtk::manage(new Gtk::Image(OPENDIR_ICO));
	exerciseFinishedButton = Gtk::manage(new Gtk::CheckButton);
	commentTextView = Gtk::manage(new Gtk::TextView);

	// needed to convert given integer values (reached points, etc) to string
	std::stringstream intToString;

	reachedPointsEntry->set_size_request(50, 30);
	intToString << reachedPoints;
	reachedPointsEntry->set_text(intToString.str());

	totalPointsEntry->set_size_request(50, 30);
	intToString.str("");
	intToString << totalPoints;
	totalPointsEntry->set_text(intToString.str());

	openFolderButton->set_image(*openFolderButtonImage);
	openFolderButton->set_size_request(50, 50);
	openFolderButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);

	exerciseFinishedButton->set_active(exerciseFinished);

	Glib::RefPtr<Gtk::TextBuffer> defaultText = Gtk::TextBuffer::create();
	defaultText->set_text(exerciseComment);
	commentTextView->set_size_request(130, 50);
	commentTextView->set_border_width(1);
	commentTextView->set_justification(Gtk::JUSTIFY_FILL);
	commentTextView->set_buffer(defaultText);
}

Gtk::Label* LessonTableRow::getUntilLabel() {
	return untilLabel;
}

Gtk::Entry* LessonTableRow::getReachedPointsEntry() {
	return reachedPointsEntry;
}

Gtk::Entry* LessonTableRow::getTotalPointsEntry() {
	return totalPointsEntry;
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

LessonTableRow::~LessonTableRow() {

}

