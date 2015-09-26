/*
 * FileUtility.hpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#ifndef SRC_FILEUTILITY_HPP_
#define SRC_FILEUTILITY_HPP_

bool file_exists(const std::string& fileName);

std::string remove_extension(const std::string fullFileName);

std::string trimFilePathToName(const std::string path);

int fractional_part_as_int(double number, int number_of_decimal_places);

#endif /* SRC_FILEUTILITY_HPP_ */
