/*
 * ExerciseNotebook.cpp
 *
 *  Created on: 06.04.2015
 *      Author: vaphen
 */

#include "ExerciseFrame.h"
#include "ExercisePage.h"
#include "../../constants/constants.h"

ExerciseFrame::ExerciseFrame(std::vector<std::string> &allLessons) {

	initializeWidgets();

	// add all pages to notebook. allPages vec will be filled with these pages
	for(unsigned int i = 0; i < allLessons.size(); i++)
		addLessonPage(allLessons.at(i));

	add(*notebook);
	set_size_request(-1, 500);
	show_all();
}

void ExerciseFrame::initializeWidgets() {
	notebook = Gtk::manage(new Gtk::Notebook);
}

void ExerciseFrame::addLessonPage(std::string &newLesson) {
	ExercisePage *newFrame = Gtk::manage(new ExercisePage(newLesson));
	allPages.push_back(newFrame);
	Gtk::EventBox *backgroundBox = Gtk::manage(new Gtk::EventBox);
	backgroundBox->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	backgroundBox->add(*newFrame);
	notebook->append_page(*backgroundBox, newLesson, true);
	notebook->show_all();
}

void ExerciseFrame::removeLessonPage(Glib::ustring &lessonToDelete) {
	/*
	 * there is no way to delete notebook-page by its label.
	 * we need to iterate through all pages to check if it's
	 * the one that should be deleted
	 */
	for(unsigned int i = 0; i < allPages.size(); i++) {
		if(allPages.at(i)->getLessonName() == lessonToDelete) {
			notebook->remove_page(i);
			break;
		}
	}

	notebook->show_all();
}

ExerciseFrame::~ExerciseFrame() {

}

