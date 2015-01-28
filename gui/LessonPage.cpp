/*
 * LessonPage.cpp
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#include "LessonPage.h"
#include "../constants/constants.h"
#include "../constants/Labels.h"
#include "../helpers/HelpDialogs.h"
#include "../helpers/TimeConvert.h"
#include "LessonTableRow.h"
#include <iostream>
#include <gtkmm.h>
#include <string>
#include <sstream>

/*
 * TODO: shorten this class... e.g. outsource the exerciseTable
 */

/// Constructor for a new Notebook-lesson-page
/**
 * Initializes all child-widgets, the menuebar, the exercisetable and the
 * new exercise box
 * @param pageTitle the title of the new Frame (visible in top left corner)
 */
LessonPage::LessonPage(std::string pageTitle) :
		curLesson(pageTitle) {
	set_label(LessonPageLabels::FRAME_HEADER + curLesson);

	initializeWidgets();

	initializeTableMenueBar();

	initializeExerciseTable();

	initializeNewExerciseBox();

	tableScroller->add(*exerciseTable);
	tableScroller->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	mainBox->pack_start(*tableScroller, Gtk::PACK_EXPAND_WIDGET, 0);
	mainBox->pack_start(*newExerciseFrame, Gtk::PACK_SHRINK, 0);
	this->add(*mainBox);
}

/// Destructor that frees dynamically allocated pointers
LessonPage::~LessonPage() {
	for(int i = 0; i < allRows.size(); i++) {
		delete allRows.at(i);
	}
}

/// Initializes all widget-pointers with gtk garbage collector (no need to destroy them manually)
void LessonPage::initializeWidgets() {
	mainBox = Gtk::manage(new Gtk::VBox);
	tableOptionsBox = Gtk::manage(new Gtk::HBox);
	exerciseTable = Gtk::manage(new Gtk::Table);
	newExerciseFrame = Gtk::manage(new Gtk::Frame);
	newExerciseBox = Gtk::manage(new Gtk::HBox);
	tableScroller = Gtk::manage(new Gtk::ScrolledWindow);
	saveChangingsButton = Gtk::manage(new Gtk::Button(LessonPageLabels::SAVE_CHANGINGS_BUTTON));
	resetButton = Gtk::manage(new Gtk::Button(LessonPageLabels::RESET_BUTTON));
}

/// Initialize all widgets of the menuebar and add them to the main-frame
void LessonPage::initializeTableMenueBar() {
	Gtk::HBox *centerBox = Gtk::manage(new Gtk::HBox);
	resetButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonPage::resetRowsClicked));
	tableOptionsBox->pack_end(*saveChangingsButton, Gtk::PACK_SHRINK, 10);
	tableOptionsBox->pack_end(*resetButton, Gtk::PACK_SHRINK, 10);
	centerBox->pack_start(*tableOptionsBox, Gtk::PACK_EXPAND_PADDING);
	mainBox->pack_start(*centerBox, Gtk::PACK_SHRINK, 10);
}

/// Initialize new exercise box and add it to the bottom of the page
void LessonPage::initializeNewExerciseBox() {
	exerciseUntilLabel = Gtk::manage(new Gtk::Label(EXERCISE_UNTIL_LABEL_TEXT));
	exerciseUntilDaySpin = Gtk::manage(new Gtk::SpinButton);
	exerciseUntilMonthSpin = Gtk::manage(new Gtk::SpinButton);
	exerciseUntilYearSpin = Gtk::manage(new Gtk::SpinButton);
	saveNewExerciseButton = Gtk::manage(new Gtk::Button(SAVE_BUTTON_TEXT));

	TimeConvert timeOps;

	exerciseUntilDaySpin->set_max_length(2);
	exerciseUntilDaySpin->set_editable(false);
	exerciseUntilDaySpin->set_range(1, timeOps.getDaysInMonth(timeOps.getCurMonth(), timeOps.getCurYear()));
	exerciseUntilDaySpin->set_increments(1, 1);
	exerciseUntilDaySpin->set_wrap(true);
	exerciseUntilDaySpin->set_value(timeOps.getCurDay());

	exerciseUntilMonthSpin->set_max_length(2);
	exerciseUntilMonthSpin->set_editable(false);
	exerciseUntilMonthSpin->set_range(1, 12);
	exerciseUntilMonthSpin->set_increments(1, 1);
	exerciseUntilMonthSpin->set_wrap(true);
	exerciseUntilMonthSpin->set_value(timeOps.getCurMonth());
	exerciseUntilMonthSpin->signal_changed().connect(sigc::mem_fun(*this, &LessonPage::newExerciseDateChanged));

	exerciseUntilYearSpin->set_max_length(4);
	exerciseUntilYearSpin->set_editable(false);
	exerciseUntilYearSpin->set_range(timeOps.getCurYear(), timeOps.getCurYear() + 100);
	exerciseUntilYearSpin->set_increments(1, 1);
	exerciseUntilYearSpin->set_value(timeOps.getCurYear());
	exerciseUntilYearSpin->signal_changed().connect(sigc::mem_fun(*this, &LessonPage::newExerciseDateChanged));

	exerciseUntilLabel->set_padding(10, 10);

	newExerciseBox->pack_start(*exerciseUntilLabel, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*exerciseUntilDaySpin, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*exerciseUntilMonthSpin, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*exerciseUntilYearSpin, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*saveNewExerciseButton, Gtk::PACK_START, 0);
	saveNewExerciseButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonPage::saveButtonClicked));

	newExerciseFrame->set_label(NEW_EXERCISE_LABEL_TEXT);
	newExerciseFrame->add(*newExerciseBox);
}

