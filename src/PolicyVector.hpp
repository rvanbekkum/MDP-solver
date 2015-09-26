/*
 * PolicyVector.hpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#ifndef SRC_POLICYVECTOR_HPP_
#define SRC_POLICYVECTOR_HPP_

#include <vector>
#include "Globals.h"

class PolicyVector {
private:
	std::vector<Index> policyVector;
public:
	PolicyVector(std::vector<Index>);
	virtual ~PolicyVector();
	Index get(Index state_no);
};

#endif /* SRC_POLICYVECTOR_HPP_ */
