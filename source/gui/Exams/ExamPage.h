/*
 * MarkPage.h
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#ifndef MarkPage_H_
#define MarkPage_H_

#include "../../sql/SQLiteConnect.h"
#include "ExamTableRow.h"
#include <gtkmm.h>

class ExamPage : public Gtk::Frame {
public:
	ExamPage();
	virtual ~ExamPage();
	void appendLessonToLessonCombo(std::string);
	void removeLessonFromLessonCombo(std::string);
private:
	std::vector<ExamTableRow*> allRows;
	std::vector<std::vector<std::string> > exams;

	SQLiteConnect connection;

	Gtk::VBox *mainBox;
	Gtk::HBox *tableOptionsBox;
	Gtk::Table *markTable;
	Gtk::Label *nothingAddedYetLabel;
	Gtk::Frame *newExerciseFrame;
	Gtk::HBox *newExerciseBox;
	Gtk::ScrolledWindow *tableScroller;

	// used to get selected value
	Gtk::TreeModelColumn<Glib::ustring> lessonHeader;

	Gtk::Button *saveChangingsButton;
	Gtk::Button *resetButton;

	Gtk::ComboBox *lessonCombo;
	Gtk::TreeModel::ColumnRecord allHeader;
	Glib::RefPtr<Gtk::ListStore> allLessons;

	Gtk::Button* getDeleteButton();

	void resetRowsClicked();
	void saveNewExamButtonClicked(Gtk::SpinButton*, Gtk::ComboBox*);
	void saveChangingsButtonClicked();
	void deleteButtonClicked(ExamTableRow&, Gtk::Button*);
	void initializeWidgets();
	void initializeTableMenueBar();
	void initializeNewExerciseBox();
	void initializeMarkTable();
	void addNothingAddedYetLabelToTable(); // :D
	void addRowToTable();
};

#endif /* MarkPage_H_ */
