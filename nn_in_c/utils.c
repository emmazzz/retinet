#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void feedInput(Network *nn, Vector *v) {
	// feed the input to the first layer
	Layer *inputLayer = nn->layers;
	printf("heyyy\n");
	Node *curNode = inputLayer->nodes;
	for (int i = 0;i < v->size;i++) {
		printf("yyyy\n");
		fflush(stdout);
		curNode->output = v->vals[i];
		printf("zzzz\n");
		curNode ++;
	}
}

void feedForwardNetwork(Network *nn) {
	// move forward on the network and calculate each layer's output
}

void backPropagateNetwork(Network *nn, int target) {
	// go through each layer and backprop
}


