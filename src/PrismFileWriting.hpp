/*
 * PrismFileWriting.hpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#ifndef SRC_PRISMFILEWRITING_HPP_
#define SRC_PRISMFILEWRITING_HPP_

#include "DecPOMDPDiscrete.h"
#include "PolicyVector.hpp"

void writePrismFile(std::string filePath, DecPOMDPDiscreteInterface* mdp, PolicyVector policy);

std::string getPrismFilePath(std::string problemFilePath, double discount, double horizon);

#endif /* SRC_PRISMFILEWRITING_HPP_ */
