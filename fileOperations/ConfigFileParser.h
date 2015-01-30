/*
 * configFileParser.h
 *
 *  Created on: 29.01.2015
 *      Author: john
 */

#ifndef FILEOPERATIONS_CONFIGFILEPARSER_H_
#define FILEOPERATIONS_CONFIGFILEPARSER_H_

#include "../constants/constants.h"
#include <fstream>
#include <map>

class ConfigFileParser {
public:
	ConfigFileParser();
	virtual ~ConfigFileParser();
	const std::string getSaveDirectoryPath() const;
private:
	std::map<int, std::string> configurations = {
			{ConfigParams::SAVE_DIRECTORY_PATH, ""}
	};
	std::string pathToConfigFile;
	std::ofstream writeConfigFile;
	std::ifstream readConfigFile;

	void createConfigFile();
	void loadConfigs();
};

#endif /* FILEOPERATIONS_CONFIGFILEPARSER_H_ */
