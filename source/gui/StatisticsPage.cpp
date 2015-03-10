/*
 * StatisticsPage.cpp
 *
 *  Created on: 06.02.2015
 *      Author: john
 */

#include "StatisticsPage.h"
#include "StatisticsLessonTable.h"
#include "../sql/SQLiteConnect.h"
#include "../constants/constants.h"
#include "../helpers/HelpDialogs.h"

StatisticsPage::StatisticsPage() {
	set_label("Statistiken");

	refreshStatisticsTable();
}

StatisticsPage::~StatisticsPage() {}

void StatisticsPage::addNothingAddedYetLabel() {
	// TODO: not needed right now; check if it is needed
}

void StatisticsPage::refreshStatisticsTable() {
	// clear the frame if something is in
	remove();

	statisticTables.clear();
	// initialize the table that the old one is deleted
	pageTable = Gtk::manage(new Gtk::Table);
	mainScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow);

	mainScrolledWindow->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

	std::vector<std::string> lessons = {};
	try {
		SQLiteConnect connection;
		lessons = connection.getLessons();
	}catch (ERRORS &error) {
		HelpDialogs::showErrorDialog(error);
		return;
	}

	// there is no lesson
	if(lessons.size() == 0) {
		nothingAddedYetLabel = Gtk::manage(new Gtk::Label("Es wurden noch keine Fächer hinzugefügt."));
		pageTable->attach(*nothingAddedYetLabel, 0, 3, 0, 1, Gtk::EXPAND, Gtk::FILL, 0, 20);
	} else {
		// a first lesson was added
		//pageTable->remove(*nothingAddedYetLabel);
		for(unsigned int i = 0; i < lessons.size(); i++) {
			Gtk::VBox *lessonBox = Gtk::manage(new Gtk::VBox);
			Gtk::Label *lessonHeadingLabel = Gtk::manage(new Gtk::Label(lessons.at(i)));
			Gtk::ScrolledWindow *tableScroller = Gtk::manage(new Gtk::ScrolledWindow);
			boost::shared_ptr<StatisticsLessonTable> newTable(new StatisticsLessonTable(lessons.at(i)));
			statisticTables.push_back(newTable);
			tableScroller->add(*(statisticTables.back()));
			tableScroller->set_size_request(430, 350);
			tableScroller->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
			lessonBox->pack_start(*lessonHeadingLabel, Gtk::PACK_EXPAND_PADDING, 10);
			lessonBox->pack_start(*tableScroller, Gtk::PACK_EXPAND_PADDING, 10);
			pageTable->attach(*lessonBox, i % 3, i % 3 + 1, std::floor(i / 3), std::floor(i / 3) + 1, Gtk::EXPAND, Gtk::FILL, 0, 20);
		}
	}
	mainScrolledWindow->add(*pageTable);
	add(*mainScrolledWindow);
	show_all_children();
}
