/*
 * StatisticsPage.h
 *
 *  Created on: 06.02.2015
 *      Author: john
 */

#ifndef GUI_STATISTICSPAGE_H_
#define GUI_STATISTICSPAGE_H_

#include <gtkmm.h>
#include "StatisticsLessonTable.h"

class StatisticsPage : public Gtk::Frame {
public:
	StatisticsPage();
	virtual ~StatisticsPage();
private:
	Gtk::Table *pageTable;
	std::vector<StatisticsLessonTable*> statisticTables;
	std::vector<Gtk::VBox*> statisticTablesVBoxes;

	Gtk::Label *nothingAddedYetLabel;

	void initializeWidgets();
	void addNothingAddedYetLabel();
};

#endif /* GUI_STATISTICSPAGE_H_ */
