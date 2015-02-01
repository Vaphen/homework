/*
 * configFileParser.h
 *
 *  Created on: 29.01.2015
 *      Author: john
 */

#ifndef FILEOPERATIONS_CONFIGFILEPARSER_H_
#define FILEOPERATIONS_CONFIGFILEPARSER_H_

#include "../constants/constants.h"
#include <map>
#include <rude/config.h>

class ConfigFileParser {
public:
	ConfigFileParser();
	virtual ~ConfigFileParser();
	const std::string getSaveDirectoryPath() const;
	void setSaveDirectoryPath(const std::string&);
	void createDefaultConfigFile();
	void setPdfExecutablePath(const std::string&);
	const std::string getPdfExecutablePath() const;
	void setFileManagerPath(const std::string&);
	const std::string getFileManagerPath() const;
private:
	std::string pathToConfigFile;

	std::string saveFolderDirPath;
	std::string pdfExecutablePath;
	std::string fileManagerPath;
};

#endif /* FILEOPERATIONS_CONFIGFILEPARSER_H_ */
