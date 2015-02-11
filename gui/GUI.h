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
#include "SettingsPage.h"
#include "StatisticsPage.h"
#include "../sql/SQLiteConnect.h"

#define WINDOW_TITLE "Homework"
#define NOTEBOOK_WELCOME "Start"
#define NOTEBOOK_SETTINGS "Einstellungen"

class GUI : public Gtk::Window {
public:
	GUI();
	virtual ~GUI();
	void addLessonPage(std::string);
protected:
	/*
	 * Widgets
	 */
	Gtk::Notebook *notebook;
	SettingsPage *settings_frame;
	StatisticsPage *statistics_frame;
private:
	SQLiteConnect connection;
	std::vector<std::string> doSqlLessonRequest();
	void on_my_switch_page(GtkNotebookPage*, unsigned int page_num);
};

#endif /* GUI_H_ */
