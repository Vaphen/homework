/*
 * StatisticsLessonTable.cpp
 *
 *  Created on: 06.02.2015
 *      Author: john
 */

#include "StatisticsLessonTable.h"
#include "../../constants/Labels.h"
#include "../../constants/constants.h"
#include "../../sql/SQLiteConnect.h"
#include "../../helpers/TimeConvert.h"

#if defined(_WIN32) || defined(WIN32)
	#include "../../helpers/conversions.h"
#endif

// default constructor for special point-table
/**
 * @param curLesson the lesson the points table belongs to
 * @throws ERRORS if an sql-error occures
 */
StatisticsLessonTable::StatisticsLessonTable(std::string curLesson) :
		curLesson(curLesson),
		reachedPointsTotal(0),
		totalPointsTotal(0){
	initializeWidgets();
	attach(*exerciseUntilLabel, 0, 1, 0, 1, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*pointsReachedLabel, 1, 2, 0, 1, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*pointsTotalLabel, 2, 3, 0, 1, Gtk::EXPAND, Gtk::FILL, 10, 0);

	std::vector<std::vector<std::string> > nameAndPointsVec;
	SQLiteConnect connection;
	try {
		nameAndPointsVec = connection.getPoints(curLesson);
	}catch(ERRORS& error) {
		throw error;
	}
	if(nameAndPointsVec.size() == 0) {
		appendRow("---", "0", "0");
		return;
	}
	TimeConvert timeConvert;
	for(unsigned int i = 0; i < nameAndPointsVec.at(0).size(); i++) {
		appendRow(timeConvert.unixToGermanDateFormat(nameAndPointsVec.at(0).at(i)),
				  nameAndPointsVec.at(1).at(i),
				  nameAndPointsVec.at(2).at(i));
		reachedPointsTotal += std::stoi(nameAndPointsVec.at(1).at(i));
		totalPointsTotal += std::stoi(nameAndPointsVec.at(2).at(i));
	}

	appendTotal();

	show_all_children();
	//appendRow();
}

StatisticsLessonTable::~StatisticsLessonTable() { }

void StatisticsLessonTable::initializeWidgets() {
	exerciseUntilLabel = Gtk::manage(new Gtk::Label);
	pointsReachedLabel = Gtk::manage(new Gtk::Label);
	pointsTotalLabel = Gtk::manage(new Gtk::Label);

	exerciseUntilLabel->set_markup(StatisticsLessonTableLabels::UNTIL_HEADING_LABEL);
	pointsReachedLabel->set_markup(StatisticsLessonTableLabels::REACHED_HEADING_LABEL);
	pointsTotalLabel->set_markup(StatisticsLessonTableLabels::TOTAL_HEADING_LABEL);
}

void StatisticsLessonTable::appendRow(std::string until, std::string reachedPoints, std::string totalPoints) {
	unsigned int rows, columns;
	get_size(rows, columns);
	Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label(until));
	Gtk::Label *reachedPointsLabel = Gtk::manage(new Gtk::Label(reachedPoints));
	Gtk::Label *totalPointsLabel = Gtk::manage(new Gtk::Label(totalPoints));
	attach(*untilLabel, 0, 1, rows, rows + 1, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*reachedPointsLabel, 1, 2, rows , rows + 1, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*totalPointsLabel, 2, 3, rows, rows + 1, Gtk::EXPAND, Gtk::FILL, 10, 0);
	show_all_children();
}

void StatisticsLessonTable::appendTotal() {
	Gtk::HSeparator *hSep = Gtk::manage(new Gtk::HSeparator);
	hSep->set_size_request(400, 20);

	unsigned int rows, columns;
		get_size(rows, columns);
	Gtk::Label *untilLabel = Gtk::manage(new Gtk::Label("Punkte zusammen:"));
	Gtk::Label *reachedPointsTotalLabel = Gtk::manage(new Gtk::Label(std::to_string(reachedPointsTotal)));
	Gtk::Label *totalPointsTotalLabel = Gtk::manage(new Gtk::Label(std::to_string(totalPointsTotal)));

	Gtk::HSeparator *hSepTwo = Gtk::manage(new Gtk::HSeparator);
	hSepTwo->set_size_request(400, 20);

	float reachedPointsinPercent = [&] () {
		return (totalPointsTotal == 0) ? 0 : (((float)100 / totalPointsTotal) * reachedPointsTotal);
	}();

	Gtk::Label *reachedLabel = Gtk::manage(new Gtk::Label("Erreicht:"));
	Gtk::Label *percentLabel = Gtk::manage(new Gtk::Label(std::to_string(reachedPointsinPercent).substr(0, 5) + "% / 100%"));

	attach(*hSep, 0, 3, rows, rows + 1, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*untilLabel, 0, 1, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*reachedPointsTotalLabel, 1, 2, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*totalPointsTotalLabel, 2, 3, rows + 1, rows + 2, Gtk::EXPAND, Gtk::FILL, 10, 0);

	attach(*hSepTwo, 0, 3, rows + 2, rows + 3, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*reachedLabel, 0, 1, rows + 3, rows + 4, Gtk::EXPAND, Gtk::FILL, 10, 0);
	attach(*percentLabel, 1, 2, rows + 3, rows + 4, Gtk::EXPAND, Gtk::FILL, 10, 0);

	show_all_children();
}
