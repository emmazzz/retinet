#include <stdio.h>

double hidOut[500];

int main() {
	for (int i = 0;i < 500;i++) {
		// activate
		if (input[i] > 0) {
			hidOut[i] = input[i];
		} else {
			hidOut[i] = -1*input[i];
		}
	}

	double maxProb = 0;
	int res = 9;

	for (int i = 0;i < 9;i++) {
		// printf("%f\n", outputWeights[i][0]);
		double sum = outputBias[i];
		for (int j = 0;j < 500;j++) {
			sum += outputWeights[i][j]*hidOut[j];
		}
		
		if (sum > maxProb) {
			maxProb = sum;
			res = i;
		}

	}
	
	printf("result is %d\n" ,res);

}