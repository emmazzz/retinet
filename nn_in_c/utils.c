#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void feedInput(Network *nn, Vector *v) {
	// feed the input to the first layer
	Layer *inputLayer = nn->layers;
	Node *curNode = inputLayer->nodes;
	for (int i = 0;i < v->size;i++) {
		fflush(stdout);
		curNode->output = v->vals[i];
		curNode ++;
	}
}


void forwardPropagate(Network *nn) {
	// move forward on the network and calculate each layer's output
	
}

void backPropagateNetwork(Network *nn, int target) {
	// go through each layer and backprop
}


