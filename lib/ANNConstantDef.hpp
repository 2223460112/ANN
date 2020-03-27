/*
 * ANNConstantDef.h
 *
 *  Created on: 20200225
 *      Author: Z2J
 */

#ifndef ANNCONSTANTDEF_H_
#define ANNCONSTANTDEF_H_

#include <cstdlib>

#define elu_alpha 0.25

#define selu_alpha 1.6732632423543772848170429916717
#define selu_scale 1.0507009873554804934193349852946

#define leaky_relu_alpha 0.01

#define r_relu_upperbound 0.3
#define r_relu_lowerbound 0.8
#define r_relu_test 0.5
#define r_relu_r (((double)rand()/RAND_MAX)*(r_relu_upperbound-r_relu_lowerbound)+r_relu_lowerbound)

#define Weight_Init_Range 0.05

#define Ftype(T) T.f,T.d,T.s

#define size_long_int 4
#define size_double 8
#define ushort unsigned short

#define __SIGMOND__ 1
#define __TANH__ 2
#define __HARD_SIGMOND__ 3
#define __RELU__ 4
#define __RELU6__ 5
#define __ELU__ 7
#define __SELU__ 8
#define __LEAKY_RELU__ 9
#define __R_RELU__ 10
#define __HARD_SWISH__ 11
#define __MISH__ 12
#define __SWISH__ 13

#define __QUAD__ 1
#define __CROSS_ENTROPY__ 2

#define __BASE__ 0
#define __DNN_OUTPUT__ 1
#define __DNN_INNER__ 2

#define fdputc(x,f) for(uint32_t i=0;i<sizeof(double);i++)putc(((unsigned char *)&x)[i],f);
#define fdgetc(x,f) for(uint32_t i=0;i<sizeof(double);i++)((unsigned char *)&x)[i]=getc(f);
#define fiputc(x,f) for(uint32_t i=0;i<sizeof(uint32_t);i++)putc(((unsigned char *)&x)[i],f);
#define figetc(x,f) for(uint32_t i=0;i<sizeof(uint32_t);i++)((unsigned char *)&x)[i]=getc(f);
#define fbputc(x,f) putc(x,f);
#define fbgetc(x,f) x=getc(f);

#endif /* ANNCONSTANTDEF_H_ */
