/*
 * Layer.hpp
 *
 *  Created on: 20200323
 *      Author: Z2J
 */

#ifndef LIB_NET_HPP_
#define LIB_NET_HPP_

#include "../lib/Layer.hpp"

class Net {
protected:
	UnitPool Pool;
	std::vector<BaseLayer> Layers;
public:
	Net(uint32_t _size, uint32_t Layerstag[], std::pair<uint32_t, uint32_t> rect[],
			uint32_t Msize, uint32_t LtM[],StepFunc::func step,LossFunc::func loss,double learnrate) {
		Layers.resize(_size);
		for(uint32_t i=0;i<_size;i++){
			if(Layerstag[i]==__DNN_OUTPUT__){
				DNNOutputLayer tmp(&Pool,i,i+1,step,loss,learnrate);
				Layers[i]=tmp;
			}else
			if(Layerstag[i]==__DNN_INNER__){
				DNNInnerLayer tmp(&Pool,i,i+1,step,learnrate);
				Layers[i]=tmp;
			}
		}
	}
public:
	void Save(FILE *file){

	}
};

#endif
