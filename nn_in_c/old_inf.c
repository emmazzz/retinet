#include "inf.h"
#include <stdio.h>
#include <math.h>

<<<<<<< HEAD
#define CAPSTONE_HLS
=======
>>>>>>> 021f73cd03ec45b6fa62c6ce4296dc823a46c1fa

double inf_input[784];
double hidOut[40];
#ifdef CAPSTONE_HLS
double tmp[41];
#endif
double probs[10];

<<<<<<< HEAD
int main() {
=======

#define CAPSTONE_HLS

// int main() {

// 	int i, j;
// 	// write into input

	// for (int label = 0; label < 10; ++label)
	// {
	// 	// int i = 9;
	// 	char file[40];
	// 	sprintf(file, "processed%d",label);
	// 	//printf("%s\n", file);
	// 	FILE *f = fopen(file,"r");

	// 	for (int i = 0; i < 28*28; ++i)
	// 	{
	// 		int tmp, bla;
	// 		fread(&tmp, 4, 1, f);
	// 		fread(&bla, 4, 1, f);
	// 		//double temp = (double) fgetc(f);
	// 		double temp = (double) tmp;
	// 		//printf("%f\n", temp);
	// 		input[i] = temp/256.0;
			
	// 	}
	// 	for (int i = 0;i < 70;i++) {
	// 		double sum = hiddenWeights[i][0];
	// 		for (int j = 1;j < 785;j++) {
	// 			sum += hiddenWeights[i][j]*input[j-1];
	// 		}
	// 		// activate
	// 		hidOut[i] = 1.0/(1.0 + exp(-1.0*sum));
	// 	}

	// 	double maxProb = 0;
	// 	int res = 0;
	// 	for (int i = 0;i < 10;i++) {
	// 		double sum = outputWeights[i][0];
	// 		for (int j = 1;j < 71;j++) {
	// 			sum += outputWeights[i][j]*hidOut[j-1];
	// 		}
	// 		// activate
	// 		sum = 1.0/(1.0 + exp(-1.0*sum));
	// 		//printf("%d: prob = %f\n", i, sum);
	// 		if (sum > maxProb) {
	// 			maxProb = sum;
	// 			res = i;
	// 		}
	// 	}	

	// 	printf("Should be %d\n", label);
	// 	printf("result is %d\n\n" ,res);

	// }
	



	// emma

	// load_mnist_init_test();
	// int numC = 0;
	// for (int k = 0;k < 2000;k++) {
	// 	//printf("%d\n", k);
	// 	Vector *inp = getNextImageTest();
	// 	int label = getNextLabelTest();
	// 	//if (k < 8000) continue;
	// 	for (int i = 0;i < 784;i++) {
	// 		input[i] = inp->vals[i];
	// 	}

	// 	for (int i = 0;i < 70;i++) {
	// 		double sum = hiddenWeights[i][0];
	// 		for (int j = 1;j < 785;j++) {
	// 			sum += hiddenWeights[i][j]*input[j-1];
	// 		}
	// 		// activate
	// 		hidOut[i] = 1.0/(1.0 + exp(-1.0*sum));
	// 	}

	// 	double maxProb = 0;
	// 	int res = 0;

	// 	for (int i = 0;i < 10;i++) {
	// 		double sum = outputWeights[i][0];
	// 		for (int j = 1;j < 71;j++) {
	// 			sum += outputWeights[i][j]*hidOut[j-1];
	// 		}
	// 		// activate
	// 		sum = 1.0/(1.0 + exp(-1.0*sum));
	// 		printf("%d: prob = %f\n", i, sum);
	// 		if (sum > maxProb) {
	// 			maxProb = sum;
	// 			res = i;
	// 		}

	// 	}

		
	// 	if (res == label) numC ++;

	// 	printf("result is %d\n" ,res);
	// }
// 	return res;
// }

>>>>>>> 021f73cd03ec45b6fa62c6ce4296dc823a46c1fa

#ifdef CAPSTONE_HLS
	for (i = 0; i < 28*28; i++) {
#pragma HLS UNROLL
		inf_input[i] = i / 256.0;
	}
#else
	FILE *f = fopen("processed","r");
	for (i = 0; i < 28*28; ++i)
	{
		int tmp, bla;
		fread(&tmp, 4, 1, f);
		fread(&bla, 4, 1, f);
		double temp = (double) tmp;
		printf("%f\n", temp);
		input[i] = temp/256.0;
	}
#endif

	for (i = 0;i < 40;i++) {
		double sum = hiddenWeights[i][0];
		for (int j = 1;j < 785;j++) {
#pragma HLS UNROLL
			sum += hiddenWeights[i][j]*inf_input[j-1];
		}
		// activate
		hidOut[i] = 1.0/(1.0 + exp(-1.0*sum));
	}

	double maxProb = 0;
	int res = 0;

	for (i = 0;i < 10;i++) {
#pragma HLS UNROLL
		double sum = outputWeights[i][0];
		for (j = 1;j < 41;j++) {
#pragma HLS UNROLL
			tmp[i] = outputWeights[i][j]*hidOut[j-1];
		}
		for (j = 1; j < 41; j++) {
			sum += tmp[i];
		}
		// activate
		sum = 1.0/(1.0 + exp(-1.0*sum));
//		printf("%d: prob = %f\n", i, sum);
		probs[i] = sum;
	}

	for (i = 0; i < 10; i++) {
		if (probs[i] > maxProb) {
			maxProb = probs[i];
			res = i;
		}
	}



