#include "inf.h"
#include "read.h"
#include <stdio.h>
#include <math.h>

double input[724];
double hidOut[20];


int main() {

	load_minist_init();
	int numC = 0;
	for (int k = 0;k < 10000;k++) {
		Vector *inp = getNextImage();
		int label = getNextLabel();
		if (k < 8000) continue;
		for (int i = 0;i < 724;i++) {
			input[i] = inp->vals[i];
		}

		for (int i = 0;i < 20;i++) {
			double sum = hiddenWeights[i][0];
			for (int j = 1;j < 725;j++) {
				sum += hiddenWeights[i][j]*input[j-1];
			}
			// activate
			hidOut[i] = 1.0/(1.0 + exp(-1.0*sum));
		}

		double maxProb = 0;
		int res = 0;

		for (int i = 0;i < 10;i++) {
			double sum = outputWeights[i][0];
			for (int j = 1;j < 21;j++) {
				sum += outputWeights[i][j]*hidOut[j-1];
			}
			// activate
			sum = 1.0/(1.0 + exp(-1.0*sum));
			if (sum > maxProb) {
				maxProb = sum;
				res = i;
			}

		}

		
		if (res == label) numC ++;

		printf("label is %d result is %d\n", label,res);
	}

	printf("accuracy %f\n", numC/2000.0);

	
}