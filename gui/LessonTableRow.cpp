/*
 * LessonTableRow.cpp
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#include "LessonTableRow.h"
#include "../constants/constants.h"
#include <string>
#include <gtkmm.h>
#include <sstream>
#include <iostream>

/**
 * TODO: rename column, its row
 */
LessonTableRow::LessonTableRow(std::vector<std::string> column) {
	for(std::string col : column)
		std::cout << col << "-";
	std::cout << std::endl;
	/*idInSqlDB = atoi(column.at(COLUMN_ID::ID).c_str());
	toDoUntil = column.at(COLUMN_ID::UNTIL);
	reachedPoints = atoi(column.at(COLUMN_ID::REACHED_POINTS).c_str());
	totalPoints = atoi(column.at(COLUMN_ID::TOTAL_POINTS).c_str());
	exerciseFinished = (column.at(COLUMN_ID::EXERCISE_FINISHED) == "1") ? true : false;
	exerciseComment = column.at(COLUMN_ID::EXERCISE_COMMENT);*/

	initializeWidgets();
}

void LessonTableRow::initializeWidgets() {
	untilLabel = Gtk::manage(new Gtk::Label(toDoUntil));
	reachedPointsEntry = Gtk::manage(new Gtk::Entry);
	totalPointsEntry = Gtk::manage(new Gtk::Entry);
	exerciseFinishedButton = Gtk::manage(new Gtk::CheckButton);
	commentTextView = Gtk::manage(new Gtk::TextView);

	// needed to convert given integer values (reached points, etc) to string
	std::stringstream intToString;

	reachedPointsEntry->set_size_request(50, 30);
	intToString << reachedPoints;
	reachedPointsEntry->set_text(intToString.str());

	totalPointsEntry->set_size_request(50, 30);
	intToString << totalPoints;
	totalPointsEntry->set_text(intToString.str());

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

Gtk::CheckButton* LessonTableRow::getExerciseFinishedButton() {
	return exerciseFinishedButton;
}

Gtk::TextView* LessonTableRow::getCommentTextView() {
	return commentTextView;
}

LessonTableRow::~LessonTableRow() {

}

