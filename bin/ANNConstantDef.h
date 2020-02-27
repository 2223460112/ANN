/*
 * ANNConstantDef.h
 *
 *  Created on: 2020年2月25日
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

#endif /* ANNCONSTANTDEF_H_ */
