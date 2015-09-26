/*
 * PolicyVector.cpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#include "PolicyVector.hpp"

PolicyVector::PolicyVector(std::vector<Index> vect) {
	policyVector = vect;
}

PolicyVector::~PolicyVector() {
}

Index PolicyVector::get(Index state_no) {
	return policyVector[state_no];
}
