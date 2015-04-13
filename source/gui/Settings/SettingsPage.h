/*
 * SettingsPage.h
 *
 *  Created on: 25.12.2014
 *      Author: vaphen
 */

#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include "../../sql/SQLiteConnect.h"
#include "../Exercises/ExerciseFrame.h"
#include "../Exams/ExamPage.h"
#include <gtkmm.h>
#include <regex>
#include "SettingsLessonTable.h"

class SettingsPage : public Gtk::Frame {
public:
	SettingsPage(ExerciseFrame&, ExamPage&);
	virtual ~SettingsPage();
private:
	ExerciseFrame &exerciseFrame;
	ExamPage &examFrame;
	SettingsLessonTable *lessonTable;
	Gtk::Entry *newLessonEdit;
	Gtk::Frame *environmentSettingsFrame;
	Gtk::Entry *fileDirPathEdit;
	Gtk::Entry *pdfExecutablePathEdit;
	Gtk::Entry *fileManagerPathEdit;
	Gtk::Frame *onlineSettingsFrame;
	Gtk::CheckButton *enableOnlineSync;
	Gtk::Entry *usernameEdit;
	Gtk::Entry *passwordEdit;
	Gtk::CheckButton *stayLoggedIn;
	Gtk::Button *loginButton;

	Gtk::VBox *settingsVBox;
	void initWidgets();
	void initTable();
	void initializeNewLessonBox();
	void initializeEnvironmentSettings();
	void initializeOnlineSettings();
	void showCenteredWidgets();
	void saveNewLessonButtonClicked();
	void saveFileDirPathButtonClicked();
	void deleteButtonClicked();
	void chooseFileDirButtonClicked();
	void saveEnvironmentSettingsClicked();
	void choosePdfExecutableButtonClicked();
	void chooseFileManagerButtonClicked();
	void enableOnlineSyncClicked();
	SQLiteConnect connection;
};

#endif /* SETTINGSPAGE_H_ */
