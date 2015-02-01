/*
 * LessonTableRow.h
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#ifndef GUI_LESSONTABLEROW_H_
#define GUI_LESSONTABLEROW_H_

#include <gtkmm.h>
#include <memory>

class LessonTableRow {
public:
	LessonTableRow(std::vector<std::string>);
	LessonTableRow(const std::string&, int, const std::string&);
	~LessonTableRow();

	Gtk::Label* getUntilLabel() const;
	Gtk::SpinButton* getReachedPointsSpin() const;
	Gtk::SpinButton* getTotalPointsSpin()const;
	Gtk::Button* getOpenFolderButton() const;
	Gtk::Button* getOpenExercisePDF() const;
	Gtk::CheckButton* getExerciseFinishedButton() const;
	Gtk::TextView* getCommentTextView() const;
	bool getStateChanged() const;
	unsigned int getReachedPoints() const;
	unsigned int getTotalPoints() const;
	bool getIsExerciseFinished() const;
	void setStateChanged(bool);
	std::string getComment() const;
	std::string getUntil() const;
	int getID() const;
private:
	int idInSqlDB;
	std::string toDoUntil;
	unsigned int reachedPoints;
	unsigned int totalPoints;
	bool isExerciseFinished;
	std::string exerciseComment;
	std::string lessonName;
	bool stateChanged; /**< if it's true, we need to write it to sql db */


	Gtk::Label *untilLabel;
	Gtk::SpinButton *reachedPointsSpin;
	Gtk::SpinButton *totalPointsSpin;
	Gtk::Button *openFolderButton;
	Gtk::Image *openFolderButtonImage;
	Gtk::Button *openExercisePDFButton;
	Gtk::Image *openExercisePDFImage;
	Gtk::CheckButton *exerciseFinishedButton;
	Gtk::TextView *commentTextView;

	void initializeWidgets();
	void openFolderButtonClicked();
	void openExercisePDFButtonClicked();
	void changeState();
};

#endif /* GUI_LESSONTABLEROW_H_ */
