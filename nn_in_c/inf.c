#include "inf.h"
#include "read.h"
#include <stdio.h>
#include <math.h>

double input[724];
double hidOut[20];


int main() {

	// write into input
	FILE *f = fopen("processed","r");
	for (int i = 0; i < 28*28; ++i)
	{
		int tmp, bla;
		fread(&tmp, 4, 1, f);
		fread(&bla, 4, 1, f);
		//double temp =(double) fgetc(f);
		double temp = (double) tmp;
		printf("%f\n", temp);
		input[i] = temp/256.0;
		
	}
	



	// emma

	// load_minist_init();
	// int numC = 0;
	// for (int k = 0;k < 10000;k++) {
	// 	Vector *inp = getNextImage();
	// 	int label = getNextLabel();
	// 	if (k < 8000) continue;
	// 	for (int i = 0;i < 724;i++) {
	// 		input[i] = inp->vals[i];
	// 	}

	for (int i = 0;i < 70;i++) {
		double sum = hiddenWeights[i][0];
		for (int j = 1;j < 785;j++) {
			sum += hiddenWeights[i][j]*input[j-1];
		}
		// activate
		hidOut[i] = 1.0/(1.0 + exp(-1.0*sum));
	}

	double maxProb = 0;
	int res = 0;

	for (int i = 0;i < 10;i++) {
		double sum = outputWeights[i][0];
		for (int j = 1;j < 71;j++) {
			sum += outputWeights[i][j]*hidOut[j-1];
		}
		// activate
		sum = 1.0/(1.0 + exp(-1.0*sum));
		printf("%d: prob = %f\n", i, sum);
		if (sum > maxProb) {
			maxProb = sum;
			res = i;
		}

	}

		
	// 	if (res == label) numC ++;

	printf("result is %d\n" ,res);
	// }

	// printf("accuracy %f\n", numC/2000.0);

	
}