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
#include<iostream>

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
	double Lossd; //dE/dSum
	std::pair<uint32_t, uint32_t> OutputPos; //record where to put your result;
public:
	virtual ushort TYPE() {
		return __BASE__;
	}

public:
	virtual void FIXDlist(uint32_t _Downstream) {
		Downstream = _Downstream;
	}
	virtual void FIXload(uint32_t input) {
		modify_count = 0;
		dW.setZero(W.rows(), W.cols());
		dbias = Sumd = Outd = Lossd = 0;
	}
public:
	BaseUnit() {
	}
	BaseUnit(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss) {
		Pool = _Pool;
		OutputPos = _Pos;
		W.setRandom(Pool->Mats[_Input].rows(), Pool->Mats[_Input].cols());
		W *= Weight_Init_Range;
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;
		Downstream = -1;
		step = _step, loss = _loss;
		FIXload(_Input);
	}

public:
	virtual void calc(uint32_t Input, uint32_t Output) { //calculate the Input,result should be put into Output;
	}
	virtual void train() { //train;
	}
	virtual void trainT(double targetV) { //for output unit
	}
	virtual void calcdW(uint32_t Input, double Learnrate) { //calculate delta Weight;
	}
	virtual void modify() { //used the delta Weight to modify your unit's weight;
	}
	virtual double returnWeightof(std::pair<uint32_t, uint32_t> _Pos) {
		return 0.0;
	}
	virtual double returnLossof(std::pair<uint32_t, uint32_t> _Pos) {
		return 0.0;
	}
public:
	virtual ~BaseUnit() {
	}
	virtual void save(FILE *file) {
		uint32_t intbuff;
		double doublebuff;

		if (loss._s != NULL)
			loss.save(file);
		step.save(file);

		fiputc(Downstream, file)

		intbuff = W.rows();
		fiputc(intbuff, file)
		intbuff = W.cols();
		fiputc(intbuff, file)

		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++) {
				doublebuff = W(i, j);
				fdputc(doublebuff, file)
			}
		fdputc(bias, file)

		fiputc(OutputPos.first, file)
		fiputc(OutputPos.second, file)
	}
	virtual void load(FILE *file, UnitPool *_Pool) {
	}
}
;
class DNNOutputUnit: public BaseUnit {
public:
	ushort TYPE() {
		return __DNN_OUTPUT__;
	}
public:
	DNNOutputUnit() {
	}
	DNNOutputUnit(UnitPool *Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _Pos, StepFunc::func _step,
			LossFunc::func _loss) :
			BaseUnit(Pool, _Input, _Output, _Pos, _step, _loss) {
	}
	void load(FILE *file, UnitPool *_Pool) {
		Pool = _Pool;
		uint32_t intbuff, intbuff2;
		double doublebuff;

		loss.load(getc(file));
		step.load(getc(file));

		figetc(Downstream, file)

		figetc(intbuff, file)
		figetc(intbuff2, file)
		W.setZero(intbuff, intbuff2);

		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++) {
				fdgetc(doublebuff, file)
				W(i, j) = doublebuff;
			}
		fdgetc(bias, file)

		figetc(OutputPos.first, file)
		figetc(OutputPos.second, file)
	}
	DNNOutputUnit(FILE *file, UnitPool *_Pool) {
		load(file, _Pool);
	}
public:
	void calc(uint32_t Input, uint32_t Output) {
		Sumd = (W.array() * Pool->Mats[Input].array()).sum() + bias;
		Outd = step.f(Sumd);
		Pool->Mats[Output](OutputPos.first, OutputPos.second) = Outd;
	}
	void trainT(double targetV) {
		Lossd = -loss.f(targetV, Outd) * step.d(Outd);
	}
	void calcdW(uint32_t Input, double Learnrate) {
		modify_count++;
		dW += Learnrate * Pool->Mats[Input] * Lossd;
		dbias += Learnrate * Lossd;
	}
	void modify() {
		W += (dW / modify_count);
		dW *= 0.0;
		bias += dbias / modify_count;
		dbias = 0;
		modify_count = 0;
	}
	double returnWeightof(std::pair<uint32_t, uint32_t> _Pos) {
		return W(_Pos.first, _Pos.second);
	}
	double returnLossof(std::pair<uint32_t, uint32_t> _Pos) {
		return Lossd;
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
	}
	void load(FILE *file, UnitPool *_Pool) {
		Pool = _Pool;
		uint32_t intbuff, intbuff2;
		double doublebuff;

		step.load(getc(file));

		figetc(Downstream, file)

		figetc(intbuff, file)
		figetc(intbuff2, file)
		W.setZero(intbuff, intbuff2);

		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++) {
				fdgetc(doublebuff, file)
				W(i, j) = doublebuff;
			}
		fdgetc(bias, file)

		figetc(OutputPos.first, file)
		figetc(OutputPos.second, file)
	}
	DNNInnerUnit(FILE *file, UnitPool *_Pool) {
		load(file, _Pool);
	}
