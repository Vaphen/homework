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

	std::vector<std::vector<std::string>> lessons;
	try {
		lessons = connection.getExercises(curLesson);
	}catch(ERRORS &error) {
		Dialogs::showErrorDialog(error);
	}

	for(int cols = 0; cols < lessons.size(); cols++) {
		for(int rows = 0; rows < lessons.at(cols).size(); rows++) {
			Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label(lessons.at(cols).at(rows)));
			exerciseTable->attach(*untilLabel, cols, cols + 1, rows, rows + 1, Gtk::EXPAND, Gtk::EXPAND);
		}
	}
	int numOfRows = (lessons.size() > 0) ? lessons.at(0).size() : 0;
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

