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
#include <cstdio>

#include <Eigen/Eigen>
using namespace Eigen;

#include "../lib/Functions.hpp"

//This is the Base type witch includes most function that may be used;
class BaseUnit {
protected:
	MatrixXd *Input, *Output; //Input and output must be a Matrix;
	StepFunc::func step; //as it's name;
	LossFunc::func loss;

protected:
	std::vector<BaseUnit> *Downstream, *Upstream;

protected:
	MatrixXd W, dW;
	double bias;
	double Sumd, Outd, Learnrate; //sum stands for the sum of Wi*Inputi,out stands for step.f(sum)
	uint32_t modify_count;
public:
	double Lossd; //dE/dSum
	std::pair<uint32_t, uint32_t> OutputPos; //record where to put your result;

public:
	BaseUnit(){}
	BaseUnit(MatrixXd *_Input, std::vector<BaseUnit> *_Upstream,
			MatrixXd *_Output, std::vector<BaseUnit> *_Downstream,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss, double _Learnrate) {
		modify_count = 0;
		Learnrate = _Learnrate;
		Input = _Input;
		Output = _Output, OutputPos = _Pos;
		W.setRandom(Input->rows(), Input->cols());
		W *= Weight_Init_Range;
		dW.setZero(Input->rows(), Input->cols());
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;

		Downstream = _Downstream;
		Upstream = _Upstream;

		Sumd = Outd = Lossd = 0;

		step = _step, loss = _loss;
	}

public:
	virtual void calc() { //calculate the Input,result should be put into Output;
	}
	virtual void train() { //train;
	}
	virtual void train(double targetV) { //for output unit
	}
	virtual void calcdW() { //calculate delta Weight;
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
	DNNOutputUnit(MatrixXd *_Input, std::vector<BaseUnit> *_Upstream,
			MatrixXd *_Output, std::vector<BaseUnit> *_Downstream,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss, double _Learnrate) :
			BaseUnit(_Input, _Upstream, _Output, _Downstream, _Pos, _step,
					_loss, _Learnrate) {
	}
public:
	void calc() {
		Sumd = (W.array() * Input->array()).sum();
		Outd = step.f(Sumd);
		(*Output)(OutputPos.first, OutputPos.second) = Outd;
	}
	void train(double targetV) {
		Lossd = -loss.f(targetV, Outd) * step.d(Outd);
	}
	void calcdW() {
		modify_count++;
		for (int i = 0; i < dW.rows(); i++)
			for (int j = 0; i < dW.cols(); j++)
				dW(i, j) += Learnrate * (*Input)(i, j) * Lossd;
	}
	void modify() {
		for (int i = 0; i < dW.rows(); i++)
			for (int j = 0; i < dW.cols(); j++) {
				W(i, j) += dW(i, j) / modify_count;
				dW(i, j) = 0;
			}
		modify_count = 0;
	}
	double returnWeightof(std::pair<uint32_t, uint32_t> _Pos) {
		return W(_Pos.first, _Pos.second);
	}
};
class DNNInnerUnit: public DNNOutputUnit {
public:
	DNNInnerUnit(MatrixXd *_Input, std::vector<BaseUnit> *_Upstream,
			MatrixXd *_Output, std::vector<BaseUnit> *_Downstream,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			double _Learnrate) :
			DNNOutputUnit(_Input, _Upstream, _Output, _Downstream, _Pos, _step,
					empty_loss_func, _Learnrate) {
	}
public:
	void train() {
		Lossd = 0;
		for (uint32_t u = 0; u < Downstream->size(); u++)
			Lossd += (*Downstream)[u].returnWeightof(OutputPos)
					* (*Downstream)[u].Lossd;
		Lossd *= -step.d(Outd);
	}
};

#endif /* Unit_HPP_ */
