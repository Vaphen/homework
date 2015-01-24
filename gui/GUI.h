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
private:
	SQLiteConnect connection;
	std::vector<std::string> doSqlLessonRequest();
};

#endif /* GUI_H_ */
