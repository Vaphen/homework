/*
 * MarkPage.cpp
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#include "ExamPage.h"
#include "../../constants/constants.h"
#include "../../constants/Labels.h"
#include "../../helpers/HelpDialogs.h"
#include "../../helpers/TimeConvert.h"
#include "../../fileOperations/ConfigFileParser.h"
#include "../../fileOperations/BasicFileOps.h"
#include "ExamTableRow.h"
#include <iostream>
#include <gtkmm.h>
#include <string>

#if defined(_WIN32) || defined(WIN32)
	#define nullptr 0x00
#endif

/*
 * TODO: shorten this class... e.g. outsource the markTable
 */

/// Constructor for a new Notebook-lesson-page
/**
 * Initializes all child-widgets, the menuebar, the markTable and the
 * new exercise box
 * @param pageTitle the title of the new Frame (visible in top left corner)
 * @param *parentNotebook a pointer to the notebook the side was added to
 */
ExamPage::ExamPage() {
	set_label(MarkPageLabels::FRAME_HEADER);

	initializeWidgets();

	initializeTableMenueBar();
	initializeNewExerciseBox();

	initializeMarkTable();

	Gtk::EventBox *backgroundBox = Gtk::manage(new Gtk::EventBox);
	backgroundBox->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	backgroundBox->add(*markTable);

	tableScroller->add(*backgroundBox);
	tableScroller->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

	mainBox->pack_start(*tableScroller, Gtk::PACK_EXPAND_WIDGET, 0);

	this->add(*mainBox);
}

/// Destructor that frees dynamically allocated pointers
ExamPage::~ExamPage() {
	for(unsigned int i = 0; i < allRows.size(); i++) {
		delete allRows.at(i);
	}
}

/// Initializes all widget-pointers with gtk garbage collector (no need to destroy them manually)
void ExamPage::initializeWidgets() {
	mainBox = Gtk::manage(new Gtk::VBox);
	tableOptionsBox = Gtk::manage(new Gtk::HBox);
	markTable = Gtk::manage(new Gtk::Table);
	nothingAddedYetLabel = nullptr;
	// nothing added yet label is initialized in initializemarkTable because it is deleted
	// if a first row was added. If the last row was deleted we have to reinitialize it
	newExerciseFrame = Gtk::manage(new Gtk::Frame);
	newExerciseBox = Gtk::manage(new Gtk::HBox);
	tableScroller = Gtk::manage(new Gtk::ScrolledWindow);
	saveChangingsButton = Gtk::manage(new Gtk::Button);
	resetButton = Gtk::manage(new Gtk::Button);

	/*exerciseUntilDaySpin = Gtk::manage(new Gtk::SpinButton);
	exerciseUntilMonthSpin = Gtk::manage(new Gtk::SpinButton);
	exerciseUntilYearSpin = Gtk::manage(new Gtk::SpinButton)

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
	exerciseUntilMonthSpin->signal_changed().connect(sigc::mem_fun(*this, &MarkPage::newExerciseDateChanged));

	exerciseUntilYearSpin->set_max_length(4);
	exerciseUntilYearSpin->set_editable(false);
	exerciseUntilYearSpin->set_range(timeOps.getCurYear(), timeOps.getCurYear() + 100);
	exerciseUntilYearSpin->set_increments(1, 1);
	exerciseUntilYearSpin->set_value(timeOps.getCurYear());
	exerciseUntilYearSpin->signal_changed().connect(sigc::mem_fun(*this, &MarkPage::newExerciseDateChanged));
*/
}

/// Initialize all widgets of the menuebar and add them to the main-frame
void ExamPage::initializeTableMenueBar() {
	Gtk::HBox *centerBox = Gtk::manage(new Gtk::HBox);
	Gtk::Image *saveImage = Gtk::manage(new Gtk::Image(SAVE_ICO));
	Gtk::Image *resetImage = Gtk::manage(new Gtk::Image(RESET_CHANGES_ICO));

	saveChangingsButton->set_image(*saveImage);
	saveChangingsButton->set_tooltip_text(MarkPageLabels::SAVE_CHANGINGS_BUTTON_TOOLTIP);
	saveChangingsButton->set_relief(Gtk::RELIEF_NONE);
	saveChangingsButton->signal_clicked().connect(sigc::mem_fun(*this, &ExamPage::saveChangingsButtonClicked));


	resetButton->set_image(*resetImage);
	resetButton->set_tooltip_text(MarkPageLabels::RESET_BUTTON_TOOLTIP);
	resetButton->set_relief(Gtk::RELIEF_NONE);
	resetButton->signal_clicked().connect(sigc::mem_fun(*this, &ExamPage::resetRowsClicked));

	tableOptionsBox->pack_start(*resetButton, Gtk::PACK_SHRINK, 10);
	tableOptionsBox->pack_start(*saveChangingsButton, Gtk::PACK_SHRINK, 10);

	centerBox->pack_start(*tableOptionsBox, Gtk::PACK_EXPAND_PADDING);
	centerBox->pack_end(*newExerciseFrame, Gtk::PACK_SHRINK);
	mainBox->pack_start(*centerBox, Gtk::PACK_SHRINK, 10);
}

