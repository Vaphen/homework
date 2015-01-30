/*
 * SettingsPage.h
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include "../sql/SQLiteConnect.h"
#include <gtkmm.h>
#include "SettingsLessonTable.h"

class SettingsPage : public Gtk::Frame {
public:
	SettingsPage(Gtk::Notebook*);
	virtual ~SettingsPage();
private:

	LessonTable *lessonTable;
	Gtk::HSeparator *separatorToNewLessonButtons;
	Gtk::Label *newLessonLabel;
	Gtk::Entry *newLessonEdit;
	Gtk::Button *saveNewLessonButton;
	Gtk::Button *deleteLessonButton;
	Gtk::Frame *environmentSettingsFrame;
	Gtk::VBox *environmentSettingsVBox;
	Gtk::Label *fileDirPathLabel;
	Gtk::Entry *fileDirPathEdit;
	Gtk::Button *saveFileDirPathButton;

	Gtk::HBox *mainBox;
	Gtk::VBox *settingsVBox;
	void initWidgets();
	void initTable();
	void initializeEnvironmentSettings();
	void saveNewLessonButtonClicked();
	void saveFileDirPathButtonClicked();
	void deleteButtonClicked();
	void chooseFileDirButtonClicked();
	SQLiteConnect connection;
	Gtk::Notebook *notebook;
};

#endif /* SETTINGSPAGE_H_ */
