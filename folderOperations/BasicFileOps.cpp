/*
 * BasicFileOps.cpp
 *
 *  Created on: 25.01.2015
 *      Author: vaphen
 */

#include "BasicFileOps.h"
#include "constants.h"
#include <sys/stat.h>

BasicFileOps::BasicFileOps() {
	mkdir(FOLD_PATH);
}

BasicFileOps::~BasicFileOps() {

}

void BasicFileOps::initializeDir() {

}

