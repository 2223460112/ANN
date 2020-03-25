/*
 * mem.hpp
 *
 *  Created on: 20200313
 *      Author: Z2J
 */

#ifndef LIB_MEM_HPP_
#define LIB_MEM_HPP_

#include <vector>

#include <Eigen/Eigen>

using namespace Eigen;

class BaseUnit;

class UnitPool{
public:
	std::vector<std::vector<BaseUnit>> Pool;
	std::vector<std::vector<BaseUnit>*> UDlist;
	std::vector<MatrixXd> Mats;
public:
	uint32_t newLayer(){
		Pool.resize(Pool.size()+1);
		return Pool.size()-1;
	}
	uint32_t newUnit(uint32_t Layer){
		Pool[Layer].resize(Pool[Layer].size()+1);
		return Pool[Layer].size()-1;
	}
	uint32_t UpdateToUDlist(uint32_t Layer){
		UDlist.push_back(&Pool[Layer]);
		return UDlist.size()-1;
	}
};


#endif /* LIB_MEM_HPP_ */
