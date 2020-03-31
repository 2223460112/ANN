#include "../lib/Net.hpp"
int main() {
	uint32_t Layerstag[] = { __DNN_INNER__, __DNN_INNER__, __DNN_OUTPUT__ };
	uint32_t LitM[] = { 0, 1, 2 };
	uint32_t LotM[] = { 1, 2, 3 };
	std::pair<uint32_t, uint32_t> Mrect[] = { std::make_pair(28, 28),
			std::make_pair(22, 22), std::make_pair(18, 18), std::make_pair(10,
					1) };
	uint32_t S[3] = { __SWISH__, __SWISH__, __SIGMOID__ };
	LossFunc::func L(__CROSS_ENTROPY__);
	Net DNN(3, Layerstag, 4, LitM, LotM, Mrect, S, L, 0.01);

	FILE *f = fopen("./mnistnetout", "wb");
	DNN.save(f);
	fclose(f);

	return 0;
}
