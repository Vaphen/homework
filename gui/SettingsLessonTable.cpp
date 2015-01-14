/*
 * LessonTable.cpp
 *
 *  Created on: 07.01.2015
 *      Author: vaphen
 */

#include "SettingsLessonTable.h"
#include <iostream>

#include "../sql/SQLiteConnect.h"
#include "../constants/constants.h"

LessonTable::LessonTable() :
		lessonHeader(new Gtk::TreeModelColumn<Glib::ustring>),
		allHeader(new Gtk::TreeModel::ColumnRecord) {

	allHeader->add(*lessonHeader);
	set_model(allLessons = Gtk::ListStore::create(*allHeader));
	SQLiteConnect connection;
	std::vector<std::string> lessons = {};
	try {
		lessons = connection.getLessons();
	}catch(ERRORS &error) {
		Dialogs::showErrorDialog(error);
	}

	for(std::string &lesson : lessons) {
		appendLesson(lesson);
	}
	append_column("Fächer", *lessonHeader);
	show_all_children();
}

LessonTable::~LessonTable() {
	delete allHeader;
	delete lessonHeader;
}

/**
 * appands a given lesson to the TreeView
 */
void LessonTable::appendLesson(Glib::ustring lessonName) {
	Gtk::TreeModel::Row row = *(allLessons->append());
	row[*lessonHeader] = lessonName;
}

/**
 * delete the selected lesson from the treeView
 */
void LessonTable::deleteSelectedLesson() {
	Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();
	if(selection->count_selected_rows() == 0)
		return;
	Gtk::TreeModel::iterator selectedRow = selection->get_selected();
	Gtk::TreeModel::Row row = *selectedRow;
	allLessons->erase(selectedRow);
}

/**
 * returns the name of the selected lesson
 */
Glib::ustring LessonTable::getSelectedLesson() {
	Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();
	if(selection->count_selected_rows() == 0)
		return "";
	Gtk::TreeModel::iterator selectedRow = selection->get_selected();
	Gtk::TreeModel::Row row = *selectedRow;
	return row[*lessonHeader];
}
