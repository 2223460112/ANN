/*
 * Layer.hpp
 *
 *  Created on: 20200323
 *      Author: Z2J
 */

#ifndef LIB_NET_HPP_
#define LIB_NET_HPP_

#include "../lib/mem.hpp"
#include "../lib/Layer.hpp"

class Net {
protected:
	UnitPool Pool;
	std::vector<BaseLayer> *Layers;
public:
	Net(uint32_t size, uint32_t Layers[], std::pair<uint32_t, uint32_t> rect[],
			uint32_t Msize, uint32_t LtM[]) {
	}
};

#endif