/// Initializes the exercise table with exercise-rows.
/**
 * If an error occurs, it shows an error-dialog.
 */
void LessonPage::initializeExerciseTable() {
	try {
		exercises = connection.getExercises(curLesson);
	}catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}


	int numOfRows = (exercises.size() > 0) ? exercises.at(0).size() : 0;
	// just add the table if at least 1 exercise exists
	if(numOfRows != 0) {
		Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *reachedPointsLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *totalPointsLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *openDirLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *finishedLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *commentLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *deleteLabel = Gtk::manage(new Gtk::Label);

		untilLabel->set_markup(LessonPageLabels::UNTIL);
		reachedPointsLabel->set_markup(LessonPageLabels::REACHED_POINTS);
		totalPointsLabel->set_markup(LessonPageLabels::TOTAL_POINTS);
		openDirLabel->set_markup(LessonPageLabels::OPEN_DIR);
		finishedLabel->set_markup(LessonPageLabels::EXERCISE_FINISHED);
		commentLabel->set_markup(LessonPageLabels::EXERCISE_COMMENT);
		deleteLabel->set_markup(LessonPageLabels::DELETE_EXERCISE);

		exerciseTable->attach(*untilLabel, 0, 1, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*reachedPointsLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*totalPointsLabel, 2, 3, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*openDirLabel, 3, 4, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*finishedLabel, 4, 5, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*commentLabel, 5, 6, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*deleteLabel, 6, 7, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);


		/**
		 * TODO: check, if a class of type Gtk::Label with static label-counter isnt better for
		 * this loop (or at all).
		 */
		for(int row = 0; row < numOfRows; row++) {
			// because rows are columns and other way round, we need to do it like this
			// transform the string-matrix
			std::vector<std::string> curRow;
			for(int column = 0; column < exercises.size(); column++) {
				curRow.push_back(exercises.at(column).at(row));
			}
			LessonTableRow *newLessonRow = new LessonTableRow(curRow);
			allRows.push_back(newLessonRow);
			addRowToTable();
		}
	}
	exerciseTable->show_all();
}

/// Add a new exercise to the SQL-DB and the exercise table.
/**
 * Triggered when the saveNewExerciseButton is clicked.
 * If an error occurs, it shows an error-dialog.
 */
void LessonPage::saveButtonClicked() {
	try {
		TimeConvert timeOpts(exerciseUntilDaySpin->get_value(),
				exerciseUntilMonthSpin->get_value(),
				exerciseUntilYearSpin->get_value());
		connection.addNewExercise(curLesson, FOLDER_PATH + this->curLesson + "/", timeOpts.getEnglishDateFormat());
		LessonTableRow *newRow = new LessonTableRow(timeOpts.getGermanDateFormat(), allRows.back()->getID() + 1);
		allRows.push_back(newRow);
		addRowToTable();
	} catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
}

/// Allocates a new delete-button which can be added to each row of the exercise table.
/**
 * The returned button is connected to the deleteButtonClicked function, binded to the id of its row
 * and to a pointer to the widget itself, just for deleting it after event was triggered.
 * @returns a delete-button with delete icon.
 */
