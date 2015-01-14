/*
 * LessonPage.cpp
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#include "LessonPage.h"
#include "../constants/constants.h"
#include "LessonTableRow.h"
#include <iostream>
#include <gtkmm.h>
#include <string>

LessonPage::LessonPage(std::string pageTitle) :
		curLesson(pageTitle),
		mainBox(new Gtk::VBox),
		exerciseTable(new Gtk::Table),
		newExerciseFrame(new Gtk::Frame),
		newExerciseBox(new Gtk::HBox){
	set_label("Fach: " + curLesson);

	initializeExerciseTable();

	initializeNewExerciseBox();

	mainBox->pack_start(*exerciseTable, Gtk::PACK_EXPAND_WIDGET, 0);
	mainBox->pack_start(*newExerciseFrame, Gtk::PACK_EXPAND_PADDING, 0);

	this->add(*mainBox);
}

LessonPage::~LessonPage() {
	delete exerciseTable;
	delete mainBox;
	delete newExerciseFrame;
	delete newExerciseBox;
}

void LessonPage::initializeNewExerciseBox() {
	Gtk::Label *exerciseUntilLabel = Gtk::manage(new Gtk::Label(EXERCISE_UNTIL_LABEL_TEXT));
	Gtk::Entry *exerciseUntilEntry = Gtk::manage(new Gtk::Entry);
	Gtk::Button *saveNewExerciseButton = Gtk::manage(new Gtk::Button(SAVE_BUTTON_TEXT));

	newExerciseBox->pack_start(*exerciseUntilLabel, Gtk::PACK_SHRINK, 0);
	newExerciseBox->pack_start(*exerciseUntilEntry, Gtk::PACK_SHRINK, 0);
	newExerciseBox->pack_start(*saveNewExerciseButton, Gtk::PACK_SHRINK, 0);

	saveNewExerciseButton->signal_clicked().connect(sigc::bind<Gtk::Entry*>(
			sigc::mem_fun(*this, &LessonPage::saveButtonClicked), exerciseUntilEntry));

	newExerciseFrame->set_label(NEW_EXERCISE_LABEL_TEXT);
	newExerciseFrame->add(*newExerciseBox);
}

void LessonPage::initializeExerciseTable() {
	try {
		exercises = connection.getExercises(curLesson);
	}catch(ERRORS &error) {
		Dialogs::showErrorDialog(error);
	}


	int numOfRows = (exercises.size() > 0) ? exercises.at(0).size() : 0;
	if(numOfRows != 0) {
		Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *reachedPointsLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *totalPointsLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *finishedLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *commentLabel = Gtk::manage(new Gtk::Label);
		Gtk::Label *deleteLabel = Gtk::manage(new Gtk::Label);

		untilLabel->set_markup(TABLE_LABELS::UNTIL);
		reachedPointsLabel->set_markup(TABLE_LABELS::REACHED_POINTS);
		totalPointsLabel->set_markup(TABLE_LABELS::TOTAL_POINTS);
		finishedLabel->set_markup(TABLE_LABELS::EXERCISE_FINISHED);
		commentLabel->set_markup(TABLE_LABELS::EXERCISE_COMMENT);
		deleteLabel->set_markup(TABLE_LABELS::DELETE_EXERCISE);

		exerciseTable->attach(*untilLabel, 0, 1, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*reachedPointsLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*totalPointsLabel, 2, 3, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*finishedLabel, 3, 4, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*commentLabel, 4, 5, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);
		exerciseTable->attach(*deleteLabel, 5, 6, 0, 1, Gtk::EXPAND, Gtk::FILL, 20, 0);



		std::vector<LessonTableRow> allRows;
		/**
		 * TODO: check, if a class of type Gtk::Label with static label-counter isnt better for
		 * this loop (or at all).
		 */
		for(std::vector<std::string> curRow : exercises) {
			allRows.push_back(curRow);
		}

		// rows has to start by one because of first row is used by heading-labels
		for(int rows = 1; rows < allRows.size() + 1; rows++) {
			exerciseTable->attach(*allRows.at(rows - 1).getUntilLabel(), 0, 1, rows, rows + 1, Gtk::EXPAND, Gtk::FILL);
			exerciseTable->attach(*allRows.at(rows - 1).getReachedPointsEntry(), 1, 2, rows, rows + 1, Gtk::EXPAND, Gtk::FILL);
			exerciseTable->attach(*allRows.at(rows - 1).getTotalPointsEntry(), 2, 3, rows, rows + 1, Gtk::EXPAND, Gtk::FILL);
			exerciseTable->attach(*allRows.at(rows - 1).getExerciseFinishedButton(), 3, 4, rows, rows + 1, Gtk::EXPAND, Gtk::FILL);
			exerciseTable->attach(*allRows.at(rows - 1).getCommentTextView(), 4, 5, rows, rows + 1, Gtk::EXPAND, Gtk::FILL);



			Gtk::Button *deleteButton = new Gtk::Button;
			/**
			 * TODO: make this a better path solution (with constants)
			 */
			Gtk::Image  *deleteIcon = Gtk::manage(new Gtk::Image("src/delete.png"));
			deleteButton->set_image(*deleteIcon);
			deleteButton->set_size_request(50, 50);
			deleteButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);
			exerciseTable->attach(*deleteButton, 5, 6, rows, rows + 1, Gtk::EXPAND, Gtk::FILL);
		}
	}
	exerciseTable->show_all();
}

