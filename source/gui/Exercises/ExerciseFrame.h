/*
 * ExerciseNotebook.h
 *
 *  Created on: 06.04.2015
 *      Author: vaphen
 */

#ifndef SOURCE_GUI_EXERCISES_EXERCISEFRAME_H_
#define SOURCE_GUI_EXERCISES_EXERCISEFRAME_H_

#include "ExercisePage.h"
#include <gtkmm.h>

class ExerciseFrame : public Gtk::Frame {
public:
	ExerciseFrame(std::vector<std::string>&);
	virtual ~ExerciseFrame();
	void addLessonPage(std::string&);
	void removeLessonPage(Glib::ustring&);
private:
	std::vector<ExercisePage*> allPages;
	Gtk::Notebook *notebook;

	void initializeWidgets();
};

#endif /* SOURCE_GUI_EXERCISES_EXERCISEFRAME_H_ */
