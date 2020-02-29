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

#include "ANNConstantDef.h"

typedef double d2dfunc(double, double&);

namespace StepFunc {
//li refers to last_in
class func {
private:
	double li, lo;
	d2dfunc *_f, *_d;
public:
	func(d2dfunc *fi, d2dfunc *di) {
		li = lo = 0.0;
		_f = fi, _d = di;
	}
	double f(double x) {
		return (*_f)(x, li);
	}
	double d(double y) {
		return (*_d)(y, li);
	}
};
class sigmoid {
public:
	double f(double x, double& li) {
		return 1.0 / (1.0 + exp(-x));
	}
	double d(double y, double& li) {
		return y * (1.0 - y);
	}
};
class tanh {
public:
	double f(double x, double& li) {
		return std::tanh(x);
	}
	double d(double y, double& li) {
		return 1.0 - y * y;
	}
};
class hard_sigmond {
public:
	double f(double x, double& li) {
		li = x;
		return std::max(0.0, std::min(1.0, x / 5.0 + 0.5));
	}
	double d(double y, double& li) {
		return li < -2.5 ? 0.0 : (li > 2.5 ? 0.0 : 0.2);
	}
};
class relu {
public:
	double f(double x, double& li) {
		return std::max(0.0, x);
	}
	double d(double y, double& li) {
		return y > 0.0 ? 1.0 : 0.0;
	}
};
class relu6 {
public:
	double f(double x, double& li) {
		return std::min(6.0, std::max(0.0, x));
	}
	double d(double y, double& li) {
		return y > 6.0 ? 0.0 : y < 0.0 ? 0.0 : 1.0;
	}
};
class elu {
public:
	double f(double x, double& li) {
		li = x;
		return x > 0.0 ? x : (exp(x) - 1.0) * elu_alpha;
	}
	double d(double y, double& li) {
		return li > 0.0 ? 1.0 : f(li, li) + elu_alpha;
	}
};
class selu {
public:
	double f(double x, double& li) {
		li = x;
		return selu_scale * (x > 0.0 ? x : (exp(x) - 1.0) * selu_alpha);
	}
	double d(double y, double& li) {
		return selu_scale * (li > 0.0 ? 1 : f(li, li) + selu_alpha);

	}
};
class leaky_relu {
public:
	double f(double x, double& li) {
		return x > 0 ? x : x * leaky_relu_alpha;
	}
	double d(double y, double& li) {
		return y <= 0 ? leaky_relu_alpha : 1;
	}
};
class r_relu {
public:
	double f(double x, double& li) {
#ifdef _train_
		li=r_relu_r;
#elif _test_
		li=r_relu_test;
#endif
		return x > 0 ? x : x * li;
	}
	double d(double y, double& li) {
		return y > 0 ? 1 : li;
	}
};
class hard_swish {
public:
	double f(double x, double& li) {
		li = x;
		return x * std::min(6.0, std::max(0.0, x + 3.0)) / 6.0;
	}
	double d(double y, double& li) {
		return (std::min(6.0, std::max(0.0, li + 3.0))
				+ li * (li + 3.0 > 6.0 ? 0.0 : li + 3 < 0.0 ? 0.0 : 1.0)) / 6.0;
	}
};
class Mish {
public:
	double f(double x, double& li) {
		li = x;
		return x * std::tanh(log(1 + exp(x)));
	}
	double d(double y, double& li) {
		return exp(li)
				* (4.0 * (li + 1.0) + 4.0 * exp(2.0 * li) + exp(3.0 * li)
						+ exp(li) * (4.0 * li + 6))
				/ (2 * exp(li) + exp(2.0 * li) + 2)
				/ (2 * exp(li) + exp(2.0 * li) + 2);
	}
};
//swish not fully implemented
class swish {
public:
	double f(double x, double& li) {
		li = x;
		return x * (1.0 / (1.0 + exp(-x)));
	}
	double d(double y, double& li) {
		return y + (1 - y) * (1.0 / (1.0 + exp(-li)));
	}
};

}

namespace LossFunc {
class func {
public:
	d2dfunc *_f;
	func(d2dfunc *fi) {
		_f = fi;
	}
	double f(double y, double a) {
		return _f(y, a);
	}
};
inline double quad(double y, double a) {
	return a - y;
}
inline double cross_entropy(double y, double a) {
	return (y - a) / (1 - a) / a;
}
}

#endif /* FUNCTIONS_HPP_ */