/// Initialize new exercise box and add it to the bottom of the page
void ExamPage::initializeNewExerciseBox() {
	Gtk::Label *examSemesterLabel = Gtk::manage(new Gtk::Label(MarkPageLabels::EXAM_SEMESTER_LABEL));
	Gtk::SpinButton *semesterSpin = Gtk::manage(new Gtk::SpinButton);
	Gtk::Label *examLessonLabel = Gtk::manage(new Gtk::Label(MarkPageLabels::EXAM_LESSON_LABEL));
	Gtk::ComboBox *lessonCombo = Gtk::manage(new Gtk::ComboBox);
	Gtk::Button *saveNewExamButton = Gtk::manage(new Gtk::Button(MarkPageLabels::SAVE_NEW_EXAM));

	examSemesterLabel->set_padding(10, 10);

	semesterSpin->set_range(1, 15);
	semesterSpin->set_increments(1, 1);
	semesterSpin->set_editable(false);
	semesterSpin->set_size_request(50, 25);

	examLessonLabel->set_padding(10, 10);

	Gtk::TreeModel::ColumnRecord allHeader;
	Glib::RefPtr<Gtk::ListStore> allLessons;
	allHeader.add(lessonHeader);
	lessonCombo->set_model(allLessons = Gtk::ListStore::create(allHeader));
	std::vector<std::string> allLessonsVec  = {};
	try {
		allLessonsVec = connection.getLessons();
	}catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
	for(unsigned int i = 0; i < allLessonsVec.size(); i++) {
		Gtk::TreeModel::Row newRow = *(allLessons->append());
		newRow[lessonHeader] = allLessonsVec.at(i);
	}

	// in case that there is no lesson
	if(allLessonsVec.size() == 0) {
		Gtk::TreeModel::Row newRow = *(allLessons->append());
		newRow[lessonHeader] = "---";
	}

	lessonCombo->pack_start(lessonHeader);
	lessonCombo->set_active(0);

	lessonCombo->set_size_request(120, 33);

	newExerciseBox->pack_start(*examSemesterLabel, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*semesterSpin, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*examLessonLabel, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*lessonCombo, Gtk::PACK_START, 0);
	newExerciseBox->pack_start(*saveNewExamButton, Gtk::PACK_START, 0);
	newExerciseBox->set_border_width(10);

	saveNewExamButton->set_size_request(-1, 20);
	saveNewExamButton->signal_clicked().connect(sigc::bind<Gtk::SpinButton*, Gtk::ComboBox*>(sigc::mem_fun(*this, &ExamPage::saveNewExamButtonClicked)
			, semesterSpin, lessonCombo));

	newExerciseFrame->set_label(MarkPageLabels::NEW_EXAM_LABEL);
	newExerciseFrame->add(*newExerciseBox);
	newExerciseFrame->set_border_width(5);
}

/// Initializes the exercise table with exercise-rows.
/**
 * If an error occurs, it shows an error-dialog.
 */
