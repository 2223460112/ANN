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

#include <Eigen\Eigen>
using namespace Eigen;

#include "ANNConstantDef.h"
#include "Functions.hpp"

//This is the Base type witch includes most function that may be used;
class UnitBase {
protected:
	MatrixXd *Input, *Output; //Input and output must be a Matrix;
	uint32_t OutputPos[2]; //record where to put your result
	StepFunc::func step; //as it's name
	LossFunc::func loss;

protected:
	std::vector<UnitBase*> Downstream, Upstream;
	std::vector<uint32_t> DownstreamRK[2], UpstreamRK[2]; //RK stands for Rank,record where self is to it's D/U stream unit

protected:
	MatrixXd W;
	double bias;
	double Sumd, Outd; //sum stands for the sum of Wi*Inputi,out stands for step.f(sum)
	uint32_t type;

public:
	double Lossd; //dE/dSum

public:
	void Init(short _type, MatrixXd *_Input, MatrixXd *_Output, uint32_t _Pos[]) {
		type = _type;
		Input = _Input;
		Output = _Output, OutputPos = _Pos;
		W.setRandom(Input->rows(), Input->cols());
		W *= Weight_Init_Range;
		bias = ((double) rand() / RAND_MAX * 2 - 1) * Weight_Init_Range;

		//working here
	}
	virtual void init(short type, ...) {
	}

public:
	virtual void calc() { //calculate the Input,result should be put into Output;
	}
	virtual void train() { //train and calculate delta Weight;
	}
	virtual void modify() { //used the delta Weight to modify your unit's weight;
	}

public:
	virtual ~UnitBase() { //virtual destructor;
	}
};
class DNNOutputUnit: public UnitBase {
public:
};

#endif /* Unit_HPP_ */
