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

#include "SQLiteConnect.h"
#include <gtkmm.h>

class LessonPage : public Gtk::Frame {
public:
	LessonPage(std::string);
	virtual ~LessonPage();
private:
	std::string curLesson;
	Gtk::Table *exerciseTable;
	Gtk::Label *newExerciseLabel;
	Gtk::Label *exerciseUntilLabel;
	Gtk::Entry *exerciseUntilEntry;
	Gtk::Button *saveNewExerciseButton;
	SQLiteConnect connection;

	void saveButtonClicked();
};

#endif /* LESSONPAGE_H_ */
