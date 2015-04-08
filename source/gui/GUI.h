/*
 * GUI.h
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */
#ifndef GUI_H_
#define GUI_H_

#include <string>
#include <gtkmm.h>
#include "Settings/SettingsPage.h"
#include "Statistics/StatisticsPage.h"
#include "Exams/ExamPage.h"
#include "../sql/SQLiteConnect.h"
#include "../constants/constants.h"

#define WINDOW_TITLE "Homework"
#define NOTEBOOK_WELCOME "Start"
#define NOTEBOOK_SETTINGS "Einstellungen"

class GUI : public Gtk::Window {
public:
	GUI();
	virtual ~GUI();
	void addLessonPage(std::string&);
protected:
	/*
	 * Widgets
	 */
	Gtk::VBox *vbox;
	Gtk::EventBox *pageBox;
	Gtk::Notebook *notebook;
	SettingsPage *settings_frame;
	ExamPage *exam_frame;
	StatisticsPage *statistics_frame;
private:
	SQLiteConnect connection;

	Gtk::EventBox *settingsMenueButton;
	Gtk::EventBox *statisticsMenueButton;
	Gtk::EventBox *lessonsMenueButton;
	Gtk::EventBox *marksMenueButton;

	void initializeMenueBar();
	std::vector<std::string> doSqlLessonRequest();
	bool lessonsMenueButtonClicked(GdkEventButton *event);
	bool menueButtonHover(GdkEventCrossing*, MENUE_ENTRIES&);
	bool menueButtonLeave(GdkEventCrossing*, MENUE_ENTRIES&);
	bool menueButtonClicked(GdkEventButton*, MENUE_ENTRIES&);
};

#endif /* GUI_H_ */
