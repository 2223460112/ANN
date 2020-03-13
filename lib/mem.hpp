/*
 * mem.hpp
 *
 *  Created on: 2020.3.13
 *      Author: z2j
 */

#ifndef LIB_MEM_HPP_
#define LIB_MEM_HPP_

#include "Unit.hpp"
#include <vector>

class UnitPool{
public:
	std::vector<std::vector<std::vector<BaseUnit*>>> Pool;
public:
	uint32_t newNet(){
		Pool.resize(Pool.size()+1);
		return Pool.size()-1;
	}
	uint32_t newLayer(uint32_t Net){
		Pool[Net].resize(Pool[Net].size()+1);
		return Pool[Net].size()-1;
	}
	uint32_t newUnit(uint32_t Net,uint32_t Layer){
		Pool[Net][Layer].resize(Pool[Net][Layer].size()+1);
		return Pool[Net][Layer].size()-1;
	}
};


#endif /* LIB_MEM_HPP_ */
