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
#include "../fileOperations/ConfigFileParser.h"
#include "../fileOperations/BasicFileOps.h"
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
	initializeNewExerciseBox();

	initializeExerciseTable();

	tableScroller->add(*exerciseTable);
	tableScroller->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	mainBox->pack_start(*tableScroller, Gtk::PACK_EXPAND_WIDGET, 0);
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
	nothingAddedYetLabel = nullptr;
	// nothing added yet label is initialized in initializeExerciseTable because it is deleted
	// if a first row was added. If the last row was deleted we have to reinitialize it
	newExerciseFrame = Gtk::manage(new Gtk::Frame);
	newExerciseBox = Gtk::manage(new Gtk::HBox);
	tableScroller = Gtk::manage(new Gtk::ScrolledWindow);
	saveChangingsButton = Gtk::manage(new Gtk::Button);
	resetButton = Gtk::manage(new Gtk::Button);
}

/// Initialize all widgets of the menuebar and add them to the main-frame
void LessonPage::initializeTableMenueBar() {
	Gtk::HBox *centerBox = Gtk::manage(new Gtk::HBox);
	Gtk::Image *saveImage = Gtk::manage(new Gtk::Image(SAVE_ICO));
	Gtk::Image *resetImage = Gtk::manage(new Gtk::Image(RESET_CHANGES_ICO));

	saveChangingsButton->set_image(*saveImage);
	saveChangingsButton->set_tooltip_text(LessonPageLabels::SAVE_CHANGINGS_BUTTON);
	saveChangingsButton->set_relief(Gtk::RELIEF_NONE);

	resetButton->set_image(*resetImage);
	resetButton->set_tooltip_text(LessonPageLabels::RESET_BUTTON);
	resetButton->set_relief(Gtk::RELIEF_NONE);
	resetButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonPage::resetRowsClicked));

	tableOptionsBox->pack_start(*resetButton, Gtk::PACK_SHRINK, 10);
	tableOptionsBox->pack_start(*saveChangingsButton, Gtk::PACK_SHRINK, 10);

	centerBox->pack_start(*tableOptionsBox, Gtk::PACK_EXPAND_PADDING);
	centerBox->pack_end(*newExerciseFrame, Gtk::PACK_SHRINK);
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
	newExerciseBox->set_border_width(10);

	saveNewExerciseButton->set_size_request(-1, 20);
	saveNewExerciseButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonPage::saveButtonClicked));

	newExerciseFrame->set_label(NEW_EXERCISE_LABEL_TEXT);
	newExerciseFrame->add(*newExerciseBox);
	newExerciseFrame->set_border_width(5);
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

	Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *reachedPointsLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *totalPointsLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *openDirLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *openPDFLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *finishedLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *commentLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *deleteLabel = Gtk::manage(new Gtk::Label);

	untilLabel->set_markup(LessonPageLabels::UNTIL);
	reachedPointsLabel->set_markup(LessonPageLabels::REACHED_POINTS);
	totalPointsLabel->set_markup(LessonPageLabels::TOTAL_POINTS);
	openDirLabel->set_markup(LessonPageLabels::OPEN_DIR);
	openPDFLabel->set_markup(LessonPageLabels::OPEN_PDF);
	finishedLabel->set_markup(LessonPageLabels::EXERCISE_FINISHED);
	commentLabel->set_markup(LessonPageLabels::EXERCISE_COMMENT);
	deleteLabel->set_markup(LessonPageLabels::DELETE_EXERCISE);

	exerciseTable->attach(*untilLabel, 0, 1, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*reachedPointsLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*totalPointsLabel, 2, 3, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*openDirLabel, 3, 4, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*openPDFLabel, 4, 5, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*finishedLabel, 5, 6, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*commentLabel, 6, 7, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	exerciseTable->attach(*deleteLabel, 7, 8, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);

	unsigned int numOfRows = (exercises.size() > 0) ? exercises.at(0).size() : 0;

	// there is no row in the table so show the label
	if(numOfRows == 0)
		addNothingAddedYetLabelToTable();

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
		LessonTableRow *newRow = new LessonTableRow(timeOpts.getGermanDateFormat(), [this] () {
			return (allRows.size() == 0) ? 1 : allRows.back()->getID() + 1;
		}(), curLesson);
		allRows.push_back(newRow);
		addRowToTable();

		BasicFileOps fileOps;
		try {
			fileOps.createFolder(fileOps.callConfigParser().getSaveDirectoryPath() + "/" + curLesson + "/" + timeOpts.getGermanDateFormat());
		} catch (FILE_ERRORS &error) {
			HelpDialogs::showErrorDialog(error);
		}

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
	deleteButton->signal_clicked().connect(sigc::bind<LessonTableRow&, Gtk::Button*>(
				sigc::mem_fun(*this, &LessonPage::deleteButtonClicked), *(allRows.back()), deleteButton));
	return deleteButton;
}

