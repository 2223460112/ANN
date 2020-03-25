/*
 * Layer.hpp
 *
 *  Created on: 20200306
 *      Author: Z2J
 */

#ifndef LIB_LAYER_HPP_
#define LIB_LAYER_HPP_

#include "Unit.hpp"
#include "mem.hpp"

class BaseLayer {
protected:
	BaseLayer *Up, *Down;
	MatrixXd *Input, *Output;
	UnitPool *Pool;
	uint32_t NetID, LayerID;
public:
	std::vector<BaseUnit> *Units;
public:
	BaseLayer() {
	}
	virtual ~BaseLayer() {
	}
	virtual void FIXUDlist() {
		for(uint32_t i=0;i<Units->size();i++)(*Units)[i].FIXUDlist(Up->Units,Down->Units);
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
	DNNOutputLayer(MatrixXd *_Input, BaseLayer *_Up, MatrixXd *_Output,
			StepFunc::func _step, LossFunc::func _loss, double _Learnrate,
			uint32_t _NetID, UnitPool *_Pool) {
		NetID = _NetID, Pool = _Pool;
		LayerID = Pool->newLayer(NetID);
		Input = _Input, Output = _Output;
		Up = _Up;
		Units->resize(Output->rows() * Output->cols());
		for (uint32_t i = 0, k = 0; i < Output->rows(); i++)
			for (uint32_t j = 0; j < Output->cols(); j++, k++) {
				DNNOutputUnit tmp(_Input, _Up->Units, _Output, NULL,
						std::pair<uint32_t, uint32_t>(i, j), _step, _loss,
						_Learnrate);
				Pool->Pool[NetID][LayerID][Pool->newUnit(NetID, LayerID)] = tmp;
			}
		Units = Pool->UDlist[Pool->UpdateToUDlist(NetID, LayerID)];
	}
	~DNNOutputLayer() {
	}
public:
	void calc() {
		for (uint32_t i = 0; i < Units->size(); i++)
			(*Units)[i].calc();
	}
	void train(MatrixXd targetV) {
		for (uint32_t i = 0; i < Units->size(); i++)
			(*Units)[i].train(
					targetV((*Units)[i].OutputPos.first,
							(*Units)[i].OutputPos.second));
	}
	void calcdW() {
		for (uint32_t i = 0; i < Units->size(); i++)
			(*Units)[i].calcdW();
	}
	void modify() {
		for (uint32_t i = 0; i < Units->size(); i++)
			(*Units)[i].modify();
	}
};
class DNNInnerLayer: public DNNOutputLayer {
public:
	DNNInnerLayer(MatrixXd *_Input, BaseLayer *_Up, MatrixXd *_Output,
			BaseLayer *_Down, StepFunc::func _step, double _Learnrate,
			uint32_t _NetID, UnitPool *_Pool) :
			DNNOutputLayer(_Input, _Up, _Output, _step, empty_loss_func,
					_Learnrate, _NetID, _Pool) {
		Down = _Down;
	}
	~DNNInnerLayer() {
	}
public:
	void train() {
		for (uint32_t i = 0; i < Units->size(); i++)
			(*Units)[i].train();
	}
};

#endif /* LIB_LAYER_HPP_ */
