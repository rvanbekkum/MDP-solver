/*
 * FileUtility.cpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#include <string>
#include <fstream>
#include <math.h>
#include "FileUtility.hpp"

/**
 * Checks if the file with the passed name exists.
 *
 * @param fileName : name of the file
 *
 * @return bool indicating whether the file exists
 */
bool file_exists(const std::string& fileName) {
	std::ofstream file(fileName.c_str());
	return file.good();
}

/**
 * Removes the file extension from the passed file name
 * and returns the raw file name.
 *
 * @param fullFileName : the full file name with extension
 *
 * @return raw file name without extension
 */
std::string remove_extension(const std::string fullFileName) {
	int lastindex = fullFileName.find_last_of(".");
	std::string rawFileName = fullFileName.substr(0, lastindex);
	return rawFileName;
}

std::string trimFilePathToName(const std::string path) {
	return path.substr(path.find_last_of("\\/") + 1, path.length() - 1);
}

int fractional_part_as_int(double number, int number_of_decimal_places) {
    double dummy;
    double frac = modf(number,&dummy);
    return round(frac*pow(10,number_of_decimal_places));
}
