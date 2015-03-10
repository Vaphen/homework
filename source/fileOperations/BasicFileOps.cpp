/*
 * BasicFileOps.cpp
 *
 *  Created on: 25.01.2015
 *      Author: vaphen
 */

#include "BasicFileOps.h"
#include "ConfigFileParser.h"
#include "../constants/constants.h"
#include <gtkmm.h>
#include <string>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>


/// Returns true if the given file exists
/**
 * @param fileName the path to the file that should be checked
 */
bool BasicFileOps::isFileExistant(const std::string &fileName) {
  std::ifstream ifile(fileName);
  return ifile;
}

/// Copies a given file via ios::binary
/**
 * @param from the path where the 'from'-file is located
 * @param to the path where the copied file should be placed
 */
void BasicFileOps::copyFile(const std::string &from, const std::string &to) {
	if(!isFileExistant(from))
		throw FILE_ERRORS::FILE_DOESNT_EXIST;
	std::ifstream inputFileStream(from, std::ios::binary);
	std::ofstream outputFileStream(to, std::ios::binary);
    std::copy(std::istreambuf_iterator<char>(inputFileStream),
        std::istreambuf_iterator<char>( ),
        std::ostreambuf_iterator<char>(outputFileStream));
}


/// Opens a PDF-File with the programm that is set in preferences
/**
 * @param filePath the path to the PDF-file
 */
void BasicFileOps::openPdfFile(const std::string &filePath) {
	boost::thread ownThread([filePath](){
		ConfigFileParser configParser;
		system(std::string(configParser.getPdfExecutablePath() + " " + filePath).c_str());
	});
	ownThread.detach();
}

/// Opens a specific folder with the file manager that is set in preferences
/**
 * @param path the path to the folder that should be opened
 */
void BasicFileOps::openFileManager(const std::string &path) {
	boost::thread ownThread([path](){
		ConfigFileParser configParser;
		system(std::string(configParser.getFileManagerPath() + " " + path).c_str());
	});
	ownThread.detach();
}

/// Creates the a folder to a special path
/*
 * @param path the path of the new folder
 * @throws FILE_ERRORS if the folder could not be created or is already existant
 */
void BasicFileOps::createFolder(const std::string& path) {
	boost::filesystem::path dir(path);
	if(boost::filesystem::is_directory(path))
		throw FILE_ERRORS::FOLDER_ALREADY_EXISTANT;
	try {
		boost::filesystem::create_directory(dir);
	}catch(const boost::filesystem::filesystem_error &error) {
		throw FILE_ERRORS::FOLDER_NOT_CREATABLE;
	}
}

/// Deletes the folder from the given path
/**
 * @param path the path of the folder that should be delted
 * @throws FILE_ERRORS if file was not deletable
 */
void BasicFileOps::deleteFolder(const std::string &path) {
	boost::filesystem::path dir(path);
	try {
		boost::filesystem::remove_all(dir);
	}catch(const boost::filesystem::filesystem_error &error) {
		throw FILE_ERRORS::FOLDER_NOT_DELETABLE;
	}
}

ConfigFileParser BasicFileOps::callConfigParser() const {
	return configParser;
}
