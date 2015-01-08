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
#include "SQLiteConnect.h"

#define WINDOW_TITLE "Homework"
#define NOTEBOOK_WELCOME "Start"
#define NOTEBOOK_SETTINGS "Einstellungen"
#define WELCOME_TEXT "Hallo zur ersten Version der Hausaufgaben-Software.\nWenn Sie Fertig ist, kommen noch viele andere tolle Funktionen hinzu.\n"

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
	Gtk::Frame *welcome_frame;
	Gtk::Label *welcome_text;
	SettingsPage *settings_frame;
private:
	SQLiteConnect connection;
	void showErrorDialog(std::string, std::string);
	std::vector<std::string> doSqlLessonRequest();
};

#endif /* GUI_H_ */
