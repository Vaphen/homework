/*
 * MarkTableRow.cpp
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#include "ExamTableRow.h"
#include "../../constants/constants.h"
#include "../../constants/Labels.h"
#include "../../fileOperations/BasicFileOps.h"
#include "../../fileOperations/ConfigFileParser.h"
#include "../../helpers/HelpDialogs.h"
#include "../../helpers/TimeConvert.h"
#include <string>
#include <gtkmm.h>
#include <iostream>

/// Constructor for new row (from SQL-Query)
/**
 * Should be used rather than the other constructor.
 * @param row a vector containing all infos about the row that should be added
 */
ExamTableRow::ExamTableRow(std::vector<std::string> row) :
		stateChanged(false) {
	idInSqlDB = atoi(row.at(COLUMN_ID_MARK::ID).c_str());
	TimeConvert timeConverter;
	toDoUntilTimestamp = row.at(COLUMN_ID_MARK::UNTIL);
	lessonName = row.at(COLUMN_ID_MARK::LESSON).c_str();
	semester = atoi(row.at(COLUMN_ID_MARK::SEMESTER).c_str());
	isAdmited = (row.at(COLUMN_ID_MARK::IS_ADMITED) == "1") ? true : false;
	hasPassed = (row.at(COLUMN_ID_MARK::HAS_PASSED) == "1") ? true : false;
	mark = std::atof(row.at(COLUMN_ID_MARK::MARK).c_str());
	initializeWidgets();
}
/// Constructor for new, not yet in database existing exam
/**
 * This constructor should only be called if a new exercise has been added.
 * All values are set to a default value, except the until-value, the id and the lessonName-value
 * @param until string of the date when the exam must be written
 * @param id The id of the test in the database
 * @param lesson name of the exam-lesson
 */
ExamTableRow::ExamTableRow(const unsigned int& semester, int id, const std::string &lesson) :
			stateChanged(false) {
	idInSqlDB = id;
	toDoUntilTimestamp = timeOpts.getUnixTimeFormat(timeOpts.getCurDay(),
			timeOpts.getCurMonth(), timeOpts.getCurYear());
	this->semester = semester;
	isAdmited = false;
	hasPassed = false;
	mark = 0.0f;
	lessonName = lesson;
	initializeWidgets();
}

/// Initializes all widgets for one row setting properties too.
void ExamTableRow::initializeWidgets() {
	daySpin = Gtk::manage(new Gtk::SpinButton);
	monthSpin = Gtk::manage(new Gtk::SpinButton);
	yearSpin = Gtk::manage(new Gtk::SpinButton);
	untilBox = Gtk::manage(new Gtk::HBox);
	semesterSpin = Gtk::manage(new Gtk::SpinButton);
	isAdmitedButton = Gtk::manage(new Gtk::CheckButton);
	hasPassedButton = Gtk::manage(new Gtk::CheckButton);
	markSpin = Gtk::manage(new Gtk::SpinButton);
	lessonNameLabel = Gtk::manage(new Gtk::Label(lessonName));


	daySpin->set_max_length(2);
	daySpin->set_editable(false);
	daySpin->set_range(1, timeOpts.getDaysInMonth(timeOpts.getCurMonth(), timeOpts.getCurYear()));
	daySpin->set_increments(1, 1);
	daySpin->set_value(timeOpts.getDayOfUnixTimestamp(toDoUntilTimestamp));
	daySpin->set_size_request(40, 25);
	daySpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));

	monthSpin->set_max_length(2);
	monthSpin->set_editable(false);
	monthSpin->set_range(1, 12);
	monthSpin->set_increments(1, 1);
	monthSpin->set_value(timeOpts.getMonthOfUnixTimestamp(toDoUntilTimestamp));
	monthSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::examDateChanged));
	monthSpin->set_size_request(40, 25);
	monthSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));

	yearSpin->set_max_length(4);
	yearSpin->set_editable(false);
	yearSpin->set_range(timeOpts.getCurYear(), timeOpts.getCurYear() + 100);
	yearSpin->set_increments(1, 1);
	yearSpin->set_value(timeOpts.getYearOfUnixTimestamp(toDoUntilTimestamp));
	yearSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::examDateChanged));
	yearSpin->set_size_request(80, 25);
	yearSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));



	Gtk::Label *firstPoint = Gtk::manage(new Gtk::Label("."));
	Gtk::Label *secondPoint = Gtk::manage(new Gtk::Label("."));
	untilBox->pack_start(*daySpin, Gtk::SHRINK, 0);
	untilBox->pack_start(*firstPoint, Gtk::SHRINK, 0);
	untilBox->pack_start(*monthSpin, Gtk::SHRINK, 0);
	untilBox->pack_start(*secondPoint, Gtk::SHRINK, 0);
	untilBox->pack_start(*yearSpin, Gtk::SHRINK, 0);

	semesterSpin->set_size_request(80, 33);
	semesterSpin->set_max_length(2);
	semesterSpin->set_increments(1, 1);
	semesterSpin->set_range(1, 15);
	semesterSpin->set_editable(false);
	semesterSpin->set_value(semester);
	semesterSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));

	isAdmitedButton->set_active(isAdmited);
	isAdmitedButton->signal_clicked().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));
	hasPassedButton->set_active(hasPassed);
	hasPassedButton->signal_clicked().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));

	markSpin->set_digits(1);
	markSpin->set_increments(0.1, 0.1);
	markSpin->set_range(0.9, 6.0);
	markSpin->set_size_request(80, 33);
	markSpin->set_editable(false);
	if(mark < 1.0f) {
		markSpin->set_text("---");
	}else {
		markSpin->set_value(mark);
	}
	markSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::changeState));
	markSpin->signal_changed().connect(sigc::mem_fun(*this, &ExamTableRow::changedMarkValue));
}

