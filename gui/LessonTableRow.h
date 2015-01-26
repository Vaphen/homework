/*
 * LessonTableRow.h
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#ifndef GUI_LESSONTABLEROW_H_
#define GUI_LESSONTABLEROW_H_

#include <gtkmm.h>
#include <functional>


class LessonTableRow {
public:
	LessonTableRow(std::vector<std::string>);
	LessonTableRow(std::string, int);
	~LessonTableRow();

	Gtk::Label* getUntilLabel();
	Gtk::SpinButton* getReachedPointsSpin();
	Gtk::SpinButton* getTotalPointsSpin();
	Gtk::Button* getOpenFolderButton();
	Gtk::CheckButton* getExerciseFinishedButton();
	Gtk::TextView* getCommentTextView();
	int getID();

private:
	int idInSqlDB;
	std::string toDoUntil;
	unsigned int reachedPoints;
	unsigned int totalPoints;
	std::string folderPath;
	bool exerciseFinished;
	std::string exerciseComment;
	bool stateChanged; /**< if it's true, we need to write it to sql db */


	Gtk::Label *untilLabel;
	Gtk::SpinButton *reachedPointsSpin;
	Gtk::SpinButton *totalPointsSpin;
	Gtk::Button *openFolderButton;
	Gtk::Image *openFolderButtonImage;
	Gtk::CheckButton *exerciseFinishedButton;
	Gtk::TextView *commentTextView;

	void initializeWidgets();
	void openFolderButtonClicked();
	void changeState();
};

#endif /* GUI_LESSONTABLEROW_H_ */
