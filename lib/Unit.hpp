/*
 * Unit.hpp
 *
 *  Created on: 20200225
 *      Author: Z2J
 */

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <cstdarg>
#include <cstdio>

#include "../lib/mem.hpp"

#include "../lib/Functions.hpp"

//This is the Base type witch includes most function that may be used;
class BaseUnit {
protected:
	UnitPool *Pool;
	uint32_t Input, Output;
	StepFunc::func step;
	LossFunc::func loss;

protected:
	uint32_t Downstream;

protected:
	MatrixXd W, dW;
	double bias;
	double Sumd, Outd, Learnrate; //sum stands for the sum of Wi*Inputi,out stands for step.f(sum)
	uint32_t modify_count;
public:
	double Lossd; //dE/dSum
	std::pair<uint32_t, uint32_t> OutputPos; //record where to put your result;

public:
	BaseUnit() {
	}
	BaseUnit(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss, double _Learnrate) {
		Pool = _Pool;
		modify_count = 0;
		Learnrate = _Learnrate;
		Input = _Input;
		Output = _Output, OutputPos = _Pos;
		W.setRandom(Pool->Mats[Input].rows(), Pool->Mats[Input].cols());
		W *= Weight_Init_Range;
		dW.setZero(Pool->Mats[Input].rows(), Pool->Mats[Input].cols());
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;

		Sumd = Outd = Lossd = 0;

		step = _step, loss = _loss;

		Downstream=-1;
	}
	virtual void FIXDlist(uint32_t _Downstream) {
		Downstream = _Downstream;
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
}
;
class DNNOutputUnit: public BaseUnit {
public:
	DNNOutputUnit(UnitPool *Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss, double _Learnrate) :
			BaseUnit(Pool, _Input, _Output, _Pos, _step, _loss, _Learnrate) {
	}
public:
	void calc() {
		Sumd = (W.array() * Pool->Mats[Input].array()).sum();
		Outd = step.f(Sumd);
		Pool->Mats[Output](OutputPos.first, OutputPos.second) = Outd;
	}
	void train(double targetV) {
		Lossd = -loss.f(targetV, Outd) * step.d(Outd);
	}
	void calcdW() {
		modify_count++;
		for (int i = 0; i < dW.rows(); i++)
			for (int j = 0; i < dW.cols(); j++)
				dW(i, j) += Learnrate * Pool->Mats[Input](i, j) * Lossd;
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
	DNNInnerUnit(UnitPool *Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos,
			StepFunc::func _step, double _Learnrate) :
			DNNOutputUnit(Pool, _Input, _Output, _Pos, _step, empty_loss_func,
					_Learnrate) {
		Downstream = -1;
	}
public:
	void train() {
		Lossd = 0;
		for (uint32_t u = 0; u < Pool->Dlist[Downstream]->size(); u++)
			Lossd += (*Pool->Dlist[Downstream])[u].returnWeightof(OutputPos)
					* (*Pool->Dlist[Downstream])[u].Lossd;
		Lossd *= -step.d(Outd);
	}
};

#endif /* Unit_HPP_ */
