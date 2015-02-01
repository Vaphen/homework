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
	Gtk::Entry *newLessonEdit;
	Gtk::Frame *environmentSettingsFrame;
	Gtk::Entry *fileDirPathEdit;
	Gtk::Entry *pdfExecutablePathEdit;
	Gtk::Entry *fileManagerPathEdit;

	Gtk::VBox *settingsVBox;
	void initWidgets();
	void initTable();
	void initializeNewLessonBox();
	void initializeEnvironmentSettings();
	void showCenteredWidgets();
	void saveNewLessonButtonClicked();
	void saveFileDirPathButtonClicked();
	void deleteButtonClicked();
	void chooseFileDirButtonClicked();
	void saveEnvironmentSettingsClicked();
	void choosePdfExecutableButtonClicked();
	void chooseFileManagerButtonClicked();
	SQLiteConnect connection;
	Gtk::Notebook *notebook;
};

#endif /* SETTINGSPAGE_H_ */