public:
	void train() {
		Lossd = 0;
		for (uint32_t u = 0; u < Pool->Dlist[Downstream].size(); u++)
			Lossd += Pool->AtDlist(Downstream, u).returnWeightof(OutputPos)
					* Pool->AtDlist(Downstream, u).returnLossof(OutputPos);
		Lossd *= step.d(Outd);
	}
};
class ConvolutionUnit: public BaseUnit {
public:
	ushort TYPE() {
		return __CONVOLUTION__;
	}
	uint32_t padding, stride, channel, Itmp;
	std::vector<std::vector<StepFunc::func>> steps;
	MatrixXd TdW, Outd, Sumd, Lossd;
public:
	ConvolutionUnit(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			std::pair<uint32_t, uint32_t> _kernalsize, StepFunc::func _step,
			uint32_t _padding, uint32_t _stride, uint32_t _channel) {
		Pool = _Pool;
		padding = _padding, stride = _stride, channel = _channel;
		Itmp = Pool->Mats.size();
		Pool->Mats.resize(Pool->Mats.size() + 1);
		if (padding)
			Pool->Mats[Itmp].setZero(
					channel * (_kernalsize.first * 2 - 2)
							+ Pool->Mats[_Input].rows(),
					channel * (_kernalsize.second * 2 - 2)
							+ Pool->Mats[_Input].cols());
		else
			Pool->Mats[Itmp].setZero(Pool->Mats[_Input].rows(),
					Pool->Mats[_Input].cols());
		W.setRandom(_kernalsize.first, _kernalsize.second) * Weight_Init_Range;
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;
		dW.setZero(_kernalsize.first, _kernalsize.second);
		TdW.setZero(W.rows(), W.cols());
		dbias = 0;
		modify_count = 0;

		Outd.setZero(
				(Pool->Mats[Itmp].rows() / channel - _kernalsize.first + stride)
						/ stride,
				(Pool->Mats[Itmp].cols() - _kernalsize.second + stride)
						/ stride);
		Sumd.setZero(Outd.rows(), Outd.cols());
		Lossd.setZero(Outd.rows(), Outd.cols());

		steps.resize(Outd.rows());
		for (uint32_t i = 0; i < steps.size(); i++) {
			steps[i].resize(Outd.cols());
			for (uint32_t j = 0; j < steps[i].size(); j++)
				steps[i][j] = _step;
		}
		Downstream = -1;
	}
	void FIXDlist(uint32_t _Dlist) {
		Downstream = _Dlist;
	}
	void FIXload(uint32_t input) {
	}
	void save(FILE *file) {
		uint32_t intbuff;
		double doublebuff;

		fiputc(Downstream, file)

		fiputc(padding, file)
		fiputc(channel, file)
		fiputc(stride, file)
		fiputc(Itmp, file)

		intbuff = W.rows();
		fiputc(intbuff, file)
		intbuff = W.cols();
		fiputc(intbuff, file)

		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++) {
				doublebuff = W(i, j);
				fdputc(doublebuff, file)
			}
		fdputc(bias, file)

		for (uint32_t i = 0; i < Outd.rows(); i++)
			for (uint32_t j = 0; j < Outd.cols(); j++)
				steps[i][j].save(file);
	}
	ConvolutionUnit(FILE *file, UnitPool *_Pool) {
		Pool = _Pool;
		uint32_t intbuff, intbuff2;
		double doublebuff;

		figetc(Downstream, file)

		figetc(padding, file)
		figetc(channel, file)
		figetc(stride, file)
		figetc(Itmp, file)

		figetc(intbuff, file)
		figetc(intbuff2, file)
		W.setZero(intbuff, intbuff2);

		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++) {
				fdgetc(doublebuff, file)
				W(i, j) = doublebuff;
			}
		fdgetc(bias, file)

		dW.setZero(W.rows(), W.cols());
		TdW.setZero(W.rows(), W.cols());

		Outd.setZero(
				(Pool->Mats[Itmp].rows() / channel - W.rows() + stride)
						/ stride,
				(Pool->Mats[Itmp].cols() - W.cols() + stride) / stride);
		Sumd.setZero(Outd.rows(), Outd.cols());
		Lossd.setZero(Outd.rows(), Outd.cols());

		steps.resize(Outd.rows());
		for (uint32_t i = 0; i < steps.size(); i++) {
			steps[i].resize(Outd.cols());
			for (uint32_t j = 0; j < steps[i].size(); j++)
				steps[i][j].load(getc(file));
		}
		dbias = 0;
		modify_count = 0;
	}
