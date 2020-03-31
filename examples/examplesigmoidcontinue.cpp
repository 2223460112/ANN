#include <cstdio>
#include "../lib/Net.hpp"
using namespace std;
double tbf(double y) {
	double x = 2 * y;
	return -0.9 + 0.122592525544741 * sin(x) + 0.430783383039806 * sinh(x)
			- 0.56304347826087 * x * x + 1.530434782608697 * pow(2.0, -x)
			+ 4.682744405392363 * tan(x / 8.0);
}
#define tbfb (-2.0)
#define tbft 1.6
int main() {

	system("clear");
	printf("\033[?25l");


	FILE *fi = fopen("./1_sigmoidnetout", "rb");
	Net beastDNN(fi);
	fclose(fi);

	FILE *fto = fopen("1_sigmoidtest.csv", "wb");

	Matrix<double, 1, 1> IM, OM, TM;
	for (uint32_t t = 0; t < 20; t++) {
		printf("\033[44m\033[30m");
		printf("turn %3d start       \n", t);
		printf("\033[42m\033[30m");
		for (uint32_t p = 0; p < 25; p++) {
			printf("    patch %d: 000.0%%\n", p);
			for (uint32_t pos = 1; pos <= 1000; pos++) {
				if (pos % 50 == 0) {
					printf("\033[1A");
					printf("    patch %3d: %5.1lf%%\n", p, (double) pos / 10.0);
				}
				IM(0, 0) = (tbft - tbfb) * (double) rand() / RAND_MAX + tbfb;
				OM(0, 0) = tbf(IM(0, 0));
				beastDNN.calc(IM);
				beastDNN.train(OM);
				beastDNN.calcdW();
				beastDNN.modify();
			}
			printf("\033[1A");
		}
		putchar('\n');
		printf("\033[43m\033[30m");
		double terp2 = 0;
		for (uint32_t pos = 1; pos <= 10000; pos++) {
			if (pos % 50 == 0) {
				printf("\033[1A");
				printf("test %5.1lf%%          \n", (double) pos / 100.0);
			}
			IM(0, 0) = (tbft - tbfb) * (double) pos / 10000.0 + tbfb;
			OM(0, 0) = tbf(IM(0, 0));
			TM = beastDNN.calc(IM);
			fprintf(fto, "%0.8lf,", TM(0, 0));
			terp2 += (TM(0, 0) - OM(0, 0)) * (TM(0, 0) - OM(0, 0));
		}
		putc('\n', fto);
		printf("\033[1A");
		printf("result: %6.7lf\n", terp2/10000.0);
	}
	FILE *f = fopen("./1_sigmoidnetout", "wb");
	beastDNN.save(f);
	fclose(f);
	fclose(fto);

	printf("\033[0mDone                 \n");
	return 0;
}
