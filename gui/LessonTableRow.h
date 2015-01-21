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
	Gtk::Entry* getReachedPointsEntry();
	Gtk::Entry* getTotalPointsEntry();
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

	Gtk::Label *untilLabel;
	Gtk::Entry *reachedPointsEntry;
	Gtk::Entry *totalPointsEntry;
	Gtk::Button *openFolderButton;
	Gtk::Image *openFolderButtonImage;
	Gtk::CheckButton *exerciseFinishedButton;
	Gtk::TextView *commentTextView;

	void initializeWidgets();
	void openFolderButtonClicked();

};

#endif /* GUI_LESSONTABLEROW_H_ */
