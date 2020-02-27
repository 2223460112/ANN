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
	MatrixXd Input, Output; //Input and output must be a Matrix;
public:
	virtual void SetInput(MatrixXd _Input) { //Read your unit's input data from the forward unit's output data;
	}
	MatrixXd GetOutput() { //fixed function,no need to implement it;
		return Output;
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
class InputUnit: public UnitBase {
protected:
	UnitBase*** DownStream;
	uint32_t** DownStreamRK[2];
public:
};

#endif /* Unit_HPP_ */
