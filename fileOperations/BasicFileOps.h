/*
 * BasicFileOps.h
 *
 *  Created on: 25.01.2015
 *      Author: vaphen
 */

#ifndef FILEOPERATIONS_BASICFILEOPS_H_
#define FILEOPERATIONS_BASICFILEOPS_H_

#include <string>

/**
 * TODO: isnt it better to get ConfigFileParams via inheritance
 */
class BasicFileOps {
public:
	BasicFileOps();
	virtual ~BasicFileOps();
	bool isFileExistant(const std::string&);
	void copyFile(const std::string&, const std::string &);
	void openPdfFile(const std::string&);
	void openFileManager(const std::string&);
	void createFolder(const std::string&);
	void deleteFolder(const std::string&);
};

#endif /* FILEOPERATIONS_BASICFILEOPS_H_ */
