/*
 * Unit.hpp
 *
 *  Created on: 2020年2月25日
 *      Author: Z2J
 */

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <eigen3/Eigen/Eigen>
using namespace Eigen;

#include "ANNConstantDef.h"
#include "Functions.hpp"

//This is the Base type witch includes most function that may be used;
class UnitBase {
protected:
	MatrixXd *Input, *Output; //Input and output must be a Matrix;
	uint32_t OutputPos[2]; //record where to put your result
public:
	void SetInput(MatrixXd *_Input) {
		Input = _Input;
	}
	void SetOutput(MatrixXd *_Output, uint32_t _Pos[2]) {
		Output = _Output, OutputPos = _Pos;
	}
	virtual void calc() { //calculate the Input,result should be put into Output;
	}
	virtual void train() { //train and calculate delta Weight;
	}
	virtual void modify() { //used the delta Weight to modify your unit's weight;
	}
	virtual ~UnitBase() { //virtual destructor;
	}
};
class DNNOutputUnit: public UnitBase {
protected:
	UnitBase** UpStream;
	uint32_t* UpStreamRK;
	uint32_t UpStreamSize;
protected:
	double bias, **W;
public:
};

#endif /* Unit_HPP_ */
