/*
 * StatisticsPage.h
 *
 *  Created on: 06.02.2015
 *      Author: john
 */

#ifndef GUI_STATISTICSPAGE_H_
#define GUI_STATISTICSPAGE_H_

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include "StatisticsLessonTable.h"

class StatisticsPage : public Gtk::Frame {
public:
	StatisticsPage();
	virtual ~StatisticsPage();
	void refreshStatisticsTable();
private:
	Gtk::ScrolledWindow *mainScrolledWindow;
	Gtk::Table *pageTable;
	std::vector<boost::shared_ptr<StatisticsLessonTable> > statisticTables;
	std::vector<Gtk::VBox*> statisticTablesVBoxes;

	Gtk::Label *nothingAddedYetLabel;

	void initializeWidgets();
	void addNothingAddedYetLabel();
};

#endif /* GUI_STATISTICSPAGE_H_ */
