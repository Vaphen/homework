/*
 * LessonTable.cpp
 *
 *  Created on: 07.01.2015
 *      Author: vaphen
 */

#include "LessonTable.h"
#include "SQLiteConnect.h"
#include "constants.h"

LessonTable::LessonTable() :
		lessonHeader(new Gtk::TreeModelColumn<Glib::ustring>),
		allHeader(new Gtk::TreeModel::ColumnRecord) {

	allHeader->add(*lessonHeader);
	set_model(allLessons = Gtk::ListStore::create(*allHeader));
	SQLiteConnect connection;
	connection.open_db(Database::LESSON_DB);
	std::vector<std::string> lessons = connection.getLessons();
	for(std::string &lesson : lessons) {
		appendLesson(lesson);
	}
	connection.close_db();
	append_column("Fächer", *lessonHeader);
	show_all_children();
}

LessonTable::~LessonTable() {
	delete allHeader;
	delete lessonHeader;
}

void LessonTable::appendLesson(Glib::ustring lessonName) {
	Gtk::TreeModel::Row row = *(allLessons->append());
	row[*lessonHeader] = lessonName;
}

