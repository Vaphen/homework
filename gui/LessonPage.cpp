/*
 * LessonPage.cpp
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#include "LessonPage.h"
#include "../constants/constants.h"
#include "../constants/HelpDialogs.h"
#include "../constants/Labels.h"
#include "LessonTableRow.h"
#include <iostream>
#include <gtkmm.h>
#include <string>
#include <functional>

LessonPage::LessonPage(std::string pageTitle) :
		curLesson(pageTitle),
		mainBox(new Gtk::VBox),
		exerciseTable(new Gtk::Table),
		newExerciseFrame(new Gtk::Frame),
		newExerciseBox(new Gtk::HBox),
		tableScroller(new Gtk::ScrolledWindow){
	set_label(LessonPageLabels::FRAME_HEADER + curLesson);

	initializeExerciseTable();

	initializeNewExerciseBox();

	tableScroller->add(*exerciseTable);
	tableScroller->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	mainBox->pack_start(*tableScroller, Gtk::PACK_EXPAND_WIDGET, 0);
	mainBox->pack_start(*newExerciseFrame, Gtk::PACK_SHRINK, 0);
	this->add(*mainBox);
}

LessonPage::~LessonPage() {
	delete exerciseTable;
	delete mainBox;
	delete newExerciseFrame;
	delete newExerciseBox;
	delete tableScroller;
}

void LessonPage::initializeNewExerciseBox() {
	Gtk::Label *exerciseUntilLabel = Gtk::manage(new Gtk::Label(EXERCISE_UNTIL_LABEL_TEXT));
	Gtk::Entry *exerciseUntilEntry = Gtk::manage(new Gtk::Entry);
	Gtk::Button *saveNewExerciseButton = Gtk::manage(new Gtk::Button(SAVE_BUTTON_TEXT));

	exerciseUntilEntry->set_max_length(10);

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
			LessonTableRow newLessonRow(curRow);
			allRows.push_back(newLessonRow);
			addRowToTable();
		}
	}
	exerciseTable->show_all();
}

/**
 * add exercise to the SQL-DB: triggered if button is clicked
 * @param exerciseUntilEntry entrybox with date, until it must be finished
 */
void LessonPage::saveButtonClicked(Gtk::Entry *exerciseUntilEntry) {
	if(exerciseUntilEntry->get_text() == "" || !isValidDate(exerciseUntilEntry->get_text())) {
		HelpDialogs::showErrorDialog("Das Feld 'bis' darf nicht leer sein.",
								 "Bitte füllen Sie das Feld aus.");
		return;
	}
	try {
		connection.addNewExercise(curLesson, FOLDER_PATH + this->curLesson + "/");
		LessonTableRow newRow(exerciseUntilEntry->get_text(), allRows.back().getID() + 1);
		allRows.push_back(newRow);
		addRowToTable();
	} catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
	}
}

/**
 * returns a delete-button with delete icon. button is connected
 * with deleteButtonClicked function, binded to id of its row and
 * to a pointer to the widget itself, just for deleting it by clicking
 * on it
 */
Gtk::Button* LessonPage::getDeleteButton() {
	Gtk::Image *deleteIcon = Gtk::manage(new Gtk::Image(DELETE_ICO));
	Gtk::Button *deleteButton = Gtk::manage(new Gtk::Button);
	deleteButton->set_image(*deleteIcon);
	deleteButton->set_size_request(50, 50);
	deleteButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);
	deleteButton->signal_clicked().connect(sigc::bind<int, Gtk::Button*>(
				sigc::mem_fun(*this, &LessonPage::deleteButtonClicked), allRows.back().getID(), deleteButton));
	return deleteButton;
}

/**
 * @param exerciseId the id of the row which the button should delete
 * @param *deleteButton pointer to the widget itself that we can delete it
 */
void LessonPage::deleteButtonClicked(int exerciseId, Gtk::Button *deleteButton) {
	if(HelpDialogs::showConfirmDialog(HelpDialogs::CONFIRM_DELETION,
			HelpDialogs::CONFIRM_DELETION_SUBTEXT) != HelpDialogs::CONFIRMED)
		return;
	for(int i = 0; i < allRows.size(); i++) {
		if(allRows.at(i).getID() == exerciseId) {
			try {
				connection.deleteExercise(this->curLesson, allRows.at(i).getID());
			} catch(ERRORS &error) {
				HelpDialogs::showErrorDialog(error);
				return;
			}
			exerciseTable->remove(*allRows.at(i).getCommentTextView());
			exerciseTable->remove(*allRows.at(i).getExerciseFinishedButton());
			exerciseTable->remove(*allRows.at(i).getReachedPointsEntry());
			exerciseTable->remove(*allRows.at(i).getTotalPointsEntry());
			exerciseTable->remove(*allRows.at(i).getOpenFolderButton());
			exerciseTable->remove(*allRows.at(i).getUntilLabel());
			exerciseTable->remove(*deleteButton);
			allRows.erase(allRows.begin() + i);
			exerciseTable->show_all();
			return;
		}
	}
}

void LessonPage::addRowToTable() {
	exerciseTable->attach(*allRows.back().getUntilLabel(), 0, 1, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back().getReachedPointsEntry(), 1, 2, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back().getTotalPointsEntry(), 2, 3, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back().getOpenFolderButton(), 3, 4, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back().getExerciseFinishedButton(), 4, 5, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*allRows.back().getCommentTextView(), 5, 6, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->attach(*getDeleteButton(), 6, 7, allRows.size() + 1, allRows.size() + 2, Gtk::EXPAND, Gtk::FILL);
	exerciseTable->show_all();
}


