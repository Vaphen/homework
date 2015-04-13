/*
 * LessonTable.cpp
 *
 *  Created on: 07.01.2015
 *      Author: vaphen
 */

#include "SettingsLessonTable.h"
#include <iostream>

#include "../../sql/SQLiteConnect.h"
#include "../../constants/constants.h"
#include "../../constants/Labels.h"
#include "../../helpers/HelpDialogs.h"

SettingsLessonTable::SettingsLessonTable() :
		lessonHeader(new Gtk::TreeModelColumn<Glib::ustring>),
		allHeader(new Gtk::TreeModel::ColumnRecord) {

	allHeader->add(*lessonHeader);
	set_model(allLessons = Gtk::ListStore::create(*allHeader));
	SQLiteConnect connection;
	lessons = {};
	try {
		lessons = connection.getLessons();
	}catch(ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
		return;
	}

	for(unsigned int i = 0; i < lessons.size(); i++) {
		appendLesson(lessons.at(i));
	}
	append_column(SettingsLessonTableLabels::TABLE_HEADING, *lessonHeader);
	show_all_children();
}

SettingsLessonTable::~SettingsLessonTable() {
	delete allHeader;
	delete lessonHeader;
}

/**
 * appands a given lesson to the TreeView
 */
void SettingsLessonTable::appendLesson(Glib::ustring lessonName) {
	Gtk::TreeModel::Row row = *(allLessons->append());
	row[*lessonHeader] = lessonName;
}

/**
 * delete the selected lesson from the treeView
 */
void SettingsLessonTable::deleteSelectedLesson() {
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
Glib::ustring SettingsLessonTable::getSelectedLesson() {
	Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();
	if(selection->count_selected_rows() == 0)
		return "";
	Gtk::TreeModel::iterator selectedRow = selection->get_selected();
	Gtk::TreeModel::Row row = *selectedRow;
	return row[*lessonHeader];
}

/*std::vector<std::string> LessonTable::getAllLessons() {
	return vecOfAllLessons;
}*/
