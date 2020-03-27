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
//li refers to last_in
class func {
private:
	double li = 0, lo = 0;
	d2dfunc *_f = NULL, *_d = NULL;
	sfunc *_s = NULL;
public:
	func() {
	}
	func(d2dfunc *fi, d2dfunc *di, sfunc *si) {
		li = lo = 0.0;
		_f = fi, _d = di, _s = si;
	}
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
};
class sigmoid {
public:
	static double f(double x, double &li) {
		return 1.0 / (1.0 + exp(-x));
	}
	static double d(double y, double &li) {
		return y * (1.0 - y);
	}
	static void s(FILE *file) {
		fbputc(__SIGMOND__, file)
	}
};
class tanh {
public:
	static double f(double x, double &li) {
		return std::tanh(x);
	}
	static double d(double y, double &li) {
		return 1.0 - y * y;
	}
	static void s(FILE *file) {
		fbputc(__TANH__, file)
	}
};
class hard_sigmond {
public:
	static double f(double x, double &li) {
		li = x;
		return std::max(0.0, std::min(1.0, x / 5.0 + 0.5));
	}
	static double d(double y, double &li) {
		return li < -2.5 ? 0.0 : (li > 2.5 ? 0.0 : 0.2);
	}
	static void s(FILE *file) {
		fbputc(__HARD_SIGMOND__, file)
	}
};
class relu {
public:
	static double f(double x, double &li) {
		return std::max(0.0, x);
	}
	static double d(double y, double &li) {
		return y > 0.0 ? 1.0 : 0.0;
	}
	static void s(FILE *file) {
		fbputc(__RELU__, file)
	}
};
class relu6 {
public:
	static double f(double x, double &li) {
		return std::min(6.0, std::max(0.0, x));
	}
	static double d(double y, double &li) {
		return y > 6.0 ? 0.0 : y < 0.0 ? 0.0 : 1.0;
	}
	static void s(FILE *file) {
		fbputc(__RELU6__, file)
	}
};
class elu {
public:
	static double f(double x, double &li) {
		li = x;
		return x > 0.0 ? x : (exp(x) - 1.0) * elu_alpha;
	}
	static double d(double y, double &li) {
		return li > 0.0 ? 1.0 : f(li, li) + elu_alpha;
	}
	static void s(FILE *file) {
		fbputc(__ELU__, file)
	}
};
class selu {
public:
	static double f(double x, double &li) {
		li = x;
		return selu_scale * (x > 0.0 ? x : (exp(x) - 1.0) * selu_alpha);
	}
	static double d(double y, double &li) {
		return selu_scale * (li > 0.0 ? 1 : f(li, li) + selu_alpha);

	}
	static void s(FILE *file) {
		fbputc(__SELU__, file)
	}
};
class leaky_relu {
public:
	static double f(double x, double &li) {
		return x > 0 ? x : x * leaky_relu_alpha;
	}
	static double d(double y, double &li) {
		return y <= 0 ? leaky_relu_alpha : 1;
	}
	static void s(FILE *file) {
		fbputc(__LEAKY_RELU__, file)
	}
};
class r_relu {
public:
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
		fbputc(__R_RELU__, file)
	}
};
class hard_swish {
public:
	static double f(double x, double &li) {
		li = x;
		return x * std::min(6.0, std::max(0.0, x + 3.0)) / 6.0;
	}
	static double d(double y, double &li) {
		return (std::min(6.0, std::max(0.0, li + 3.0))
				+ li * (li + 3.0 > 6.0 ? 0.0 : li + 3 < 0.0 ? 0.0 : 1.0)) / 6.0;
	}
	static void s(FILE *file) {
		fbputc(__HARD_SWISH__, file)
	}
};
class Mish {
public:
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
		fbputc(__MISH__, file)
	}
};
//swish not fully implemented
class swish {
public:
	static double f(double x, double &li) {
		li = x;
		return x * (1.0 / (1.0 + exp(-x)));
	}
	static double d(double y, double &li) {
		return y + (1 - y) * (1.0 / (1.0 + exp(-li)));
	}
	static void s(FILE *file) {
		fbputc(__SWISH__, file)
	}
};

}

namespace LossFunc {
class func {
	d2dfunc2 *_f = NULL;
public:
	sfunc *_s = NULL;
	func() {
	}
	func(d2dfunc2 *fi, sfunc *si) {
		_f = fi;
		_s = si;
	}
	double f(double y, double a) {
		return _f(y, a);
	}
	void save(FILE *file) {
		_s(file);
	}
};
static inline double quad(double y, double a) {
	return a - y;
}
static inline double cross_entropy(double y, double a) {
	return (y - a) / (1 - a) / a;
}
static inline void quad_s(FILE *file) {
	fbputc(__QUAD__, file)
}
static inline void cross_entropy_s(FILE *file) {
	fbputc(__CROSS_ENTROPY__, file)
}
}

const LossFunc::func empty_loss_func;

#endif /* FUNCTIONS_HPP_ */
