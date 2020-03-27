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
	std::vector<BaseLayer*> Layers;
	double Learnrate;
public:
	Net(uint32_t _size, uint32_t Layerstag[], uint32_t Msize, uint32_t LitM[],
			uint32_t LotM[], std::pair<uint32_t, uint32_t> Mrect[],
			StepFunc::func step, LossFunc::func loss, double learnrate) {
		Pool.Mats.resize(Msize);
		for (uint32_t i = 0; i < Msize; i++)
			Pool.Mats[i].setZero(Mrect[i].first, Mrect[i].second);

		Learnrate = learnrate;
		Layers.resize(_size);
		for (uint32_t i = 0; i < _size; i++) {
			if (Layerstag[i] == __DNN_OUTPUT__) {
				Layers[i] = new DNNOutputLayer(&Pool, LitM[i], LotM[i], step,
						loss);
			} else if (Layerstag[i] == __DNN_INNER__) {
				Layers[i] = new DNNInnerLayer(&Pool, LitM[i], LotM[i], step);
			}
		}
		for (uint32_t i = 0; i < _size - 1; i++)
			Layers[i]->FIXDlist(Layers[i + 1]->Units);
	}
	~Net(){
		for(uint32_t i=0;i<Layers.size();i++)delete Layers[i];
	}
public:
	void save(FILE *file) {
		uint32_t intbuff;
		//double doublebuff;

		intbuff = Pool.Pool.size();
		fiputc(intbuff, file)
		for (uint32_t i = 0; i < Pool.Pool.size(); i++) {
			intbuff = Pool.Pool[i].size();
			fiputc(intbuff, file)
			for (uint32_t j = 0; j < Pool.Pool[i].size(); j++) {
				intbuff = Pool.Pool[i][j]->TYPE();
				fbputc(intbuff, file)
				Pool.Pool[i][j]->save(file);
			}
		}

		intbuff = Pool.Dlist.size();
		fiputc(intbuff, file)
		for (uint32_t i = 0; i < Pool.Dlist.size(); i++) {
			fiputc(Pool.Dindex[i], file)
			if (Pool.Dindex[i] != (uint32_t) -1)
				continue;
			intbuff = Pool.Dlist[i].size();
			fiputc(intbuff, file)
			for (uint32_t j = 0; j < Pool.Dlist[i].size(); j++) {
				intbuff = Pool.Dlist[i][j].first;
				fiputc(intbuff, file)
				intbuff = Pool.Dlist[i][j].second;
				fiputc(intbuff, file)
			}
		}

		intbuff = Pool.Mats.size();
		fiputc(intbuff, file)
		for (uint32_t i = 0; i < Pool.Mats.size(); i++) {
			intbuff = Pool.Mats[i].rows();
			fiputc(intbuff, file)
			intbuff = Pool.Mats[i].cols();
			fiputc(intbuff, file)
		}

		fiputc(Learnrate, file)

		for (uint32_t i = 0; i < Layers.size(); i++) {
			intbuff = Layers[i]->TYPE();
			fiputc(intbuff, file)
			Layers[i]->save(file);
		}
	}
public:
	MatrixXd calc(MatrixXd Input) {
		Pool.Mats[0] = Input;
		for (uint32_t i = 0; i < Layers.size(); i++)
			Layers[i]->calc();
		return Pool.Mats[Pool.Mats.size() - 1];
	}
	void train(MatrixXd TargetV) {
		Layers[Layers.size() - 1]->train(TargetV);
		for (uint32_t i = 1; i < Layers.size(); i++)
			Layers[Layers.size() - i - 1]->train();
	}
	void calcdW() {
		for (uint32_t i = 1; i <= Layers.size(); i++) {
			Layers[Layers.size() - i]->calcdW(Learnrate);
		}
	}
	void modify() {
		for (uint32_t i = 0; i < Layers.size(); i++)
			Layers[i]->modify();
	}
}
;

#endif
