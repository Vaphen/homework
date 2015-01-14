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
#include <gtkmm.h>

class LessonPage : public Gtk::Frame {
public:
	LessonPage(std::string);
	virtual ~LessonPage();
private:
	std::string curLesson;
	std::vector<std::vector<std::string>> exercises;

	SQLiteConnect connection;

	Gtk::VBox *mainBox;
	Gtk::Table *exerciseTable;
	Gtk::Frame *newExerciseFrame;
	Gtk::HBox *newExerciseBox;

	void saveButtonClicked(Gtk::Entry*);
	void attachExerciseToTable(int, int);
	void initializeNewExerciseBox();
	void initializeExerciseTable();
};

#endif /* LESSONPAGE_H_ */
