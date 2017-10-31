#define CAPSTONE_HLS

#include "inf.hpp"
#include <stdio.h>
//#include <math.h>
#include <stdint.h>
//#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <hls_math.h>

#ifdef CAPSTONE_HLS
#include <ap_fixed.h>
typedef ap_fixed<32, 16, AP_RND_CONV, AP_SAT> my_float;
#else
#define my_float float
#endif

my_float inf_input[784];
my_float hidOut[40];
// stores intermediate values for synth to succeed
my_float tmp[41];
my_float tmp2[785];
my_float probs[10];
// endof

int main() {

	int i, j;
	my_float exp_m1_sum;

	// write into input
#ifdef CAPSTONE_HLS

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
#pragma HLS PIPELINE off
		my_float sum = hiddenWeights[i][0];
		for (j = 1;j < 785;j++) {
#pragma HLS UNROLL
			my_float temp_double = inf_input[j-1];
			my_float temp_double2 = hiddenWeights[i][j];
			tmp2[j] = temp_double2*temp_double;
		}

		for (j = 1; j < 785; j++) {
#pragma HLS LOOP_FLATTEN off
			my_float temp_double3 = tmp2[j];
			sum = sum + temp_double3;
		}

		// activate
		// break down expression
		exp_m1_sum = exp((my_float)(-1.0) * sum);
		my_float temp_float4_0 = (my_float)1.0 + exp_m1_sum;
		my_float temp_float4 = (my_float)(1.0)/temp_float4_0;
		hidOut[i] = temp_float4;
	}

	my_float maxProb = 0.f;
	int res = 0;

	for (i = 0;i < 10;i++) {
#pragma HLS UNROLL
		my_float sum = outputWeights[i][0];
		for (j = 1;j < 41;j++) {
#pragma HLS UNROLL
#pragma HLS RESOURCE variable=tmp core=DSP48
			tmp[i] = outputWeights[i][j]*hidOut[j-1];
		}
		for (j = 1; j < 41; j++) {
			sum += tmp[i];
		}
		// activate
		// break down expression
		exp_m1_sum = exp((my_float)(-1.0) * sum);
		my_float temp_float5_0 = (my_float)1.0 + exp_m1_sum;
		my_float temp_float5 = (my_float)(1.0)/temp_float5_0;
		probs[i] = temp_float5;
	}

	for (i = 0; i < 10; i++) {
		if (probs[i] > maxProb) {
			maxProb = probs[i];
			res = i;
		}
	}

	return res;
}
