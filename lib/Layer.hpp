/*
 * Layer.hpp
 *
 *  Created on: 20200306
 *      Author: Z2J
 */

#ifndef LIB_LAYER_HPP_
#define LIB_LAYER_HPP_

#include "../lib/Unit.hpp"
#include "../lib/mem.hpp"

class BaseLayer {
protected:
	UnitPool *Pool;
	BaseLayer *Up, *Down;
	uint32_t Input, Output;
	uint32_t LayerID;
public:
	uint32_t Units;
public:
	BaseLayer() {
	}
	virtual ~BaseLayer() {
	}
	virtual void FIXUDlist() {
		for (uint32_t i = 0; i < Pool->UDlist[Units]->size(); i++)
			(*Pool->UDlist[Units])[i].FIXUDlist(Up->Units, Down->Units);
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
	DNNOutputLayer(UnitPool *_Pool, uint32_t _Input, BaseLayer *_Up,
			uint32_t _Output, StepFunc::func _step, LossFunc::func _loss,
			double _Learnrate) {
		Pool = _Pool;
		LayerID = Pool->newLayer();
		Input = _Input, Output = _Output;
		Up = _Up;
		Pool->Pool[LayerID].resize(
				Pool->Mats[Output].rows() * Pool->Mats[Output].cols());
		for (uint32_t i = 0, k = 0; i < Pool->Mats[Output].rows(); i++)
			for (uint32_t j = 0; j < Pool->Mats[Output].cols(); j++, k++) {
				DNNOutputUnit tmp(_Pool, _Input, _Up->Units, _Output,
						std::pair<uint32_t, uint32_t>(i, j), _step, _loss,
						_Learnrate);
				Pool->Pool[LayerID][Pool->newUnit(LayerID)] = tmp;
			}
		Units = Pool->UpdateToUDlist(LayerID);
	}
	~DNNOutputLayer() {
	}
public:
	void calc() {
		for (uint32_t i = 0; i < (*Pool->UDlist[Units]).size(); i++)
			(*Pool->UDlist[Units])[i].calc();
	}
	void train(MatrixXd targetV) {
		for (uint32_t i = 0; i < (*Pool->UDlist[Units]).size(); i++)
			(*Pool->UDlist[Units])[i].train(
					targetV((*Pool->UDlist[Units])[i].OutputPos.first,
							(*Pool->UDlist[Units])[i].OutputPos.second));
	}
	void calcdW() {
		for (uint32_t i = 0; i < (*Pool->UDlist[Units]).size(); i++)
			(*Pool->UDlist[Units])[i].calcdW();
	}
	void modify() {
		for (uint32_t i = 0; i < (*Pool->UDlist[Units]).size(); i++)
			(*Pool->UDlist[Units])[i].modify();
	}
};
class DNNInnerLayer: public DNNOutputLayer {
public:
	DNNInnerLayer(UnitPool *_Pool, uint32_t _Input, BaseLayer *_Up,
			uint32_t _Output, BaseLayer *_Down, StepFunc::func _step,
			double _Learnrate) {
		Pool = _Pool;
		LayerID = Pool->newLayer();
		Input = _Input, Output = _Output;
		Up = _Up;
		Down = _Down;
		Pool->Pool[LayerID].resize(
				Pool->Mats[Output].rows() * Pool->Mats[Output].cols());
		for (uint32_t i = 0, k = 0; i < Pool->Mats[Output].rows(); i++)
			for (uint32_t j = 0; j < Pool->Mats[Output].cols(); j++, k++) {
				DNNInnerUnit tmp(_Pool, _Input, _Up->Units, _Output,
						_Down->Units, std::pair<uint32_t, uint32_t>(i, j),
						_step, _Learnrate);
				Pool->Pool[LayerID][Pool->newUnit(LayerID)] = tmp;
			}
		Units = Pool->UpdateToUDlist(LayerID);
	}
	~DNNInnerLayer() {
	}
public:
	void train() {
		for (uint32_t i = 0; i < (*Pool->UDlist[Units]).size(); i++)
			(*Pool->UDlist[Units])[i].train();
	}
};

#endif /* LIB_LAYER_HPP_ */
