#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

void feedInput(Network *nn, Vector *v) {
	// feed the input to the first layer
	Layer *inputLayer = nn->layers;
	Node *curNode = inputLayer->nodes;
	for (int i = 0;i < v->size;i++) {
		curNode->output = v->vals[i];
		curNode ++;
	}
}

double activate(double val){
	return 1.0/(1.0 + exp(-1*val));
}

double derivative(double val){
	double eval = exp(val);
	return eval/((1+eval)*(1+eval));
}

void forwardPropagate(Network *nn, double bias) {
	// move forward on the network and calculate each layer's output
	int num_layer = nn->layer_count;
	Layer *inputLayer = nn->layers;
	Layer *hiddenLayer = (Layer *)((uint8_t *)(inputLayer) + inputLayer->size);
	Layer *outputLayer = (Layer *)((uint8_t *)(hiddenLayer) + hiddenLayer->size);
	int input_node_count = inputLayer->node_count;
	int hidden_node_count = hiddenLayer->node_count;
	int output_node_count = outputLayer->node_count;
	Node *hidden_node = hiddenLayer->nodes;
	for (int i =0;i < hidden_node_count;i++) {
		double sum = 0;
		Node *node = inputLayer->nodes;
		for (int j = 0;j < input_node_count;j++) {
			sum += (hidden_node->weights[j])*(node->output);
			node = (Node *)((uint8_t *)(node) + node->size);
		}
		hidden_node->output = activate(sum);
		hidden_node = (Node *)((uint8_t *)(hidden_node) + hidden_node->size);
	}

	Node *output_node = outputLayer->nodes;
	for (int i =0;i < output_node_count;i++) {
		double sum = 0;
		Node *node = hiddenLayer->nodes;
		for (int j = 0;j < hidden_node_count;j++) {
			sum += (output_node->weights[j])*(node->output);
			node = (Node *)((uint8_t *)(node) + node->size);
		}
		output_node->output = activate(sum);
		output_node = (Node *)((uint8_t *)(output_node) + output_node->size);
	}
}

void backPropagateNetwork(Network *nn, int target) {
	// go through each layer and backprop
}


