/*
 * Unit.hpp
 *
 *  Created on: 20200225
 *      Author: Z2J
 */

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <vector>
#include <cstdarg>

#include <Eigen/Eigen>
using namespace Eigen;

#include "ANNConstantDef.h"
#include "Functions.hpp"

//This is the Base type witch includes most function that may be used;
class BaseUnit {
protected:
	MatrixXd *Input, *Output; //Input and output must be a Matrix;
	std::pair<uint32_t, uint32_t> OutputPos; //record where to put your result;
	StepFunc::func step; //as it's name;
	LossFunc::func loss;

protected:
	std::vector<BaseUnit*> *Downstream, *Upstream;

protected:
	MatrixXd W;
	double bias;
	double Sumd, Outd; //sum stands for the sum of Wi*Inputi,out stands for step.f(sum)
	uint32_t type;

public:
	double Lossd; //dE/dSum

public:
	BaseUnit(short _type, MatrixXd *_Input, std::vector<BaseUnit*> *_Upstream,
			MatrixXd *_Output, std::vector<BaseUnit*> *_Downstream,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss) {
		type = _type;
		Input = _Input;
		Output = _Output, OutputPos = _Pos;
		W.setRandom(Input->rows(), Input->cols());
		W *= Weight_Init_Range;
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;

		Downstream = _Downstream;
		Upstream = _Upstream;

		Sumd = Outd = Lossd = 0;

		step = _step, loss = _loss;
	}

public:
	virtual void calc() { //calculate the Input,result should be put into Output;
	}
	virtual void train() { //train and calculate delta Weight;
	}
	virtual void train(double targetV) { //for output unit
	}
	virtual void modify() { //used the delta Weight to modify your unit's weight;
	}
	virtual double returnWeightof(std::pair<uint32_t, uint32_t> _Pos) {
		return 0.0;
	}

public:
	virtual ~BaseUnit() { //virtual destructor;
	}
};
class DNNOutputUnit: public BaseUnit {
public:
	DNNOutputUnit(short _type, MatrixXd *_Input,
			std::vector<BaseUnit*> *_Upstream, MatrixXd *_Output,
			std::vector<BaseUnit*> *_Downstream,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss) :
			BaseUnit(_type, _Input, _Upstream, _Output, _Downstream, _Pos,
					_step, _loss) {
	}
public:
	void calc() {
		Sumd = (W.array() * Input->array()).sum();
		Outd = step.f(Sumd);
		(*Output)(OutputPos.first, OutputPos.second) = Outd;
	}
	void train(double targetV) {
		Lossd=loss.f(targetV,Outd)*step.d(Outd);
	}
};

#endif /* Unit_HPP_ */
