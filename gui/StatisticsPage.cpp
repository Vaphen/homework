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

	initializeWidgets();

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
		add(*nothingAddedYetLabel);
		return;
	}


	for(unsigned int i = 0; i < lessons.size(); i++) {
		Gtk::VBox *lessonBox = Gtk::manage(new Gtk::VBox);
		Gtk::Label *lessonHeadingLabel = Gtk::manage(new Gtk::Label(lessons.at(i)));
		statisticTables.push_back(new StatisticsLessonTable(lessons.at(i)));
		lessonBox->pack_start(*lessonHeadingLabel, Gtk::PACK_SHRINK, 10);
		lessonBox->pack_start(*(statisticTables.back()), Gtk::PACK_SHRINK, 10);
		pageTable->attach(*lessonBox, i % 3, i % 3 + 1, std::floor(i / 3), std::floor(i / 3) + 1, Gtk::EXPAND, Gtk::FILL, 0, 20);
	}


	add(*pageTable);

}

StatisticsPage::~StatisticsPage() {
	for(StatisticsLessonTable* curTable : statisticTables) {
		delete curTable;
	}
}

void StatisticsPage::initializeWidgets() {
	pageTable = Gtk::manage(new Gtk::Table);
	nothingAddedYetLabel = Gtk::manage(new Gtk::Label("Es wurden noch keine Fächer hinzugefügt."));
}

void StatisticsPage::addNothingAddedYetLabel() {
	// TODO: not needed right now; check if it is needed
}
