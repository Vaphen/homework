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

#include "../sql/SQLiteConnect.h"
#include "LessonTableRow.h"
#include <gtkmm.h>

class LessonPage : public Gtk::Frame {
public:
	LessonPage(std::string, Gtk::Notebook*);
	virtual ~LessonPage();
private:
	std::string curLesson;
	std::vector<LessonTableRow*> allRows;
	std::vector<std::vector<std::string> > exercises;

	SQLiteConnect connection;

	Gtk::Notebook *parentNotebook;

	Gtk::VBox *mainBox;
	Gtk::HBox *tableOptionsBox;
	Gtk::Table *exerciseTable;
	Gtk::Label *nothingAddedYetLabel;
	Gtk::Frame *newExerciseFrame;
	Gtk::HBox *newExerciseBox;
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
	void deleteButtonClicked(LessonTableRow&, Gtk::Button*);
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
