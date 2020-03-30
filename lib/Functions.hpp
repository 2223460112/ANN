/*
 * Functions.hpp
 *
 *  Created on: 20200225
 *      Author: Z2J
 */

#ifndef FUNCTIONS_HPP_
#define FUNCTIONS_HPP_

#include <cmath>
#include <algorithm>

#include "ANNConstantDef.hpp"

typedef double d2dfunc(double, double&);
typedef double d2dfunc2(double, double);
typedef void sfunc(FILE*);

namespace StepFunc {
struct sigmoid {
	static double f(double x, double &li) {
		return 1.0 / (1.0 + exp(-x));
	}
	static double d(double y, double &li) {
		return y * (1.0 - y);
	}
	static void s(FILE *file) {
		putc(__SIGMOID__, file);
	}
}vsigmoid;
struct tanh {
	static double f(double x, double &li) {
		return std::tanh(x);
	}
	static double d(double y, double &li) {
		return 1.0 - y * y;
	}
	static void s(FILE *file) {
		putc(__TANH__, file);
	}
}vtanh;
struct hard_sigmoid {
	static double f(double x, double &li) {
		li = x;
		return std::max(0.0, std::min(1.0, x / 5.0 + 0.5));
	}
	static double d(double y, double &li) {
		return li < -2.5 ? 0.0 : (li > 2.5 ? 0.0 : 0.2);
	}
	static void s(FILE *file) {
		putc(__HARD_SIGMOID__, file);
	}
}vhard_sigmoid;
struct relu {
	static double f(double x, double &li) {
		return std::max(0.0, x);
	}
	static double d(double y, double &li) {
		return y > 0.0 ? 1.0 : 0.0;
	}
	static void s(FILE *file) {
		putc(__RELU__, file);
	}
}vrelu;
struct relu6 {
	static double f(double x, double &li) {
		return std::min(6.0, std::max(0.0, x));
	}
	static double d(double y, double &li) {
		return y > 6.0 ? 0.0 : y < 0.0 ? 0.0 : 1.0;
	}
	static void s(FILE *file) {
		putc(__RELU6__, file);
	}
}vrelu6;
struct elu {
	static double f(double x, double &li) {
		li = x;
		return x > 0.0 ? x : (exp(x) - 1.0) * elu_alpha;
	}
	static double d(double y, double &li) {
		return li > 0.0 ? 1.0 : f(li, li) + elu_alpha;
	}
	static void s(FILE *file) {
		putc(__ELU__, file);
	}
}velu;
struct selu {
	static double f(double x, double &li) {
		li = x;
		return selu_scale * (x > 0.0 ? x : (exp(x) - 1.0) * selu_alpha);
	}
	static double d(double y, double &li) {
		return selu_scale * (li > 0.0 ? 1 : f(li, li) + selu_alpha);

	}
	static void s(FILE *file) {
		putc(__SELU__, file);
	}
}vselu;
struct leaky_relu {
	static double f(double x, double &li) {
		return x > 0 ? x : x * leaky_relu_alpha;
	}
	static double d(double y, double &li) {
		return y <= 0 ? leaky_relu_alpha : 1;
	}
	static void s(FILE *file) {
		putc(__LEAKY_RELU__, file);
	}
}vleaky_relu;
struct r_relu {
	static double f(double x, double &li) {
#ifdef _train_
		li=r_relu_r;
#elif _test_
		li=r_relu_test;
#endif
		return x > 0 ? x : x * li;
	}
	static double d(double y, double &li) {
		return y > 0 ? 1 : li;
	}
	static void s(FILE *file) {
		putc(__R_RELU__, file);
	}
}vr_relu;
struct hard_swish {
	static double f(double x, double &li) {
		li = x;
		return x * std::min(6.0, std::max(0.0, x + 3.0)) / 6.0;
	}
	static double d(double y, double &li) {
		return (std::min(6.0, std::max(0.0, li + 3.0))
				+ li * (li + 3.0 > 6.0 ? 0.0 : li + 3 < 0.0 ? 0.0 : 1.0)) / 6.0;
	}
	static void s(FILE *file) {
		putc(__HARD_SWISH__, file);
	}
}vhard_swish;
struct mish {
	static double f(double x, double &li) {
		li = x;
		return x * std::tanh(log(1 + exp(x)));
	}
	static double d(double y, double &li) {
		return exp(li)
				* (4.0 * (li + 1.0) + 4.0 * exp(2.0 * li) + exp(3.0 * li)
						+ exp(li) * (4.0 * li + 6))
				/ (2 * exp(li) + exp(2.0 * li) + 2)
				/ (2 * exp(li) + exp(2.0 * li) + 2);
	}
	static void s(FILE *file) {
		putc(__MISH__, file);
	}
}vmish;
//swish not fully implemented
struct swish {
	static double f(double x, double &li) {
		li = x;
		return x * (1.0 / (1.0 + exp(-x)));
	}
	static double d(double y, double &li) {
		return y + (1 - y) * (1.0 / (1.0 + exp(-li)));
	}
	static void s(FILE *file) {
		putc(__SWISH__, file);
	}
}vswish;
//li refers to last_in
class func {
private:
	double li = 0, lo = 0;
	d2dfunc *_f = NULL, *_d = NULL;
	sfunc *_s = NULL;
public:
	double f(double x) {
		return (*_f)(x, li);
	}
	double d(double y) {
		return (*_d)(y, li);
	}
public:
	void save(FILE *file) {
		_s(file);
	}
	void load(ushort TYPE) {
		if (TYPE == __SIGMOID__) {
			_f = vsigmoid.f;
			_d = vsigmoid.d;
			_s = vsigmoid.s;
		} else if (TYPE == __TANH__) {
			_f = vtanh.f;
			_d = vtanh.d;
			_s = vtanh.s;
		} else if (TYPE == __RELU__) {
			_f = vrelu.f;
			_d = vrelu.d;
			_s = vrelu.s;
		} else if (TYPE == __ELU__) {
			_f = velu.f;
			_d = velu.d;
			_s = velu.s;
		} else if (TYPE == __RELU6__) {
			_f = vrelu6.f;
			_d = vrelu6.d;
			_s = vrelu6.s;
		} else if (TYPE == __SELU__) {
			_f = vselu.f;
			_d = vselu.d;
			_s = vselu.s;
		} else if (TYPE == __LEAKY_RELU__) {
			_f = vleaky_relu.f;
			_d = vleaky_relu.d;
			_s = vleaky_relu.s;
		} else if (TYPE == __HARD_SWISH__) {
			_f = vhard_swish.f;
			_d = vhard_swish.d;
			_s = vhard_swish.s;
		} else if (TYPE == __MISH__) {
			_f = vmish.f;
			_d = vmish.d;
			_s = vmish.s;
		} else if (TYPE == __SWISH__) {
			_f = vswish.f;
			_d = vswish.d;
			_s = vswish.s;
		}
	}
	func() {
	}
	func(ushort TYPE) {
		li = lo = 0.0;
		load(TYPE);
	}
	func(FILE *file) {
		li = lo = 0.0;
		ushort TYPE = getc(file);
		load(TYPE);
	}
};
}

namespace LossFunc {
static inline double quad(double y, double a) {
	return a - y;
}
static inline void quad_s(FILE *file) {
	fbputc(__QUAD__, file)
}
class func {
	d2dfunc2 *_f = NULL;
public:
	sfunc *_s = NULL;
	void load(ushort TYPE){
		if(TYPE==__QUAD__){
			_f=quad;_s=quad_s;
		}
	}
	func() {
	}
	func(ushort TYPE) {
		load(TYPE);
	}
	func(FILE* file){
		load(getc(file));
	}
	double f(double y, double a) {
		return _f(y, a);
	}
	void save(FILE *file) {
		_s(file);
	}
};
}

const LossFunc::func empty_loss_func;

#endif /* FUNCTIONS_HPP_ */