void ExamPage::initializeMarkTable() {
	try {
		exams = connection.getMarks();
	}catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}

	Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *lessonLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *semesterLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *isAdmittedLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *hasPassedLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *markLabel = Gtk::manage(new Gtk::Label);
	Gtk::Label *deleteLabel = Gtk::manage(new Gtk::Label);

	untilLabel->set_markup(MarkPageLabels::UNTIL);
	lessonLabel->set_markup(MarkPageLabels::LESSON);
	semesterLabel->set_markup(MarkPageLabels::SEMESTER);
	isAdmittedLabel->set_markup(MarkPageLabels::IS_ADMITTED);
	hasPassedLabel->set_markup(MarkPageLabels::HAS_PASSED);
	markLabel->set_markup(MarkPageLabels::MARK);
	deleteLabel->set_markup(MarkPageLabels::DELETE_EXAM);

	markTable->attach(*untilLabel, 0, 1, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	markTable->attach(*lessonLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	markTable->attach(*semesterLabel, 2, 3, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	markTable->attach(*isAdmittedLabel, 3, 4, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	markTable->attach(*hasPassedLabel, 4, 5, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	markTable->attach(*markLabel, 5, 6, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
	markTable->attach(*deleteLabel, 6, 7, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);

	unsigned int numOfRows = (exams.size() > 0) ? exams.at(0).size() : 0;

	// there is no row in the table so show the label
	if(numOfRows == 0)
		addNothingAddedYetLabelToTable();

	for(unsigned int row = 0; row < numOfRows; row++) {
		// first dimension of the vector is the column; the second dimension are the entries
		std::vector<std::string> curRow;
		for(unsigned int column = 0; column < exams.size(); column++) {
			curRow.push_back(exams.at(column).at(row));
		}

		ExamTableRow *newExamRow = new ExamTableRow(curRow);
		allRows.push_back(newExamRow);
		addRowToTable();
	}

	markTable->show_all();
}

/// Add a new exam to the SQL-DB and the exam table.
/**
 * Triggered when the saveNewExamButton is clicked.
 * If an error occurs, it shows an error-dialog.
 */
void ExamPage::saveNewExamButtonClicked(Gtk::SpinButton *semesterSpin, Gtk::ComboBox *lessonCombo) {
	try {
		Gtk::TreeModel::iterator iter = lessonCombo->get_active();
		Gtk::TreeModel::Row row = *iter;
		Glib::ustring selectedLesson = row[lessonHeader];
		connection.addNewExam(semesterSpin->get_value_as_int(), selectedLesson);
		ExamTableRow *newRow = new ExamTableRow(semesterSpin->get_value_as_int(),
				((allRows.size() == 0) ? 1 : allRows.back()->getID() + 1),
				selectedLesson);
		allRows.push_back(newRow);
		addRowToTable();
	} catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
}

/// Allocates a new delete-button which can be added to each row of the exam table.
/**
 * The returned button is connected to the deleteButtonClicked function, binded to the id of its row
 * and to a pointer to the widget itself, just for deleting it after event was triggered.
 * @returns a delete-button with delete icon.
 */
Gtk::Button* ExamPage::getDeleteButton() {
	Gtk::Image *deleteIcon = Gtk::manage(new Gtk::Image(DELETE_ICO));
	Gtk::Button *deleteButton = Gtk::manage(new Gtk::Button);
	deleteButton->set_image(*deleteIcon);
	deleteButton->set_size_request(50, 50);
	deleteButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);
	deleteButton->set_tooltip_text(MarkPageLabels::DELETE_TOOLTIP);
	deleteButton->signal_clicked().connect(sigc::bind<ExamTableRow&, Gtk::Button*>(
				sigc::mem_fun(*this, &ExamPage::deleteButtonClicked), *(allRows.back()), deleteButton));
	return deleteButton;
}

/// Event triggered by a delete button
/**
 * It shows a confirm-dialog that needs to be approved before the exam will be deleted.
 * Parameters were binded to this function by the getDeleteButton() function.
 * @param exerciseId the (sql)id of the row which should be deleted
 * @param *deleteButton pointer to the widget itself so that it can delete itself
 */
void ExamPage::deleteButtonClicked(ExamTableRow &lastRow, Gtk::Button *deleteButton) {
	// if 'yes' was clicked
	if(HelpDialogs::showConfirmDialog(MarkPageLabels::DELETE_TITLE, MarkPageLabels::DELETE_MESSAGE) == 1) {
		try {
			connection.deleteExam(lastRow.getID());
		} catch(ERRORS &error) {
			HelpDialogs::showErrorDialog(error);
			return;
		}
		markTable->remove(*lastRow.getUntilBox());
		markTable->remove(*lastRow.getLessonNameLabel());
		markTable->remove(*lastRow.getSemesterSpin());
		markTable->remove(*lastRow.getIsAdmitedButton());
		markTable->remove(*lastRow.getHasPassedButton());
		markTable->remove(*lastRow.getMarkSpin());
		markTable->remove(*deleteButton);
		allRows.erase(std::find(allRows.begin(), allRows.end(), &lastRow));

		// there is no lesson left so show the label
		if(allRows.size() == 0) {
			addNothingAddedYetLabelToTable();
		}

		markTable->show_all();
	}
}

/// Event triggered by the resetButton
/**
 * It resets all changed rows with their native (old) values
 */
void ExamPage::resetRowsClicked() {
	TimeConvert timeOpts;
	for(unsigned int i = 0; i < allRows.size(); i++) {
		ExamTableRow *curRow = allRows.at(i);
		if(curRow->getStateChanged()) {
			curRow->getDaySpin()->set_value(timeOpts.getDayOfUnixTimestamp(curRow->getUntilTimestamp()));
			curRow->getMonthSpin()->set_value(timeOpts.getMonthOfUnixTimestamp(curRow->getUntilTimestamp()));
			curRow->getYearSpin()->set_value(timeOpts.getYearOfUnixTimestamp(curRow->getUntilTimestamp()));
			curRow->getSemesterSpin()->set_value(curRow->getSemester());
			curRow->getIsAdmitedButton()->set_active(curRow->getIsAdmited());
			curRow->getHasPassedButton()->set_active(curRow->getHasPassed());
			curRow->getMarkSpin()->set_value(curRow->getMark());
			curRow->setStateChanged(false);
		}
	}
}

/// Adds a new row to the end of the exercise table
/**
 * This function takes the last element out of the allRows vector and
 * adds it at the end of the exercise table.
 */
void ExamPage::addRowToTable() {

	unsigned int rows, cols;
	markTable->get_size(rows, cols);

	// delete the label because a first row was added right now
	if(nothingAddedYetLabel != nullptr) {
		mainBox->remove(*nothingAddedYetLabel);
		nothingAddedYetLabel = nullptr;
	}

	markTable->attach(*allRows.back()->getUntilBox(), 0, 1, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->attach(*allRows.back()->getLessonNameLabel(), 1, 2, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->attach(*allRows.back()->getSemesterSpin(), 2, 3, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->attach(*allRows.back()->getIsAdmitedButton(), 3, 4, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->attach(*allRows.back()->getHasPassedButton(), 4, 5, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->attach(*allRows.back()->getMarkSpin(), 5, 6, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->attach(*getDeleteButton(), 6, 7, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL);
	markTable->show_all();

}

void ExamPage::addNothingAddedYetLabelToTable() {
	nothingAddedYetLabel = Gtk::manage(new Gtk::Label);
	Pango::FontDescription fdesc;
	fdesc.set_size(15 * PANGO_SCALE);
	nothingAddedYetLabel->modify_font(fdesc);
	nothingAddedYetLabel->set_markup(MarkPageLabels::NO_EXERCISE_ADDED_LABEL);
	mainBox->pack_end(*nothingAddedYetLabel, Gtk::PACK_SHRINK, 10);
	mainBox->show_all();

}


void ExamPage::saveChangingsButtonClicked() {
	for(unsigned int i = 0; i < allRows.size(); i++) {
		ExamTableRow* curRow = allRows.at(i);
		if(curRow->getStateChanged()) {
			//std::string newToDoUntil = curRow->getUntilEntry()->get_text();
			unsigned short newSemester = curRow->getSemesterSpin()->get_value();
			bool newIsAdmitted = curRow->getIsAdmitedButton()->get_active();
			bool newHasPassed = curRow->getHasPassedButton()->get_active();
			float newMark = curRow->getMarkSpin()->get_value();

			unsigned short updatedDay = curRow->getDaySpin()->get_value_as_int();
			unsigned short updatedMonth = curRow->getMonthSpin()->get_value_as_int();
			unsigned short updatedYear = curRow->getYearSpin()->get_value_as_int();

			TimeConvert timeOpts;
			unsigned int newUntilTimestamp = timeOpts.getUnixTimeFormat(updatedDay,
								updatedMonth, updatedYear);


			// update curRow properties for reset button
			curRow->setUntilTimestamp(std::to_string(newUntilTimestamp));
			curRow->setSemester(newSemester);
			curRow->setIsAdmitted(newIsAdmitted);
			curRow->setHasPassed(newHasPassed);
			curRow->setMark(newMark);

			// update SQL-entry
			SQLiteConnect connection;
			try {
				connection.updateExam(curRow->getID(),
									  newUntilTimestamp,
									  newSemester,
									  newIsAdmitted,
									  newHasPassed,
									  newMark);
			}catch(ERRORS &error) {
				HelpDialogs::showErrorDialog(error);
			}
		}
	}
}
