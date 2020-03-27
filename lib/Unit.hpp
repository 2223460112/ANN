/*
 * Unit.hpp
 *
 *  Created on: 20200225
 *      Author: Z2J
 */

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include "../lib/Functions.hpp"

#include<cstdlib>

#include "../lib/mem.hpp"

//This is the Base type witch includes most function that may be used;
class BaseUnit {
protected:
	UnitPool *Pool;
	StepFunc::func step;
	LossFunc::func loss;

protected:
	uint32_t Downstream;

protected:
	MatrixXd W, dW;
	double bias, dbias;
	double Sumd, Outd; //sum stands for the sum of Wi*Inputi,out stands for step.f(sum)
	uint32_t modify_count;
public:
	double Lossd; //dE/dSum
	std::pair<uint32_t, uint32_t> OutputPos; //record where to put your result;
	virtual ushort TYPE() {
		return __BASE__;
	}

public:
	BaseUnit() {
	}
	BaseUnit(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss) {
		Pool = _Pool;
		modify_count = 0;
		OutputPos = _Pos;
		W.setRandom(Pool->Mats[_Input].rows(), Pool->Mats[_Input].cols());
		W *= Weight_Init_Range;
		dW.setZero(Pool->Mats[_Input].rows(), Pool->Mats[_Input].cols());
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;

		dbias = Sumd = Outd = Lossd = 0;

		step = _step, loss = _loss;

		Downstream = -1;
	}
public:
	void FIXDlist(uint32_t _Downstream) {
		Downstream = _Downstream;
	}

public:
	virtual void calc(uint32_t Input, uint32_t Output) { //calculate the Input,result should be put into Output;
	}
	virtual void train() { //train;
	}
	virtual void train(double targetV) { //for output unit
	}
	virtual void calcdW(uint32_t Input, double Learnrate) { //calculate delta Weight;
	}
	virtual void modify() { //used the delta Weight to modify your unit's weight;
	}
	virtual double returnWeightof(std::pair<uint32_t, uint32_t> _Pos) {
		return 0.0;
	}
public:
	virtual ~BaseUnit() {
	}
	void _save(FILE *file) {
		uint32_t intbuff;
		//double doublebuff;

		if (loss._s != NULL)
			loss.save(file);
		step.save(file);
		fiputc(Downstream, file)

		intbuff = W.rows();
		fiputc(intbuff, file)
		intbuff = W.cols();
		fiputc(intbuff, file)

		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++)
				fdputc(W(i, j), file)
		fdputc(bias, file)

		fiputc(OutputPos.first, file)
		fiputc(OutputPos.second, file)
	}
	virtual void save(FILE *file) {
	}
}
;
class DNNOutputUnit: public BaseUnit {
public:
	ushort TYPE() {
		return __DNN_OUTPUT__;
	}
public:
	DNNOutputUnit(UnitPool *Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss) :
			BaseUnit(Pool, _Input, _Output, _Pos, _step, _loss) {
	}
public:
	void calc(uint32_t Input, uint32_t Output) {
		Sumd = (W.array() * Pool->Mats[Input].array()).sum();
		Outd = step.f(Sumd);
		Pool->Mats[Output](OutputPos.first, OutputPos.second) = Outd;
	}
	void train(double targetV) {
		Lossd = -loss.f(targetV, Outd) * step.d(Outd);
	}
	void calcdW(uint32_t Input, double Learnrate) {
		modify_count++;
		for (int i = 0; i < dW.rows(); i++)
			for (int j = 0; j < dW.cols(); j++)
				dW(i, j) += Learnrate * Pool->Mats[Input](i, j) * Lossd;
		dbias += Learnrate * Lossd;
	}
	void modify() {
		for (int i = 0; i < dW.rows(); i++)
			for (int j = 0; j < dW.cols(); j++) {
				W(i, j) += dW(i, j) / modify_count;
				dW(i, j) = 0;
			}
		bias += dbias / modify_count;
		modify_count = 0;
	}
	double returnWeightof(std::pair<uint32_t, uint32_t> _Pos) {
		return W(_Pos.first, _Pos.second);
	}
};
class DNNInnerUnit: public DNNOutputUnit {
public:
	ushort TYPE() {
		return __DNN_INNER__;
	}
public:
	DNNInnerUnit(UnitPool *Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step) :
			DNNOutputUnit(Pool, _Input, _Output, _Pos, _step, empty_loss_func) {
		Downstream = -1;
	}
public:
	void train() {
		Lossd = 0;
		for (uint32_t u = 0; u < Pool->Dlist[Downstream].size(); u++)
			Lossd += Pool->AtDlist(Downstream, u).returnWeightof(OutputPos)
					* Pool->AtDlist(Downstream, u).Lossd;
		Lossd *= -step.d(Outd);
	}
};

UnitPool::~UnitPool() {
	for (uint32_t i = 0; i < Pool.size(); i++)
		for (uint32_t j = 0; j < Pool[i].size(); j++)
			delete Pool[i][j];
}

#endif /* Unit_HPP_ */
