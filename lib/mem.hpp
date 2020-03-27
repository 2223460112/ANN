/*
 * mem.hpp
 *
 *  Created on: 20200313
 *      Author: Z2J
 */

#ifndef LIB_MEM_HPP_
#define LIB_MEM_HPP_

#include <cstdio>
#include <vector>
#include <Eigen/Eigen>

using namespace Eigen;

class BaseUnit;

class UnitPool {
public:
	std::vector<std::vector<BaseUnit*>> Pool;
	std::vector<std::vector<std::pair<uint32_t, uint32_t>>> Dlist;
	std::vector<uint32_t> Dindex;
	std::vector<MatrixXd> Mats;
public:
	uint32_t newLayer() {
		Pool.resize(Pool.size() + 1);
		return Pool.size() - 1;
	}
	uint32_t newUnit(uint32_t Layer) {
		Pool[Layer].resize(Pool[Layer].size() + 1);
		return Pool[Layer].size() - 1;
	}
	uint32_t UpdateToDlist(uint32_t Layer) {
		Dlist.resize(Dlist.size() + 1);
		Dlist[Dlist.size() - 1].resize(Pool[Layer].size());
		for (uint32_t i = 0; i < Pool[Layer].size(); i++) {
			Dlist[Dlist.size() - 1][i].first = Layer;
			Dlist[Dlist.size() - 1][i].second = i;
		}
		Dindex.push_back(Layer);
		return Dlist.size() - 1;
	}
	BaseUnit& AtDlist(uint32_t Pos, uint32_t iPos) {
		return (*Pool[Dlist[Pos][iPos].first][Dlist[Pos][iPos].second]);
	}
public:
	~UnitPool();
};

#endif /* LIB_MEM_HPP_ */