public:
	void calc(uint32_t Input, uint32_t Output) {
		Outd *= 0;
		Pool->Mats[Itmp] *= 0;
		uint32_t Irow = Pool->Mats[Input].rows() / channel;
		for (uint32_t i = 0, ps = 0; i < channel; i++) {
			if (padding)
				ps += W.rows();
			Pool->Mats[Itmp].block(ps, 0, Irow, Pool->Mats[Input].cols()) =
					Pool->Mats[Input].block(Irow * i, 0, Irow,
							Pool->Mats[Input].cols());
			ps += Irow;
			if (padding)
				ps--;
		}
		if (padding)
			Irow += W.rows() * 2 - 2;
		for (uint32_t ch = 0; ch < channel; ch++)
			for (uint32_t i = 0; i < Outd.rows(); i++)
				for (uint32_t j = 0; j < Outd.cols(); j++)
					Sumd(i, j) +=
							(W.array()
									* Pool->Mats[Itmp].block(
											ch * Irow + i * stride, j * stride,
											W.rows(), W.cols()).array()).sum();
		for (uint32_t i = 0; i < Outd.rows(); i++)
			for (uint32_t j = 0; j < Outd.cols(); j++) {
				Sumd(i, j) += bias;
				Outd(i, j) = steps[i][j].f(Sumd(i, j));
			}
		Pool->Mats[Output] = Outd;
	}
	void train() {
		for (uint32_t i = 0; i < Outd.rows(); i++)
			for (uint32_t j = 0; j < Outd.cols(); j++) {
				Lossd(i, j) = 0;
				for (uint32_t u = 0; u < Pool->Dlist[Downstream].size(); u++)
					Lossd(i, j) += Pool->AtDlist(Downstream, u).returnWeightof(
							std::make_pair(i, j))
							* Pool->AtDlist(Downstream, u).returnLossof(
									std::make_pair(i, j));
				Lossd(i, j) *= steps[i][j].d(Outd(i, j));
			}
	}
	void calcdW(uint32_t Input, double Learnrate) {
		uint32_t Irow = Pool->Mats[Input].rows() / channel;
		if (padding)
			Irow += W.rows() * 2 - 2;
		TdW *= 0;
		for (uint32_t i = 0; i < W.rows(); i++)
			for (uint32_t j = 0; j < W.cols(); j++) {
				for (uint32_t ch = 0; ch < channel; ch++)
					for (uint32_t px = 0; px < Outd.rows(); px++)
						for (uint32_t py = 0; py < Outd.cols(); py++)
							TdW(i, j) += Lossd(px, py) * Learnrate
									* Pool->Mats[Itmp](ch * Irow + i * stride,
											j * stride);
				dW(i, j) += TdW(i, j) / (Outd.rows() * Outd.cols() * channel);
			}
		double Tdbias = Learnrate * Lossd.sum();
		dbias += Tdbias / (Outd.rows() * Outd.cols());
		modify_count++;
	}
	void modify() {
		W += (dW / modify_count);
		dW *= 0.0;
		bias += dbias / modify_count;
		dbias = 0;
		modify_count = 0;
	}
public:
	double returnWeightof(std::pair<uint32_t, uint32_t> Pos) {
		double tW = 0;
		std::pair<uint32_t, uint32_t> IPos = Pos;
		if (padding)
			IPos.first += W.rows() - 1, IPos.second += W.cols() - 1;
		std::pair<uint32_t, uint32_t> Is, Isc;
		Is.first = (IPos.first / stride), Is.second = (IPos.second / stride);
		Isc.first = (stride * Is.first + W.rows() - IPos.first - 1), Isc.second =
				(stride * Is.second + W.cols() - IPos.second - 1);
		if (IPos.first < Isc.first)
			Isc.first = IPos.first;
		if (IPos.second < Isc.second)
			Isc.second = IPos.second;
		Isc.first /= stride, Isc.second /= stride;
		for (uint32_t px = 0; px < Isc.first; px++)
			for (uint32_t py = 0; py < Isc.second; py++)
				tW += Lossd(px + Is.first, py + Is.second);
		return tW;
	}
	double returnLossof(std::pair<uint32_t, uint32_t> _Pos) {
		return Lossd;
	}
};
UnitPool::~UnitPool() {
	for (uint32_t i = 0; i < Pool.size(); i++)
		for (uint32_t j = 0; j < Pool[i].size(); j++)
			delete Pool[i][j];
}

#endif /* Unit_HPP_ */
