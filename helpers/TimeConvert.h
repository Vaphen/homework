/*
 * TimeConvert.h
 *
 *  Created on: 25.01.2015
 *      Author: john
 */

#ifndef HELPERS_TIMECONVERT_H_
#define HELPERS_TIMECONVERT_H_

#include <string>
#include <sstream>
#include <iomanip>

class TimeConvert {
public:
	TimeConvert(double, double, double);
	TimeConvert();
	virtual ~TimeConvert();
	int getDaysInMonth(int const&, int const&);

	unsigned int getCurYear();
	unsigned int getCurMonth();
	unsigned int getCurDay();
	std::string unixToGermanDateFormat(std::string&);
	std::string getGermanDateFormat();
	std::string getEnglishDateFormat();
	unsigned int getUnixTimeFormat();
private:
	time_t theTime;
	struct tm *aTime;
	std::stringstream stringDateFormat;

	int day;
	int month;
	int year;

	void initializeTimeStructures();
};

#endif /* HELPERS_TIMECONVERT_H_ */
