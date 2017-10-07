#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

void feedInput(Network *nn, Vector *v) {
	// feed the input to the first layer
	Layer *inputLayer = nn->layers;
	Node *curNode = inputLayer->nodes;
	//printf("yo %d\n", v->size);
	for (int i = 0;i < v->size;i++) {
		//printf("yo %f\n", v->vals[i]);
		curNode->output = v->vals[i];
		curNode ++;
	}
}

double activate(double val){
	return 1.0/(1.0 + exp(-1.0*val));
}

double derivative(double val){
	double eval = exp(val);
	return eval/((1+eval)*(1+eval));
	//return val*(1-val);
}

double linearSum(Node *curNode, Layer *prevLayer, int nodeCount) {
	double sum = 0;
	Node *node = prevLayer->nodes;
	for (int i = 0;i < nodeCount;i++) {
		sum += (curNode->weights[i])*(node->output);
		node = (Node *)((uint8_t *)(node) + node->size);
	}
	return sum;
}

void forwardPropagate(Network *nn) {
	// move forward on the network and calculate each layer's output
	Layer *inputLayer = nn->layers;
	Layer *hiddenLayer = (Layer *)((uint8_t *)(inputLayer) + inputLayer->size);
	Layer *outputLayer = (Layer *)((uint8_t *)(hiddenLayer) + hiddenLayer->size);
	int input_node_count = inputLayer->node_count;
	int hidden_node_count = hiddenLayer->node_count;
	int output_node_count = outputLayer->node_count;
	Node *hidden_node = hiddenLayer->nodes;
	for (int i =0;i < hidden_node_count;i++) {
		double sum = linearSum(hidden_node, inputLayer,input_node_count)+hidden_node->bias;
		hidden_node->output = activate(sum);
		hidden_node = (Node *)((uint8_t *)(hidden_node) + hidden_node->size);
	}

	Node *output_node = outputLayer->nodes;
	for (int i =0;i < output_node_count;i++) {
		double sum = linearSum(output_node, hiddenLayer, hidden_node_count)+output_node->bias;
		output_node->output = activate(sum);
		output_node = (Node *)((uint8_t *)(output_node) + output_node->size);
	}
}


void backwardPropagate(Network *nn, Vector *expected) {
	// go through each layer and backprop
	Layer *inputLayer = nn->layers;
	Layer *hiddenLayer = (Layer *)((uint8_t *)(inputLayer) + inputLayer->size);
	Layer *outputLayer = (Layer *)((uint8_t *)(hiddenLayer) + hiddenLayer->size);
	int input_node_count = inputLayer->node_count;
	int hidden_node_count = hiddenLayer->node_count;
	int output_node_count = outputLayer->node_count;

	// output error
	Node *output_node = outputLayer->nodes;
	for (int i =0;i < output_node_count;i++) {
		//output_node->error = -1*(output_node->output - 
		//	expected->vals[i])*derivative(linearSum(output_node, hiddenLayer,hidden_node_count));
		output_node->error = (expected->vals[i]
			- output_node->output)* output_node->output*(1-output_node->output);
		output_node = (Node *)((uint8_t *)(output_node) + output_node->size);
	}

	// hidden error
	Node *hidden_node = hiddenLayer->nodes;
	for (int i =0;i < hidden_node_count;i++) {
		//double fPrime = derivative(linearSum(hidden_node,inputLayer,input_node_count));
		double fPrime = hidden_node->output*(1-hidden_node->output);
		double sum = 0;
		Node *node = outputLayer->nodes;
		for (int j = 0;j < output_node_count;j++) {
			sum += (node->error)*((node->weights)[i]);
			node = (Node *)((uint8_t *)(node) + node->size);
		}
		hidden_node->error = fPrime*sum;
		hidden_node = (Node *)((uint8_t *)(hidden_node) + hidden_node->size);
	}


}

void updateWeights(Network *nn) {
	double learning_rate = nn->learning_rate;
	Layer *inputLayer = nn->layers;
	Layer *hiddenLayer = (Layer *)((uint8_t *)(inputLayer) + inputLayer->size);
	Layer *outputLayer = (Layer *)((uint8_t *)(hiddenLayer) + hiddenLayer->size);
	int input_node_count = inputLayer->node_count;
	int hidden_node_count = hiddenLayer->node_count;
	int output_node_count = outputLayer->node_count;
	Node *hidden_node = hiddenLayer->nodes;
	for (int i =0;i < hidden_node_count;i++) {
		hidden_node->bias += learning_rate*hidden_node->error;
		//hidden_node->bias = 0;
		Node *node = inputLayer->nodes;
		for (int j = 0;j < input_node_count;j++) {
			hidden_node->weights[j] += 
			    learning_rate*node->output*hidden_node->error;
			node = (Node *)((uint8_t *)(node) + node->size);
		}
		hidden_node = (Node *)((uint8_t *)(hidden_node) + hidden_node->size);
	}

	Node *output_node = outputLayer->nodes;
	for (int i =0;i < output_node_count;i++) {
		output_node->bias += learning_rate*output_node->error;
		//output_node->bias = 0;
		Node *node = hiddenLayer->nodes;
		for (int j = 0;j < hidden_node_count;j++) {
			output_node->weights[j] += 
			    learning_rate*node->output*output_node->error;
			node = (Node *)((uint8_t *)(node) + node->size);
		}
		output_node = (Node *)((uint8_t *)(output_node) + output_node->size);
	}
}


