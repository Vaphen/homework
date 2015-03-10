/*
 * configFileParser.cpp
 *
 *  Created on: 29.01.2015
 *      Author: john
 */

#include "ConfigFileParser.h"
#include "../constants/constants.h"
#include <iostream>
#include <string>
#include <map>

#if defined(_WIN32) || defined(WIN32)
#include "../requiredDependencies/rude/config.h"
#else
#include <rude/config.h>
#endif


ConfigFileParser::ConfigFileParser() :
		pathToConfigFile("./" + CONFIG_FILE) {
	rude::Config config;
	config.load(pathToConfigFile.c_str());
	config.setSection("EnvironmentVariables");
	saveFolderDirPath = config.getStringValue("SAVE_FOLDER_DIR_PATH");
	pdfExecutablePath = config.getStringValue("PDF_EXECUTABLE_PATH");
	fileManagerPath = config.getStringValue("FILE_MANAGER_PATH");
}

ConfigFileParser::~ConfigFileParser() { }

void ConfigFileParser::createDefaultConfigFile() {
	rude::Config config;
	config.load(pathToConfigFile.c_str());
	config.setSection("EnvironmentVariables");
	config.setStringValue("SAVE_FOLDER_DIR_PATH", "./uni");
	config.setStringValue("PDF_EXECUTABLE_PATH", "xdg-open");
	config.setStringValue("FILE_MANAGER_PATH", "xdg-open");
	config.save();
}

void ConfigFileParser::setSaveDirectoryPath(const std::string &path) {
	rude::Config config;
	config.load(pathToConfigFile.c_str());
	config.setSection("EnvironmentVariables");
	config.setStringValue("SAVE_FOLDER_DIR_PATH", path.c_str());
	config.save();
}

const std::string ConfigFileParser::getSaveDirectoryPath() const {
	return saveFolderDirPath;
}

void ConfigFileParser::setPdfExecutablePath(const std::string &path) {
	rude::Config config;
	config.load(pathToConfigFile.c_str());
	config.setSection("EnvironmentVariables");
	config.setStringValue("PDF_EXECUTABLE_PATH", path.c_str());
	config.save();
}

const std::string ConfigFileParser::getPdfExecutablePath() const {
	return pdfExecutablePath;
}

void ConfigFileParser::setFileManagerPath(const std::string &path) {
	rude::Config config;
	config.load(pathToConfigFile.c_str());
	config.setSection("EnvironmentVariables");
	config.setStringValue("FILE_MANAGER_PATH", path.c_str());
	config.save();
}

const std::string ConfigFileParser::getFileManagerPath() const {
	return fileManagerPath;
}
