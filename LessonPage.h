/*
 * LessonPage.h
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#ifndef LESSONPAGE_H_
#define LESSONPAGE_H_

#include <gtkmm.h>

class LessonPage : public Gtk::Frame {
public:
	LessonPage(std::string);
	virtual ~LessonPage();
private:
	std::string pageTitle;
	Gtk::Table *exerciseTable;
};

#endif /* LESSONPAGE_H_ */
