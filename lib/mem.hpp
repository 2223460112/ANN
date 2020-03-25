/*
 * mem.hpp
 *
 *  Created on: 20200313
 *      Author: Z2J
 */

#ifndef LIB_MEM_HPP_
#define LIB_MEM_HPP_

#include "Unit.hpp"
#include <vector>

class UnitPool{
public:
	std::vector<std::vector<std::vector<BaseUnit>>> Pool;
	std::vector<std::vector<std::vector<BaseUnit>*>> UDlist;
public:
	uint32_t newNet(){
		Pool.resize(Pool.size()+1);
		UDlist.resize(UDlist.size()+1);
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
	uint32_t UpdateToUDlist(uint32_t Net,uint32_t Layer){
		UDlist[Net].push_back(&Pool[Net][Layer]);
		return UDlist[Net].size()-1;
	}
};


#endif /* LIB_MEM_HPP_ */
