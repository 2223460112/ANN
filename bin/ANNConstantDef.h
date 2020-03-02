/*
 * ANNConstantDef.h
 *
 *  Created on: 20200225
 *      Author: Z2J
 */

#include <cstdlib>

#ifndef ANNCONSTANTDEF_H_
#define ANNCONSTANTDEF_H_

#define elu_alpha 0.25

#define selu_alpha 1.6732632423543772848170429916717
#define selu_scale 1.0507009873554804934193349852946

#define leaky_relu_alpha 0.01

#define r_relu_upperbound 0.3
#define r_relu_lowerbound 0.8
#define r_relu_test 0.5
#define r_relu_r (((double)rand()/RAND_MAX)*(r_relu_upperbound-r_relu_lowerbound)+r_relu_lowerbound)


#define Weight_Init_Range 0.05

#define InputUnit_type 0
#define InnerUnit_type 1
#define OutputUnit_type 2


#endif /* ANNCONSTANTDEF_H_ */