Gtk::Button* LessonPage::getDeleteButton() {
	Gtk::Image *deleteIcon = Gtk::manage(new Gtk::Image(DELETE_ICO));
	Gtk::Button *deleteButton = Gtk::manage(new Gtk::Button);
	deleteButton->set_image(*deleteIcon);
	deleteButton->set_size_request(50, 50);
	deleteButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);
	deleteButton->signal_clicked().connect(sigc::bind<int, Gtk::Button*>(
				sigc::mem_fun(*this, &LessonPage::deleteButtonClicked), allRows.back()->getID(), deleteButton));
	return deleteButton;
}

/// Event triggered by a delete button
/**
 * It shows a confirm-dialog that needs to be approved before the exercise will be deleted.
 * Parameters were binded to this function by the getDeleteButton() function.
 * @param exerciseId the (sql)id of the row which should be deleted
 * @param *deleteButton pointer to the widget itself so that it can delete itself
 */
void LessonPage::deleteButtonClicked(int exerciseId, Gtk::Button *deleteButton) {
	if(HelpDialogs::showConfirmDialog(HelpDialogs::CONFIRM_DELETION,
			HelpDialogs::CONFIRM_DELETION_SUBTEXT) != HelpDialogs::CONFIRMED)
		return;
	for(int i = 0; i < allRows.size(); i++) {
		if(allRows.at(i)->getID() == exerciseId) {
			try {
				connection.deleteExercise(this->curLesson, allRows.at(i)->getID());
			} catch(ERRORS &error) {
				HelpDialogs::showErrorDialog(error);
				return;
			}
			exerciseTable->remove(*allRows.at(i)->getCommentTextView());
			exerciseTable->remove(*allRows.at(i)->getExerciseFinishedButton());
			exerciseTable->remove(*allRows.at(i)->getReachedPointsSpin());
			exerciseTable->remove(*allRows.at(i)->getTotalPointsSpin());
			exerciseTable->remove(*allRows.at(i)->getOpenFolderButton());
			exerciseTable->remove(*allRows.at(i)->getUntilLabel());
			exerciseTable->remove(*deleteButton);
			allRows.erase(allRows.begin() + i);
			exerciseTable->show_all();
			return;
		}
	}
}

/// Event triggered by the resetButton
/**
 * It resets all changed rows with their native (old) values
 */
void LessonPage::resetRowsClicked() {
	for(LessonTableRow *curRow : allRows) {
		if(curRow->getStateChanged()) {
			std::cout << curRow->getID() << std::endl;
			curRow->getReachedPointsSpin()->set_value(curRow->getReachedPoints());
			curRow->getTotalPointsSpin()->set_value(curRow->getTotalPoints());
			curRow->getExerciseFinishedButton()->set_active(curRow->getIsExerciseFinished());
			Glib::RefPtr<Gtk::TextBuffer> oldText = Gtk::TextBuffer::create();
			oldText->set_text(curRow->getComment());
			curRow->getCommentTextView()->set_buffer(oldText);
			curRow->setStateChanged(false);
		}
	}
}

/// Event triggered by changing the newExerciseDateSpins
/**
 * If the selected year respectively month has more or less days than the current one
 * the daySpinBox is updated to an appropriate value.
 */
void LessonPage::newExerciseDateChanged() {
	TimeConvert timeOpts;
	unsigned int newMonth, newYear;
	std::istringstream(exerciseUntilMonthSpin->get_text()) >> newMonth;
	std::istringstream(exerciseUntilYearSpin->get_text()) >> newYear;
	exerciseUntilDaySpin->set_range(1, timeOpts.getDaysInMonth(newMonth, newYear));
}

/// Adds a new row to the end of the exercise table
/**
 * This function takes the last element out of the allRows vector and
 * adds it at the end of the exercise table.
 */
void LessonPage::addRowToTable() {
	unsigned int rows, cols;
	exerciseTable->get_size(rows, cols);

	exerciseTable->attach(*allRows.back()->getUntilLabel(), 0, 1, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getReachedPointsSpin(), 1, 2, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getTotalPointsSpin(), 2, 3, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getOpenFolderButton(), 3, 4, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getExerciseFinishedButton(), 4, 5, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getCommentTextView(), 5, 6, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*getDeleteButton(), 6, 7, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->show_all();
}


