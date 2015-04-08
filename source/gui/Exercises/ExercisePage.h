/*
 * LessonPage.h
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#ifndef LESSONPAGE_H_
#define LESSONPAGE_H_

#define NEW_EXERCISE_LABEL_TEXT "Neue Aufgabe"
#define EXERCISE_UNTIL_LABEL_TEXT "zu erledigen bis:"
#define SAVE_BUTTON_TEXT "Aufgabe speichern"

#include "../../sql/SQLiteConnect.h"
#include "ExerciseTableRow.h"
#include <gtkmm.h>

class ExercisePage : public Gtk::Frame {
public:
	ExercisePage(std::string);
	virtual ~ExercisePage();
private:
	std::string curLesson;
	std::vector<ExerciseTableRow*> allRows;
	std::vector<std::vector<std::string> > exercises;

	SQLiteConnect connection;

	Gtk::VBox *mainBox;
	Gtk::HBox *tableOptionsBox;
	Gtk::Table *exerciseTable;
	Gtk::Label *nothingAddedYetLabel;
	Gtk::Frame *newExerciseFrame;
	Gtk::HBox *newExerciseBox;
	Gtk::EventBox *backgroundTableBox;
	Gtk::ScrolledWindow *tableScroller;


	Gtk::Button *saveChangingsButton;
	Gtk::Button *resetButton;
	Gtk::Button *statisticsButton;

	Gtk::Label *exerciseUntilLabel;
	Gtk::SpinButton *exerciseUntilDaySpin;
	Gtk::SpinButton *exerciseUntilMonthSpin;
	Gtk::SpinButton *exerciseUntilYearSpin;
	Gtk::Button *saveNewExerciseButton;

	Gtk::Button* getDeleteButton();

	void resetRowsClicked();
	void saveNewExerciseButtonClicked();
	void saveChangingsButtonClicked();
	void deleteButtonClicked(ExerciseTableRow&, Gtk::Button*);
	void statisticsButtonClicked();
	void newExerciseDateChanged();
	void initializeWidgets();
	void initializeTableMenueBar();
	void initializeNewExerciseBox();
	void initializeExerciseTable();
	void addNothingAddedYetLabelToTable(); // :D
	void addRowToTable();
};

#endif /* LESSONPAGE_H_ */
