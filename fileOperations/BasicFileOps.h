/*
 * BasicFileOps.h
 *
 *  Created on: 25.01.2015
 *      Author: vaphen
 */

#ifndef FILEOPERATIONS_BASICFILEOPS_H_
#define FILEOPERATIONS_BASICFILEOPS_H_

#include <string>

class BasicFileOps {
public:
	BasicFileOps();
	virtual ~BasicFileOps();
	void initializeDir();
	std::string showFileChooser();

};

#endif /* FILEOPERATIONS_BASICFILEOPS_H_ */
