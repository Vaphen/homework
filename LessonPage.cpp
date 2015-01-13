/*
 * LessonPage.cpp
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#include "LessonPage.h"
#include "constants.h"
#include <iostream>
#include <gtkmm.h>

LessonPage::LessonPage(std::string pageTitle) :
		curLesson(pageTitle),
		exerciseTable(new Gtk::Table),
		newExerciseLabel(new Gtk::Label(NEW_EXERCISE_LABEL_TEXT)),
		exerciseUntilLabel(new Gtk::Label(EXERCISE_UNTIL_LABEL_TEXT)),
		exerciseUntilEntry(new Gtk::Entry),
		saveNewExerciseButton(new Gtk::Button(SAVE_BUTTON_TEXT)) {
	set_label("Fach: " + curLesson);

	saveNewExerciseButton->signal_clicked().connect(sigc::mem_fun(*this, &LessonPage::saveButtonClicked));


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

		untilLabel->set_markup(TABLE_LABELS::UNTIL);
		reachedPointsLabel->set_markup(TABLE_LABELS::REACHED_POINTS);
		totalPointsLabel->set_markup(TABLE_LABELS::TOTAL_POINTS);
		finishedLabel->set_markup(TABLE_LABELS::EXERCISE_FINISHED);
		commentLabel->set_markup(TABLE_LABELS::EXERCISE_COMMENT);

		exerciseTable->attach(*untilLabel, 0, 1, 0, 1, Gtk::EXPAND, Gtk::FILL);
		exerciseTable->attach(*reachedPointsLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::FILL);
		exerciseTable->attach(*totalPointsLabel, 2, 3, 0, 1, Gtk::EXPAND, Gtk::FILL);
		exerciseTable->attach(*finishedLabel, 3, 4, 0, 1, Gtk::EXPAND, Gtk::FILL);
		exerciseTable->attach(*commentLabel, 4, 5, 0, 1, Gtk::EXPAND, Gtk::FILL);

		for(int cols = 0; cols < exercises.size(); cols++) {
			std::cout << "in first loop" << exercises.at(0).size() << std::endl;
			// rows has to start by one because of first row is used by heading-labels
			for(int rows = 1; rows < exercises.at(cols).size() + 1; rows++) {
				attachExerciseToTable(rows, cols);
			}
		}
	}





	exerciseTable->attach(*newExerciseLabel, 0, 4, numOfRows, numOfRows + 1, Gtk::FILL, Gtk::FILL);
	exerciseTable->attach(*exerciseUntilLabel, 0, 2, numOfRows + 1, numOfRows + 2, Gtk::FILL, Gtk::FILL);
	exerciseTable->attach(*exerciseUntilEntry, 2, 4, numOfRows + 1, numOfRows + 2, Gtk::FILL, Gtk::FILL);
	exerciseTable->attach(*saveNewExerciseButton, 0, 4, numOfRows + 2, numOfRows + 3, Gtk::FILL, Gtk::FILL);
	exerciseTable->show_all();
	try {
	connection.getExercises(pageTitle);
	} catch(ERRORS &error) {
		Dialogs::showErrorDialog(error);
	}
	this->add(*exerciseTable);
}

LessonPage::~LessonPage() {
	delete exerciseTable;
	delete newExerciseLabel;
	delete exerciseUntilLabel;
	delete saveNewExerciseButton;
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
			reachedPointsEntry->set_text(exercises.at(COLUMN_ID::REACHED_POINTS).at(curRowIndex));
			exerciseTable->attach(*reachedPointsEntry, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::TOTAL_POINTS: {
			Gtk::Entry *totalPointsEntry = Gtk::manage(new Gtk::Entry);
			totalPointsEntry->set_text(exercises.at(COLUMN_ID::TOTAL_POINTS).at(curRowIndex));
			exerciseTable->attach(*totalPointsEntry, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::EXERCISE_FINISHED: {
			Gtk::CheckButton *exerciseFinishedButton = Gtk::manage(new Gtk::CheckButton);
			exerciseTable->attach(*exerciseFinishedButton, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
		case COLUMN_ID::EXERCISE_COMMENT: {
			Gtk::Entry *commentEntry = Gtk::manage(new Gtk::Entry);
			commentEntry->set_text(exercises.at(COLUMN_ID::EXERCISE_COMMENT).at(curRowIndex));
			exerciseTable->attach(*commentEntry, colNum, colNum + 1, rowNum, rowNum + 1, Gtk::EXPAND, Gtk::FILL);
			break;
		}
	}
}

void LessonPage::saveButtonClicked() {
	if(exerciseUntilEntry->get_text() == "") {
		Dialogs::showErrorDialog("Das Feld 'bis' darf nicht leer sein.",
								 "Bitte füllen Sie das Feld aus.");
	}
	try {
		connection.addNewExercise(curLesson, "12.20.1031");
	} catch(ERRORS &error) {
		Dialogs::showErrorDialog(error);
	}
}

