/*  * gui.cpp  *  *  Created on: 25.12.2014  *      Author: vaphen  */

#include "GUI.h"
#include "Exercises/ExerciseFrame.h"
#include "Settings/SettingsPage.h"
#include "Exams/ExamPage.h"
#include "../sql/SQLiteConnect.h"
#include "../constants/constants.h"
#include "../constants/Labels.h"
#include "../helpers/HelpDialogs.h"
#include "../fileOperations/BasicFileOps.h"
#include <gtkmm.h>
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
	#define nullptr 0x00
#endif

GUI::GUI() :
		allLessons(doSqlLessonRequest()),
		vbox(new Gtk::VBox),
		pageBox(new Gtk::EventBox),
		exam_frame(new ExamPage),
		exercise_frame(new ExerciseFrame(allLessons)),
		settings_frame(new SettingsPage(*exercise_frame, *exam_frame)),
		statistics_frame(new StatisticsPage) {

	set_title(WINDOW_TITLE);
	set_default_icon_from_file("./src/logo.png");

	initializeMenueBar();

	if(allLessons.size() == 0) {
		pageBox->add(*settings_frame);
	}else {
		pageBox->add(*exercise_frame);
	}

	pageBox->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	vbox->pack_start(*pageBox);
	add(*vbox);
	show_all();
	maximize();
}

/*
 * initializes all menuebar items plus the logo and adds the menuebar to vbox
 */
void GUI::initializeMenueBar() {
	// initialize widgets
	Gtk::Image *settingsMenueImage = Gtk::manage(new Gtk::Image("./src/settings_button.png"));
	settingsMenueButton = Gtk::manage(new Gtk::EventBox);

	Gtk::Image *statisticsMenueImage = Gtk::manage(new Gtk::Image("./src/statistics_button.png"));
	statisticsMenueButton = Gtk::manage(new Gtk::EventBox);

	Gtk::Image *lessonsMenueImage = Gtk::manage(new Gtk::Image("./src/lessons_button.png"));
	lessonsMenueButton = Gtk::manage(new Gtk::EventBox);

	Gtk::Image *marksMenueImage = Gtk::manage(new Gtk::Image("./src/exam_button.png"));
	marksMenueButton = Gtk::manage(new Gtk::EventBox);

	// set bgiamge of buttons
	lessonsMenueButton->add(*lessonsMenueImage);
	settingsMenueButton->add(*settingsMenueImage);
	statisticsMenueButton->add(*statisticsMenueImage);
	marksMenueButton->add(*marksMenueImage);

	// set size of buttons
	settingsMenueButton->set_size_request(200, 40);
	lessonsMenueButton->set_size_request(200, 40);
	statisticsMenueButton->set_size_request(200, 40);
	marksMenueButton->set_size_request(200, 40);

	/*****************************************************************
	 *  connect buttons to their events (press, hover and leave event)
	 *****************************************************************/
	lessonsMenueButton->signal_button_press_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonClicked),
			MENUE_ENTRIES::LESSONS));
	lessonsMenueButton->signal_enter_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonHover),
			MENUE_ENTRIES::LESSONS));
	lessonsMenueButton->signal_leave_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonLeave),
			MENUE_ENTRIES::LESSONS));

	settingsMenueButton->signal_button_press_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonClicked),
				MENUE_ENTRIES::SETTINGS));
	settingsMenueButton->signal_enter_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonHover),
				MENUE_ENTRIES::SETTINGS));
	settingsMenueButton->signal_leave_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonLeave),
				MENUE_ENTRIES::SETTINGS));

	statisticsMenueButton->signal_button_press_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonClicked),
				MENUE_ENTRIES::STATISTICS));
	statisticsMenueButton->signal_enter_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonHover),
					MENUE_ENTRIES::STATISTICS));
	statisticsMenueButton->signal_leave_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonLeave),
					MENUE_ENTRIES::STATISTICS));

	marksMenueButton->signal_button_press_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonClicked),
					MENUE_ENTRIES::EXAMS));
	marksMenueButton->signal_enter_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonHover),
						MENUE_ENTRIES::EXAMS));
	marksMenueButton->signal_leave_notify_event().connect(sigc::bind<MENUE_ENTRIES>(sigc::mem_fun(*this, &GUI::menueButtonLeave),
						MENUE_ENTRIES::EXAMS));

	// add all buttons to menueBox
	Gtk::HBox *menueBox = Gtk::manage(new Gtk::HBox);
	Gtk::EventBox *menueSeparatorBox = Gtk::manage(new Gtk::EventBox);
	Gtk::Image *logo = Gtk::manage(new Gtk::Image("./src/logo.png"));

	menueSeparatorBox->set_size_request(7, 50);
	menueSeparatorBox->modify_bg(Gtk::STATE_NORMAL, secundaryColor);

	menueBox->pack_start(*lessonsMenueButton, Gtk::PACK_SHRINK, 0);
	menueBox->pack_start(*settingsMenueButton, Gtk::PACK_SHRINK, 0);
	menueBox->pack_start(*statisticsMenueButton, Gtk::PACK_SHRINK, 0);
	menueBox->pack_start(*marksMenueButton, Gtk::PACK_SHRINK, 0);
	menueBox->pack_start(*menueSeparatorBox, Gtk::PACK_SHRINK, 0);
	menueBox->pack_start(*logo, Gtk::PACK_EXPAND_PADDING, 0);



	// add menuebox to the eventbox for backgroundcolor and add eventbox to vbox
	Gtk::EventBox *menueBoxContainer = Gtk::manage(new Gtk::EventBox);
	menueBoxContainer->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	lessonsMenueButton->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	settingsMenueButton->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	statisticsMenueButton->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	marksMenueButton->modify_bg(Gtk::STATE_NORMAL, primaryColor);
	menueBoxContainer->add(*menueBox);
	vbox->pack_start(*menueBoxContainer, Gtk::PACK_SHRINK);

	Gtk::EventBox *separatorBox = Gtk::manage(new Gtk::EventBox);
	separatorBox->modify_bg(Gtk::STATE_NORMAL, secundaryColor);
	separatorBox->set_size_request(-1, 6);
	vbox->pack_start(*separatorBox, Gtk::PACK_SHRINK, 0);
}

