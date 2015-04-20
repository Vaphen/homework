/*
 * configFileParser.cpp
 *
 *  Created on: 29.01.2015
 *      Author: john
 */

#include "ConfigFileParser.h"
#include "../constants/constants.h"
#include <string>
#include <map>

#if defined(_WIN32) || defined(WIN32)
#include "../requiredDependencies/rude/config.h"
static const std::string PDF_DEFAULT_EXECUTABLE = "No Executable found.";
static const Glib::ustring FILE_MANAGER_DEFAULT = "%windir%\\explorer.exe";
#else
static const std::string PDF_DEFAULT_EXECUTABLE = "xdg-open";
static const Glib::ustring FILE_MANAGER_DEFAULT = "xdg-open";
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
	config.setStringValue("PDF_EXECUTABLE_PATH", PDF_DEFAULT_EXECUTABLE.c_str());
	config.setStringValue("FILE_MANAGER_PATH", FILE_MANAGER_DEFAULT.c_str());
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
