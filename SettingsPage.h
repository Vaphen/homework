/*
 * SettingsPage.h
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include "SQLiteConnect.h"
#include "LessonTable.h"
#include <gtkmm.h>

class SettingsPage : public Gtk::Frame {
public:
	SettingsPage(Gtk::Notebook*);
	virtual ~SettingsPage();
private:

	LessonTable *currentLessons;
	Gtk::Label *newLessonLabel;
	Gtk::Entry *newLessonEdit;
	Gtk::Button *saveNewLessonButton;
	Gtk::HBox *mainBox;
	Gtk::VBox *LessonBox;
	void initTable();
	void saveButtonClicked();
	void showErrorDialog(std::string, std::string);
	void showSuccessDialog(std::string, std::string);
	SQLiteConnect connection;
	Gtk::Notebook *notebook;
};

#endif /* SETTINGSPAGE_H_ */
