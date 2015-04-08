/*
 * LessonTable.h
 *
 *  Created on: 07.01.2015
 *      Author: vaphen
 */

#ifndef SETTINGSLESSONTABLE_H_
#define SETTINGSLESSONTABLE_H_

#include <gtkmm.h>

class SettingsLessonTable : public Gtk::TreeView {
public:
	SettingsLessonTable();
	virtual ~SettingsLessonTable();
	void appendLesson(Glib::ustring);
	void deleteSelectedLesson();
	Glib::ustring getSelectedLesson();
private:
	Gtk::TreeModelColumn<Glib::ustring> *lessonHeader;
	Gtk::TreeModel::ColumnRecord *allHeader;
	Glib::RefPtr<Gtk::ListStore> allLessons;
	std::vector<std::string> lessons;
};

#endif /* SETTINGSLESSONTABLE_H_ */
