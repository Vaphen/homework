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
#include <thread>
#include <boost/filesystem.hpp>

BasicFileOps::BasicFileOps() {

}

BasicFileOps::~BasicFileOps() {

}

bool BasicFileOps::isFileExistant(const std::string &fileName) {
  std::ifstream ifile(fileName);
  return ifile;
}

void BasicFileOps::copyFile(const std::string &from, const std::string &to) {
	std::ifstream inputFileStream(from, std::ios::binary);
	std::ofstream outputFileStream(to, std::ios::binary);
    std::copy(std::istreambuf_iterator<char>(inputFileStream),
        std::istreambuf_iterator<char>( ),
        std::ostreambuf_iterator<char>(outputFileStream));
}

void BasicFileOps::openPdfFile(const std::string &filePath) {
	std::thread ownThread([filePath](){
		ConfigFileParser configFile;
		system(std::string(configFile.getPdfExecutablePath() + " " + filePath).c_str());
	});
	ownThread.detach();
}

void BasicFileOps::openFileManager(const std::string &path) {
	std::thread ownThread([path](){
		ConfigFileParser configFile;
		system(std::string(configFile.getFileManagerPath() + " " + path).c_str());
	});
	ownThread.detach();
}

/// Creates the a folder to a special path
/*
 * @param path the path of the new folder
 * @throws FILE_ERRORS::FOLDER_NOT_CREATABLE if the folder could not be created
 */
void BasicFileOps::createFolder(const std::string& path) {
	boost::filesystem::path dir(path);
	if (!boost::filesystem::create_directory(dir))
	   throw FILE_ERRORS::FOLDER_NOT_CREATABLE;
}

void BasicFileOps::deleteFolder(const std::string &path) {
	boost::filesystem::path dir(path);
	if(!boost::filesystem::remove_all(dir))
		throw FILE_ERRORS::FOLDER_NOT_DELETABLE;
}
