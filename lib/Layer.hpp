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
	BaseLayer *Up, *Down;
	MatrixXd *Input, *Output;
public:
	std::vector<BaseUnit*> Units;
public:
	BaseLayer() {
	}
	virtual ~BaseLayer() {
		for (int i = 0; i < Units.size(); i++)
			delete Units[i];
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
public:
	virtual void read(FILE *file) {
	}
	virtual void write(FILE *file) {
	}
};
class DNNOutputLayer: public BaseLayer {
public:
	DNNOutputLayer(MatrixXd *_Input, BaseLayer*_Up, MatrixXd *_Output,
			BaseLayer *_Down, StepFunc::func _step, LossFunc::func _loss,
			double _Learnrate) {
		Input = _Input, Output = _Output;
		Up = _Up, Down = _Down;
		Units.reisze(Output->rows()*Output->cols());
		for(int i=0,k=0;i<Output->rows();i++)
			for(int j=0;j<Output->cols();j++,k++)
				Unit[i]=new DNNOutputUnit(_Input,_Up->Units)
		}
	};

#endif /* LIB_LAYER_HPP_ */
