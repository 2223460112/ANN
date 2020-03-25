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

static UnitPool Pool;



class Net {
protected:
	uint32_t NetID;
	std::vector<BaseLayer> *Layers;
public:
	Net(uint32_t size, uint32_t Layers[], std::pair<uint32_t, uint32_t> rect[],
			uint32_t LtM[], uint32_t Msize, MatrixXd Mats[]) {
		NetID=Pool.newNet();
		for(int i=0;i<size;i++){
			uint32_t tl=Pool.newLayer(NetID);
			if(Layers[i]==__DNN_OUTPUT__){
				DNNOutputLayer tmp();
			}
		}
	}
};

#endif