/// Event triggered by a delete button
/**
 * It shows a confirm-dialog that needs to be approved before the exercise will be deleted.
 * Parameters were binded to this function by the getDeleteButton() function.
 * @param exerciseId the (sql)id of the row which should be deleted
 * @param *deleteButton pointer to the widget itself so that it can delete itself
 */
void LessonPage::deleteButtonClicked(LessonTableRow &lastRow, Gtk::Button *deleteButton) {
	switch(HelpDialogs::showMultipleDeleteDialog(LessonPageLabels::MULTIPLE_DELETE_TITLE,
												 LessonPageLabels::MULTIPLE_DELETE_MESSAGE)) {
	case int(ANSWERS::CANCEL):
		return;
		break;
	case int(ANSWERS::DELETE_ALL): {
		// this case runs through to case delete cell only (no break).
		// That's why it deletes all, inclusive the cell.
		BasicFileOps fileOps;
		try {
			fileOps.deleteFolder(fileOps.callConfigParser().getSaveDirectoryPath() + "/" + curLesson + "/" + lastRow.getUntil());
		} catch (const FILE_ERRORS &error) {
			HelpDialogs::showErrorDialog(error);
		}
	}
	case int(ANSWERS::DELETE): {
		try {
			connection.deleteExercise(this->curLesson, lastRow.getID());
		} catch(ERRORS &error) {
			HelpDialogs::showErrorDialog(error);
			return;
		}
		exerciseTable->remove(*lastRow.getCommentTextView());
		exerciseTable->remove(*lastRow.getExerciseFinishedButton());
		exerciseTable->remove(*lastRow.getReachedPointsSpin());
		exerciseTable->remove(*lastRow.getTotalPointsSpin());
		exerciseTable->remove(*lastRow.getOpenFolderButton());
		exerciseTable->remove(*lastRow.getOpenExercisePDF());
		exerciseTable->remove(*lastRow.getUntilLabel());
		exerciseTable->remove(*deleteButton);
		allRows.erase(std::find(allRows.begin(), allRows.end(), &lastRow));

		// there is no lesson left so show the label
		if(allRows.size() == 0)
			addNothingAddedYetLabelToTable();

		exerciseTable->show_all();
		break;
	}
	default:
		return;
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

	// delete the label because a first row was added right now
	if(nothingAddedYetLabel != nullptr) {
		exerciseTable->remove(*nothingAddedYetLabel);
		nothingAddedYetLabel = nullptr;
	}

	exerciseTable->attach(*allRows.back()->getUntilLabel(), 0, 1, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getReachedPointsSpin(), 1, 2, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getTotalPointsSpin(), 2, 3, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getOpenFolderButton(), 3, 4, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getOpenExercisePDF(), 4, 5, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getExerciseFinishedButton(), 5, 6, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back()->getCommentTextView(), 6, 7, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*getDeleteButton(), 7, 8, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->show_all();
}

void LessonPage::addNothingAddedYetLabelToTable() {
	nothingAddedYetLabel = Gtk::manage(new Gtk::Label);
	Pango::FontDescription fdesc;
	fdesc.set_size(15 * PANGO_SCALE);
	nothingAddedYetLabel->modify_font(fdesc);
	nothingAddedYetLabel->set_markup(LessonPageLabels::NO_EXERCISE_ADDED_LABEL);
	exerciseTable->attach(*nothingAddedYetLabel, 3, 4, 1, 2, Gtk::EXPAND, Gtk::FILL, 20, 40);
}

