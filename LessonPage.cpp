/*
 * LessonPage.cpp
 *
 *  Created on: 30.12.2014
 *      Author: vaphen
 */

#include "LessonPage.h"
#include <gtkmm.h>

LessonPage::LessonPage(std::string pageTitle) :
		pageTitle(pageTitle),
		exerciseTable(new Gtk::Table(3, 3)) {
	set_label("Fach: " + pageTitle);
}

LessonPage::~LessonPage() {
	delete exerciseTable;
}

