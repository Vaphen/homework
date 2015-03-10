/*
 * StatisticsLessonTable.h
 *
 *  Created on: 06.02.2015
 *      Author: john
 */

#ifndef GUI_STATISTICSLESSONTABLE_H_
#define GUI_STATISTICSLESSONTABLE_H_

#include <gtkmm.h>

class StatisticsLessonTable : public Gtk::Table{
public:
	StatisticsLessonTable(std::string&);
	virtual ~StatisticsLessonTable();
private:
	Gtk::Label *exerciseUntilLabel;
	Gtk::Label *pointsReachedLabel;
	Gtk::Label *pointsTotalLabel;

	std::string curLesson;

	unsigned int totalPointsTotal;
	unsigned int reachedPointsTotal;

	void initializeWidgets();
	void appendRow(std::string, std::string, std::string);
	void appendTotal();
};

#endif /* GUI_STATISTICSLESSONTABLE_H_ */