/**
 * attaches a new entry to the table in the given position
 * @param rowNum: number of Row in which should be inserted
 * @param colNum: number of Col in which should be inserted
 */
void LessonPage::attachExerciseToTable(int rowNum, int colNum) {
	int curRowIndex = rowNum - 1;
	switch(colNum) {
		case COLUMN_ID::UNTIL: {
			Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label(exercises.at(COLUMN_ID::UNTIL).at(curRowIndex)));
			exerciseTable->attach(*untilLabel, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::REACHED_POINTS: {
			Gtk::Entry *reachedPointsEntry = Gtk::manage(new Gtk::Entry);
			reachedPointsEntry->set_size_request(50, 30);
			reachedPointsEntry->set_text(exercises.at(COLUMN_ID::REACHED_POINTS).at(curRowIndex));
			exerciseTable->attach(*reachedPointsEntry, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::TOTAL_POINTS: {
			Gtk::Entry *totalPointsEntry = Gtk::manage(new Gtk::Entry);
			totalPointsEntry->set_size_request(50, 30);
			totalPointsEntry->set_text(exercises.at(COLUMN_ID::TOTAL_POINTS).at(curRowIndex));
			exerciseTable->attach(*totalPointsEntry, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::EXERCISE_FINISHED: {
			Gtk::CheckButton *exerciseFinishedButton = Gtk::manage(new Gtk::CheckButton);
			if(exercises.at(COLUMN_ID::EXERCISE_FINISHED).at(curRowIndex) == "1")
				exerciseFinishedButton->set_active(true);

			exerciseTable->attach(*exerciseFinishedButton, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::EXERCISE_COMMENT: {
			Gtk::TextView *commentEntry = Gtk::manage(new Gtk::TextView);
			Glib::RefPtr<Gtk::TextBuffer> defaultText = Gtk::TextBuffer::create();

			defaultText->set_text(exercises.at(COLUMN_ID::EXERCISE_COMMENT).at(curRowIndex));
			commentEntry->set_size_request(130, 50);
			commentEntry->set_border_width(1);
			commentEntry->set_justification(Gtk::JUSTIFY_FILL);
			commentEntry->set_buffer(defaultText);
			exerciseTable->attach(*commentEntry, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		default:
			Dialogs::showErrorDialog("Es ist ein unbekannter Fehler aufgetreten.",
									 "Eine Spalte konnte nicht zur Aufgabentabelle hinzugefügt werden.");
			return;
	}
}

/**
 * add exercise to the SQL-DB: triggered if button is clicked
 * @param exerciseUntilEntry entrybox with date, until it must be finished
 */
void LessonPage::saveButtonClicked(Gtk::Entry *exerciseUntilEntry) {
	std::cout << "text:" << exerciseUntilEntry->get_text() << std::endl;
	if(exerciseUntilEntry->get_text() == "") {
		Dialogs::showErrorDialog("Das Feld 'bis' darf nicht leer sein.",
								 "Bitte füllen Sie das Feld aus.");
		return;
	}
	try {
		connection.addNewExercise(curLesson, exerciseUntilEntry->get_text());
	} catch(ERRORS &error) {
		Dialogs::showErrorDialog(error);
	}
}