/*
 * makes SQL-Request to get saved lessons.
 * Shows an errordialog if an error occures and exits the program
 */
std::vector<std::string> GUI::doSqlLessonRequest() {
	std::vector<std::string> lessons;
	try {
		lessons = connection.getLessons();
	} catch (ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
		exit(0);
	}
	return lessons;
}

/// Triggered when clicked a menue button
bool GUI::menueButtonClicked(GdkEventButton *event, MENUE_ENTRIES &entry) {
	pageBox->remove();
	switch(entry) {
		case MENUE_ENTRIES::LESSONS:
			pageBox->add(*exercise_frame);
			break;
		case MENUE_ENTRIES::SETTINGS:
			pageBox->add(*settings_frame);
			break;
		case MENUE_ENTRIES::STATISTICS:
			pageBox->add(*statistics_frame);
			statistics_frame->refreshStatisticsTable();
			break;
		case MENUE_ENTRIES::EXAMS:
			pageBox->add(*exam_frame);
			break;
		default:
			// should never be reached
			pageBox->add(*exercise_frame);
			break;
	}
	pageBox->show_all();
}

/// Triggered when hovered the lessonMenueButton
bool GUI::menueButtonHover(GdkEventCrossing *event, MENUE_ENTRIES &curEntrie) {
	Gtk::EventBox *selectedEventBox = nullptr;
	Gtk::Image *hoverButton = nullptr;
	switch(curEntrie) {
		case MENUE_ENTRIES::LESSONS:
			hoverButton = Gtk::manage(new Gtk::Image("./src/lessons_button_hover.png"));
			selectedEventBox = lessonsMenueButton;
			break;
		case MENUE_ENTRIES::SETTINGS:
			hoverButton = Gtk::manage(new Gtk::Image("./src/settings_button_hover.png"));
			selectedEventBox = settingsMenueButton;
			break;
		case MENUE_ENTRIES::STATISTICS:
			hoverButton = Gtk::manage(new Gtk::Image("./src/statistics_button_hover.png"));
			selectedEventBox = statisticsMenueButton;
			break;
		case MENUE_ENTRIES::EXAMS:
			hoverButton = Gtk::manage(new Gtk::Image("./src/exam_button_hover.png"));
			selectedEventBox = marksMenueButton;
			break;
	}
	selectedEventBox->remove();
	selectedEventBox->add(*hoverButton);
	selectedEventBox->show_all();
}

/// Triggered when leaved the lessonMenueButton
bool GUI::menueButtonLeave(GdkEventCrossing *event, MENUE_ENTRIES &curEntrie) {
	Gtk::Image *leaveButton = nullptr;
	Gtk::EventBox *selectedEventBox = nullptr;
	switch(curEntrie) {
		case MENUE_ENTRIES::LESSONS:
			leaveButton = Gtk::manage(new Gtk::Image("./src/lessons_button.png"));
			selectedEventBox = lessonsMenueButton;
			break;
		case MENUE_ENTRIES::SETTINGS:
			leaveButton = Gtk::manage(new Gtk::Image("./src/settings_button.png"));
			selectedEventBox = settingsMenueButton;
			break;
		case MENUE_ENTRIES::STATISTICS:
			leaveButton = Gtk::manage(new Gtk::Image("./src/statistics_button.png"));
			selectedEventBox = statisticsMenueButton;
			break;
		case MENUE_ENTRIES::EXAMS:
			leaveButton = Gtk::manage(new Gtk::Image("./src/exam_button.png"));
			selectedEventBox = marksMenueButton;
			break;
	}
	selectedEventBox->remove();
	selectedEventBox->add(*leaveButton);
	selectedEventBox->show_all();
}

/**
 * delete all widget-pointer
 */
GUI::~GUI() {
	delete vbox;
	delete pageBox;
	delete exercise_frame;
	delete settings_frame;
	delete statistics_frame;
	delete exam_frame;
}
