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
	uint32_t *LitM;
	uint32_t *LotM;
public:
	Net(uint32_t _size, uint32_t Layerstag[], uint32_t Msize, uint32_t _LitM[],
			uint32_t _LotM[], std::pair<uint32_t, uint32_t> Mrect[],
			uint32_t stepfuncs[], LossFunc::func loss, double learnrate) {
		Pool.Mats.resize(Msize);
		for (uint32_t i = 0; i < Msize; i++)
			Pool.Mats[i].setZero(Mrect[i].first, Mrect[i].second);

		Learnrate = learnrate;
		Layers.resize(_size);
		for (uint32_t i = 0; i < _size; i++) {
			StepFunc::func step(stepfuncs[i]);
			if (Layerstag[i] == __DNN_OUTPUT__) {
				Layers[i] = new DNNOutputLayer(&Pool, _LitM[i], _LotM[i], step,
						loss);
			} else if (Layerstag[i] == __DNN_INNER__) {
				Layers[i] = new DNNInnerLayer(&Pool, _LitM[i], _LotM[i], step);
			}
		}
		for (uint32_t i = 0; i < _size - 1; i++)
			Layers[i]->FIXDlist(Layers[_LotM[i]]->Units);

		LitM = new uint32_t[_size];
		LotM = new uint32_t[_size];
		for (uint32_t i = 0; i < _size; i++) {
			LotM[i] = _LotM[i];
			LitM[i] = _LitM[i];
		}
	}
	~Net() {
		for (uint32_t i = 0; i < Layers.size(); i++)
			delete Layers[i];
		delete[] LotM;
		delete[] LitM;
	}
public:
	void save(FILE *file) {

		uint32_t intbuff;
		ushort bytebuff;
		//double doublebuff;

		intbuff = Pool.Pool.size();
		fiputc(intbuff, file)
		for (uint32_t i = 0; i < Pool.Pool.size(); i++) {
			intbuff = Pool.Pool[i].size();
			fiputc(intbuff, file)
			for (uint32_t j = 0; j < Pool.Pool[i].size(); j++) {
				bytebuff = Pool.Pool[i][j]->TYPE();
				fbputc(bytebuff, file)
				Pool.Pool[i][j]->save(file);
			}
		}

		intbuff = Pool.Dlist.size();
		fiputc(intbuff, file)
		for (uint32_t i = 0; i < Pool.Dlist.size(); i++) {
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

		//fdputc(Learnrate, file)

		for (uint32_t i = 0; i < Layers.size(); i++) {
			intbuff = Layers[i]->TYPE();
			fiputc(intbuff, file)
			Layers[i]->save(file);
			fiputc(LitM[i], file)
			fiputc(LotM[i], file)
		}
	}
	Net(FILE *file,double _Learnrate) {
		uint32_t intbuff, intbuff2;
		//double doublebuff;
		ushort bytebuff;


		figetc(intbuff, file);
		Pool.Pool.resize(intbuff);
		Layers.resize(intbuff);
		for (uint32_t i = 0; i < Pool.Pool.size(); i++) {
			figetc(intbuff, file)
			Pool.Pool[i].resize(intbuff);
			for (uint32_t j = 0; j < Pool.Pool[i].size(); j++) {
				fbgetc(bytebuff, file)
				if (bytebuff == __DNN_OUTPUT__) {
					Pool.Pool[i][j] = new DNNOutputUnit(file, &Pool);
				} else if (bytebuff == __DNN_INNER__) {
					Pool.Pool[i][j] = new DNNInnerUnit(file, &Pool);
				}
			}
		}

		figetc(intbuff, file)
		Pool.Dlist.resize(intbuff);
		for (uint32_t i = 0; i < Pool.Dlist.size(); i++) {
			figetc(intbuff, file)
			Pool.Dlist[i].resize(intbuff);
			for (uint32_t j = 0; j < Pool.Dlist[i].size(); j++) {
				figetc(Pool.Dlist[i][j].first, file)
				figetc(Pool.Dlist[i][j].second, file)
			}
		}

		figetc(intbuff, file)
		Pool.Mats.resize(intbuff);
		for (uint32_t i = 0; i < Pool.Mats.size(); i++) {
			figetc(intbuff, file)
			figetc(intbuff2, file)
			Pool.Mats[i].setZero(intbuff, intbuff2);
		}

		//fdgetc(Learnrate, file)
		Learnrate=_Learnrate;

		LitM = new uint32_t[Layers.size()];
		LotM = new uint32_t[Layers.size()];

		for (uint32_t i = 0; i < Layers.size(); i++) {
			figetc(intbuff, file)
			if (intbuff == __DNN_OUTPUT__)
				Layers[i] = new DNNOutputLayer(file, &Pool);
			else if (intbuff == __DNN_INNER__)
				Layers[i] = new DNNInnerLayer(file, &Pool);
			figetc(LitM[i], file)
			figetc(LotM[i], file)
		}
		for (uint32_t i = 0; i < Layers.size()-1; i++)
			Layers[i]->FIXDlist(Layers[LotM[i]]->Units);
		for (uint32_t i = 0; i < Layers.size(); i++)
			Layers[i]->FIXload(LitM[i], LotM[i]);
	}
public:
	MatrixXd calc(MatrixXd Input) {
		Pool.Mats[0] = Input;
		for (uint32_t i = 0; i < Layers.size(); i++)
			Layers[i]->calc();
		return Pool.Mats[Pool.Mats.size() - 1];
	}
	void train(MatrixXd TargetV) {
		Layers[Layers.size() - 1]->trainT(TargetV);
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