/*
 * Getter
 */

Gtk::SpinButton* ExamTableRow::getDaySpin() const {
	return daySpin;
}

Gtk::SpinButton* ExamTableRow::getMonthSpin() const {
	return monthSpin;
}

Gtk::SpinButton* ExamTableRow::getYearSpin() const {
	return yearSpin;
}

Gtk::HBox* ExamTableRow::getUntilBox() const {
	return untilBox;
}

Gtk::SpinButton* ExamTableRow::getSemesterSpin() const {
	return semesterSpin;
}

Gtk::CheckButton* ExamTableRow::getIsAdmitedButton() const {
	return isAdmitedButton;
}

Gtk::CheckButton* ExamTableRow::getHasPassedButton() const {
	return hasPassedButton;
}

Gtk::SpinButton* ExamTableRow::getMarkSpin() const {
	return markSpin;
}

Gtk::Label* ExamTableRow::getLessonNameLabel() const {
	return lessonNameLabel;
}

std::string ExamTableRow::getUntilTimestamp() const {
	return toDoUntilTimestamp;
}

std::string ExamTableRow::getLessonName() const {
	return lessonName;
}

unsigned short ExamTableRow::getSemester() const {
	return semester;
}

bool ExamTableRow::getIsAdmited() const {
	return isAdmited;
}

bool ExamTableRow::getHasPassed() const {
	return hasPassed;
}

float ExamTableRow::getMark() const {
	return mark;
}

unsigned int ExamTableRow::getID() const {
	return idInSqlDB;
}

bool ExamTableRow::getStateChanged() const {
	return stateChanged;
}

/*
 * Setter
 */

void ExamTableRow::setUntilTimestamp(std::string newUntil) {
	this->toDoUntilTimestamp = newUntil;
}

void ExamTableRow::setSemester(unsigned short newSemester) {
	this->semester = newSemester;
}

void ExamTableRow::setIsAdmitted(bool newIsAdmitted) {
	this->isAdmited = newIsAdmitted;
}

void ExamTableRow::setHasPassed(bool newHasPassed) {
	this->hasPassed = newHasPassed;
}

void ExamTableRow::setMark(float newMark) {
	this->mark = newMark;
}

void ExamTableRow::setStateChanged(bool newState) {
	stateChanged = newState;
}

void ExamTableRow::changeState() {
	stateChanged = true;
}

void ExamTableRow::changedMarkValue() {
	if(markSpin->get_value() == 0.9)
		markSpin->set_text("---");
}

/// Event triggered by changing the examDateSpins
/**
 * If the selected year respectively month has more or less days than the current one
 * the daySpinBox is updated to an appropriate value.
 */
void ExamTableRow::examDateChanged() {
	TimeConvert timeOpts;

	unsigned int newMonth = monthSpin->get_value_as_int();
	unsigned int newYear = yearSpin->get_value_as_int();

	if(monthSpin->get_value_as_int() > timeOpts.getCurMonth()) {
		// the year is the current, but the month isnt. change day spin range
		daySpin->set_range(1, timeOpts.getDaysInMonth(newMonth, newYear));
	} else if(yearSpin->get_value_as_int() > timeOpts.getCurYear()) {
		// the year and the month are different from the status quo. set range to appropriate values.
		daySpin->set_range(1, timeOpts.getDaysInMonth(newMonth, newYear));
		monthSpin->set_range(1, 12);
	} else {
		// its current month and year; so set values to current ones
		daySpin->set_range(timeOpts.getCurDay(), timeOpts.getDaysInMonth(timeOpts.getCurMonth(), timeOpts.getCurYear()));
		monthSpin->set_range(timeOpts.getCurMonth(), 12);
	}
}


ExamTableRow::~ExamTableRow() { }

