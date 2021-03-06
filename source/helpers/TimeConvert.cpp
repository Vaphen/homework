/*
 * TimeConvert.cpp
 *
 *  Created on: 25.01.2015
 *      Author: john
 */

#include "TimeConvert.h"
#include <ctime>

#if defined(_WIN32) || defined(WIN32)
	#include "conversions.h"
#endif
/// Constructor for time-operations. It takes Arguments of a special day, month and year.
/**
 * @param day day used to convert to special format
 * @param month month used to convert to special format
 * @param year year used to convert to special format
 */

/// Constructor for time-operations. It is used for getting some independent information like current day
TimeConvert::TimeConvert() {
	initializeTimeStructures();
}

TimeConvert::~TimeConvert() {

}

void TimeConvert::initializeTimeStructures() {
	theTime = time(NULL),
	aTime = localtime(&theTime);
}

/// Calculates the amount of days in a special month of a year
/**
 * @param month the month from that the days should be calculated (1-12)
 * @param year the year from that the days should be calculated (Format: YYYY)
 * @returns amount of days in the given month of the given year
 */
int TimeConvert::getDaysInMonth(int const &month, int const &year) {
	int numberOfDays;
	if (month == 4 || month == 6 || month == 9 || month == 11) {
	  numberOfDays = 30;
	} else if (month == 2) {
		bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		numberOfDays = (isLeapYear) ? 29 : 28;
	} else {
	  numberOfDays = 31;
	}
	return numberOfDays;
}

unsigned int TimeConvert::getCurYear() {
	initializeTimeStructures();
	return aTime->tm_year + 1900; // Year is X years since 1900
}

unsigned int TimeConvert::getCurMonth() {
	initializeTimeStructures();
	return aTime->tm_mon + 1; // Month is from 0 to 11
}

unsigned int TimeConvert::getCurDay() {
	initializeTimeStructures();
	return aTime->tm_mday;
}

std::string TimeConvert::getGermanDateFormat(unsigned int day, unsigned int month, unsigned int year) {
	stringDateFormat.str("");
	// this converts the given date-values to a valid german date string (DD.MM.YYYY)
	stringDateFormat << std::setfill('0') << std::setw(2) << day << '.'
			<< std::setfill('0') << std::setw(2) << month << '.' << year;
	return stringDateFormat.str();
}

std::string TimeConvert::unixToGermanDateFormat(std::string &unixTimestamp) {
	char germanDateFormat[11];
	theTime = std::atoi(unixTimestamp.c_str());
	aTime = localtime(&theTime);
	strftime(germanDateFormat, sizeof(germanDateFormat), "%d.%m.%Y", aTime);
	return std::string(germanDateFormat);
}

unsigned int TimeConvert::getDayOfUnixTimestamp(std::string unixTimestamp) {
	// need 3 chars for two characters (e.g. 07)
	char day[3];
	theTime = std::atoi(unixTimestamp.c_str());
	aTime = localtime(&theTime);
	strftime(day, sizeof(day), "%d", aTime);

	std::string sday = day;
	if(sday.at(0) == '0')
		sday = sday.substr(1, 2);
	return std::stoi(sday);
}

unsigned int TimeConvert::getMonthOfUnixTimestamp(std::string unixTimestamp) {
	char month[3];
	theTime = std::atoi(unixTimestamp.c_str());
	aTime = localtime(&theTime);
	strftime(month, sizeof(month), "%m", aTime);

	std::string smonth = month;
	if(smonth.at(0) == '0')
		smonth = smonth.substr(1, 2);
	return std::stoi(smonth);
}

unsigned int TimeConvert::getYearOfUnixTimestamp(std::string unixTimestamp) {
	char year[5];
	theTime = std::atoi(unixTimestamp.c_str());
	aTime = localtime(&theTime);
	strftime(year, sizeof(year), "%Y", aTime);
	std::string syear = year;
	return std::stoi(syear);
}

std::string TimeConvert::getEnglishDateFormat(unsigned int day, unsigned int month, unsigned int year) {
	stringDateFormat.str("");
	// this converts the given date-values to a valid english date string (YYYY-MM-DD)
	stringDateFormat << year << "-" << std::setfill('0') << std::setw(2) <<
			month << "-" << std::setfill('0') << std::setw(2) << day;
	return stringDateFormat.str();
}

unsigned int TimeConvert::getUnixTimeFormat(unsigned int day, unsigned int month, unsigned int year) {
	std::time_t result = std::time(NULL);
	struct tm *tm = localtime(&result);
	tm->tm_year = (year - 1900);
	tm->tm_mon = (month - 1);
	tm->tm_mday = day;
	return mktime(tm);
}
