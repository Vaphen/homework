/*
 * LessonTable.h
 *
 *  Created on: 07.01.2015
 *      Author: vaphen
 */

#ifndef SETTINGSLESSONTABLE_H_
#define SETTINGSLESSONTABLE_H_

#include <gtkmm.h>
#include "../sql/SQLiteConnect.h"

class LessonTable : public Gtk::TreeView {
public:
	LessonTable();
	virtual ~LessonTable();
	void appendLesson(Glib::ustring);
	void deleteSelectedLesson();
	Glib::ustring getSelectedLesson();
private:
	Gtk::TreeModelColumn<Glib::ustring> *lessonHeader;
	Gtk::TreeModel::ColumnRecord *allHeader;
	Glib::RefPtr<Gtk::ListStore> allLessons;
};

#endif /* SETTINGSLESSONTABLE_H_ */
