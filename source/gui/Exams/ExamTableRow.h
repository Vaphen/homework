/*
 * MarkTableRow.h
 *
 *  Created on: 14.01.2015
 *      Author: john
 */

#ifndef GUI_MarkTableRow_H_
#define GUI_MarkTableRow_H_

#include "../../sql/SQLiteConnect.h"
#include <gtkmm.h>
#include <memory>

class ExamTableRow {
public:
	ExamTableRow(std::vector<std::string>);
	ExamTableRow(const unsigned int&, int, const std::string&);
	~ExamTableRow();

	Gtk::SpinButton* getDaySpin() const;
	Gtk::SpinButton* getMonthSpin() const;
	Gtk::SpinButton* getYearSpin() const;
	Gtk::HBox* getUntilBox() const;
	Gtk::SpinButton* getSemesterSpin() const;
	Gtk::CheckButton* getIsAdmitedButton() const;
	Gtk::CheckButton* getHasPassedButton() const;
	Gtk::SpinButton* getMarkSpin() const;
	Gtk::Label* getLessonNameLabel() const;

	std::string getUntilTimestamp() const;
	std::string getLessonName() const;
	unsigned short getSemester() const;
	bool getIsAdmited() const;
	bool getHasPassed() const;
	float getMark() const;

	void setUntilTimestamp(std::string);
	void setSemester(unsigned short);
	void setIsAdmitted(bool);
	void setHasPassed(bool);
	void setMark(float);

	bool getStateChanged() const;
	void setStateChanged(bool);

	unsigned int getID() const;
private:
	unsigned int idInSqlDB;
	std::string toDoUntilTimestamp;
	std::string lessonName;
	unsigned short semester;
	bool isAdmited;
	bool hasPassed;
	float mark;
	bool stateChanged; /**< if it's true, we need to write it to sql db */

	SQLiteConnect connection;
	TimeConvert timeOpts;

	Gtk::SpinButton *daySpin;
	Gtk::SpinButton *monthSpin;
	Gtk::SpinButton *yearSpin;
	Gtk::HBox *untilBox;
	Gtk::Label *lessonNameLabel;
	Gtk::SpinButton *semesterSpin;
	Gtk::CheckButton *isAdmitedButton;
	Gtk::CheckButton *hasPassedButton;
	Gtk::SpinButton *markSpin;

	void initializeWidgets();
	void changeState();
	void changedMarkValue();
	void examDateChanged();
};

#endif /* GUI_MarkTableRow_H_ */
