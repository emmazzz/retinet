#include "inf.h"
#include <stdio.h>
#include <math.h>

#define CAPSTONE_HLS

double inf_input[784];
double hidOut[40];
#ifdef CAPSTONE_HLS
double tmp[41];
#endif
double probs[10];

int main() {

	int i, j;
	// write into input
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

	// printf("result is %d\n" ,res);

	// Show accuracy if under multiple runs
	// printf("accuracy %f\n", numC/2000.0);

	return res;
}
