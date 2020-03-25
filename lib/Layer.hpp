/*
 * Layer.hpp
 *
 *  Created on: 20200306
 *      Author: Z2J
 */

#ifndef LIB_LAYER_HPP_
#define LIB_LAYER_HPP_

#include "../lib/Unit.hpp"

class BaseLayer {
protected:
	UnitPool *Pool;
	BaseLayer *Down;
	uint32_t Input, Output;
	uint32_t LayerID;
public:
	uint32_t Units;
public:
	BaseLayer() {
	}
	virtual ~BaseLayer() {
	}
	virtual void FIXUDlist(BaseLayer *_Down) {
		Down = _Down;
		for (uint32_t i = 0; i < Pool->Dlist[Units]->size(); i++)
			(*Pool->Dlist[Units])[i].FIXDlist(Down->Units);
	}
public:
	virtual void calc() {
	}
	virtual void train() {
	}
	virtual void train(MatrixXd targetV) {
	}
	virtual void calcdW() {
	}
	virtual void modify() {
	}
};
class DNNOutputLayer: public BaseLayer {
public:
	DNNOutputLayer() {
	}
	DNNOutputLayer(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			StepFunc::func _step, LossFunc::func _loss, double _Learnrate) {
		Pool = _Pool;
		LayerID = Pool->newLayer();
		Input = _Input, Output = _Output;
		Pool->Pool[LayerID].resize(
				Pool->Mats[Output].rows() * Pool->Mats[Output].cols());
		for (uint32_t i = 0, k = 0; i < Pool->Mats[Output].rows(); i++)
			for (uint32_t j = 0; j < Pool->Mats[Output].cols(); j++, k++) {
				DNNOutputUnit tmp(_Pool, _Input, _Output,
						std::pair<uint32_t, uint32_t>(i, j), _step, _loss,
						_Learnrate);
				Pool->Pool[LayerID][Pool->newUnit(LayerID)] = tmp;
			}
		Units = Pool->UpdateToDlist(LayerID);
	}
	~DNNOutputLayer() {
	}
public:
	void calc() {
		for (uint32_t i = 0; i < (*Pool->Dlist[Units]).size(); i++)
			(*Pool->Dlist[Units])[i].calc();
	}
	void train(MatrixXd targetV) {
		for (uint32_t i = 0; i < (*Pool->Dlist[Units]).size(); i++)
			(*Pool->Dlist[Units])[i].train(
					targetV((*Pool->Dlist[Units])[i].OutputPos.first,
							(*Pool->Dlist[Units])[i].OutputPos.second));
	}
	void calcdW() {
		for (uint32_t i = 0; i < (*Pool->Dlist[Units]).size(); i++)
			(*Pool->Dlist[Units])[i].calcdW();
	}
	void modify() {
		for (uint32_t i = 0; i < (*Pool->Dlist[Units]).size(); i++)
			(*Pool->Dlist[Units])[i].modify();
	}
};
class DNNInnerLayer: public DNNOutputLayer {
public:
	DNNInnerLayer(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			StepFunc::func _step, double _Learnrate) {
		Pool = _Pool;
		LayerID = Pool->newLayer();
		Input = _Input, Output = _Output;
		Pool->Pool[LayerID].resize(
				Pool->Mats[Output].rows() * Pool->Mats[Output].cols());
		for (uint32_t i = 0, k = 0; i < Pool->Mats[Output].rows(); i++)
			for (uint32_t j = 0; j < Pool->Mats[Output].cols(); j++, k++) {
				DNNInnerUnit tmp(_Pool, _Input, _Output,
						std::pair<uint32_t, uint32_t>(i, j), _step, _Learnrate);
				Pool->Pool[LayerID][Pool->newUnit(LayerID)] = tmp;
			}
		Units = Pool->UpdateToDlist(LayerID);
	}
	~DNNInnerLayer() {
	}
public:
	void train() {
		for (uint32_t i = 0; i < (*Pool->Dlist[Units]).size(); i++)
			(*Pool->Dlist[Units])[i].train();
	}
};

#endif /* LIB_LAYER_HPP_ */
