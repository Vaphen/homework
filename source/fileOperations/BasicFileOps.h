/*
 * BasicFileOps.h
 *
 *  Created on: 25.01.2015
 *      Author: vaphen
 */

#ifndef FILEOPERATIONS_BASICFILEOPS_H_
#define FILEOPERATIONS_BASICFILEOPS_H_

#include "ConfigFileParser.h"
#include <string>

class BasicFileOps {
public:
	BasicFileOps() = default;
	~BasicFileOps() = default;
	bool isFileExistant(const std::string&);
	void copyFile(const std::string&, const std::string &);
	void openPdfFile(const std::string&);
	void openFileManager(const std::string&);
	void createFolder(const std::string&);
	void deleteFolder(const std::string&);
	ConfigFileParser callConfigParser() const;
private:
	const ConfigFileParser configParser;
};

#endif /* FILEOPERATIONS_BASICFILEOPS_H_ */
