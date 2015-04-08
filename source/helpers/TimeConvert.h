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
	TimeConvert();
	virtual ~TimeConvert();
	int getDaysInMonth(int const&, int const&);

	unsigned int getCurYear();
	unsigned int getCurMonth();
	unsigned int getCurDay();
	std::string unixToGermanDateFormat(std::string&);
	std::string getGermanDateFormat(unsigned int, unsigned int, unsigned int);
	std::string getEnglishDateFormat(unsigned int, unsigned int, unsigned int);

	unsigned int getDayOfUnixTimestamp(std::string);
	unsigned int getMonthOfUnixTimestamp(std::string);
	unsigned int getYearOfUnixTimestamp(std::string);


	unsigned int getUnixTimeFormat(unsigned int, unsigned int, unsigned int);
private:
	time_t theTime;
	struct tm *aTime;
	std::stringstream stringDateFormat;

	void initializeTimeStructures();
};

#endif /* HELPERS_TIMECONVERT_H_ */
