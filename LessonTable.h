/*
 * LessonTable.h
 *
 *  Created on: 07.01.2015
 *      Author: vaphen
 */

#ifndef LESSONTABLE_H_
#define LESSONTABLE_H_

#include <gtkmm.h>
#include "SQLiteConnect.h"

class LessonTable : public Gtk::TreeView {
public:
	LessonTable();
	virtual ~LessonTable();
private:
	Gtk::TreeModelColumn<Glib::ustring> *lessonHeader;
	Gtk::TreeModel::ColumnRecord *allHeader;
	Glib::RefPtr<Gtk::ListStore> allLessons;
	void appendLesson(Glib::ustring);
};

#endif /* LESSONTABLE_H_ */
