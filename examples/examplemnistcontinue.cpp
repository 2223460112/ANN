#include "../lib/Net.hpp"
int main() {
	system("clear");
	printf("\033[?25l");

	FILE *fi = fopen("./mnistnetout", "rb");
	Net DNN(fi,0.003);
	fclose(fi);

	Matrix<double, 28, 28> IM;
	Matrix<double, 10, 1> OM;
	Matrix<double, 10, 1> TM;

	FILE *fto = fopen("mnistlossout.csv", "wb");

	double ter = 0;
	FILE *ftraini = fopen("../../traindata/mnist/train_images", "rb");
	FILE *ftrainl = fopen("../../traindata/mnist/train_labels", "rb");
	printf("\033[42m\033[30m");
	printf("patch:    000.0%%                        \n");
	for (uint32_t t = 0; t < 60000; t++) {
		uint32_t RA = getc(ftrainl);
		for (uint32_t i = 0; i < 28; i++)
			for (uint32_t j = 0; j < 28; j++)
				IM(i, j) = (double) getc(ftraini) / 255.0;
		for (uint32_t i = 0; i < 10; i++)
			TM(i, 0) = 0;
		TM(RA, 0) = 1;
		OM = DNN.calc(IM);
		DNN.train(TM);
		DNN.calcdW();
		DNN.modify();
		for (uint32_t i = 0; i < 10; i++)
			ter += abs(TM(i, 0) - OM(i, 0));
		if (t % 20 == 0) {
			printf("\033[1A");
			printf("patch:    %5.1lf%%    total loss:%8lf\n",
					(double) t / 600.0, ter / t);
			fprintf(fto, "%lf,", ter / t);
		}

	}
	fclose(ftraini);
	fclose(ftrainl);

	ter = 0;
	uint32_t RC=0;
	FILE *ftesti = fopen("../../traindata/mnist/test_images", "rb");
	FILE *ftestl = fopen("../../traindata/mnist/test_labels", "rb");
	printf("\033[43m\033[30m");
	printf("patch:    000.0%%                        \n");
	for (uint32_t t = 0; t < 10000; t++) {
		if (t % 50 == 0) {
			printf("\033[1A");
			printf("patch:    %5.1lf%%                        \n",
					(double) t / 100.0);
		}
		uint32_t RA = getc(ftestl);
		for (uint32_t i = 0; i < 28; i++)
			for (uint32_t j = 0; j < 28; j++)
				IM(i, j) = (double) getc(ftesti) / 255.0;
		for (uint32_t i = 0; i < 10; i++)
			TM(i, 0) = 0;
		TM(RA, 0) = 1;
		OM = DNN.calc(IM);
		for (uint32_t i = 0; i < 10; i++)
			ter += abs(TM(i, 0) - OM(i, 0));
		uint32_t MA=0;
		for (uint32_t i = 1; i < 10; i++)
			if(abs(1 - OM(i, 0))<abs(1 - OM(MA, 0)))MA=i;
		if(MA==RA)RC++;
	}
	fclose(ftesti);
	fclose(ftestl);

	printf("\033[1A");
	printf("result:  %8lf    %5d              \n", ter / 10000,RC);

	fprintf(fto, "%lf", ter / 10000);
	fclose(fto);

	FILE *f = fopen("./mnistnetout", "wb");
	DNN.save(f);
	fclose(f);

	printf("\033[0mDone                 \n");
	printf("\033[?25h");
	return 0;
}
