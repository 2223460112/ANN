#include <cstdio>
#include "../lib/Net.hpp"
using namespace std;
double tbf(double x) {
	return -0.9 + 0.122592525544741 * sin(x) + 0.430783383039806 * sinh(x)
			- 0.56304347826087 * x * x + 1.530434782608697 * pow(2.0, -x)
			+ 4.682744405392363 * tan(x / 8.0);
}
#define tbfb (-4.0)
#define tbft 3.2
int main() {

	system("clear");
	printf("\033[?25l");

	uint32_t Layerstag[] = { __DNN_INNER__, __DNN_INNER__, __DNN_OUTPUT__ };
	uint32_t LitM[] = { 0, 1, 2 };
	uint32_t LotM[] = { 1, 2, 3 };
	std::pair<uint32_t, uint32_t> Mrect[] = { std::make_pair(1, 1),
			std::make_pair(4, 4), std::make_pair(4, 4), std::make_pair(1, 1) };
	uint32_t S[3]={__LEAKY_RELU__,__LEAKY_RELU__,__LEAKY_RELU__};
	LossFunc::func L(__QUAD__);
	Net beastDNN(3, Layerstag, 4, LitM, LotM, Mrect, S, L, 0.01);

	FILE *f = fopen("./1_relunetout", "wb");
	beastDNN.save(f);
	fclose(f);

	printf("\033[?25h");
	printf("Done\n");
	return 0;
}
