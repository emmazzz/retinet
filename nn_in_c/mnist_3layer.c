#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "read.h"
#include "utils.h"

#define NUM_DIGITS 10



Network *createNeuralNetwork(inputSize,hiddenSize, outputSize) {
	// allocate appropriate size of memory
    int inputNodeSize = sizeof(Node);         
    int inputLayerSize = sizeof(Layer) + (inputSize * inputNodeSize);
    int hiddenNodeSize = sizeof(Node) + (inputSize * sizeof(double));
    int hiddenLayerSize = sizeof(Layer) + (hiddenSize * hiddenNodeSize);
    int outputNodeSize = sizeof(Node) + (hiddenSize * sizeof(double));
    int outputLayerSize = sizeof(Layer) + (outputSize * outputNodeSize);
    
    Network *nn = (Network*)malloc(sizeof(Network)+inputLayerSize 
    	+ hiddenLayerSize + outputLayerSize);
    nn->learning_rate = 0.7;

    // initialize input layer
    //nn->layers = (Layer *)malloc(inputLayerSize+hiddenLayerSize + outputLayerSize);
    Layer *inputLayer = (Layer *)malloc(inputLayerSize);
    inputLayer->node_count = inputSize;
    inputLayer->size = inputLayerSize;
    // init nodes
    //inputLayer->nodes = (Node *)malloc(sizeof(inputNodeSize)*inputSize);
    uint8_t *curNode = (uint8_t *)(inputLayer->nodes);
    for (int i = 0;i < inputSize;i++) {
    	//Node *node = (Node *)malloc(inputNodeSize);
    	((Node *)curNode)->size = inputNodeSize;
    	//memcpy(curNode,node,inputNodeSize);
    	curNode += inputNodeSize;

    }
    memcpy(nn->layers,inputLayer,inputLayerSize);
    free(inputLayer);

    // initialize hidden layer
    //Layer *hiddenLayer = (Layer *)((uint8_t *)inputLayer + inputLayerSize);
    Layer *hiddenLayer = (Layer *)malloc(hiddenLayerSize);
    // init nodes
    //hiddenLayer->nodes = (Node *)malloc(sizeof(hiddenNodeSize)*hiddenSize);
    hiddenLayer->node_count = hiddenSize;
    hiddenLayer->size = hiddenLayerSize;
    curNode = (uint8_t *)hiddenLayer->nodes;
    for (int i = 0;i < hiddenSize;i++) {
    	// Node node;
    	// node.size = hiddenNodeSize;
    	// node.weight_count = inputSize;
    	// node.output = i;
    	// memcpy(curNode,&node,hiddenNodeSize);
    	((Node *)curNode)->size = hiddenNodeSize;
    	((Node *)curNode)->weight_count = inputSize;
    	((Node *)curNode)->output = i;
    	curNode += hiddenNodeSize;
    }
    uint8_t *ptr = ((uint8_t *)nn->layers) + inputLayerSize;
    memcpy(ptr,hiddenLayer,hiddenLayerSize);
    free(hiddenLayer);

    // initialize output layer
    // Layer *outputLayer = (Layer *)((uint8_t *)hiddenLayer + hiddenLayerSize);
    Layer *outputLayer = (Layer *)malloc(outputLayerSize);
    // init nodes
    //outputLayer->nodes = (Node *)malloc(sizeof(outputNodeSize)*outputSize);
    outputLayer->node_count = outputSize;
    curNode = (uint8_t *)outputLayer->nodes;
    for (int i = 0;i < outputSize;i++) {
    	//curNode = (Node *)malloc(outputNodeSize);
    	((Node *)curNode)->size = outputNodeSize;
    	((Node *)curNode)->weight_count = hiddenSize;
    	((Node *)curNode)->output = i;

    	curNode = ((uint8_t *)curNode + outputNodeSize);
    }
    ptr += hiddenLayerSize;
    memcpy(ptr,outputLayer,outputLayerSize);
    return nn;

}

void initWeights(Network *nn) {
	srand(time(NULL));
	uint8_t *layer = (uint8_t *)nn->layers + nn->layers->size;
	int node_count = ((Layer *)layer)->node_count;
	uint8_t *ptr = (uint8_t *)(((Layer *)layer)->nodes);
	for (int j = 0;j < node_count;j++) {
		Node *node = (Node *)ptr;
		node->bias = rand()/(double)(RAND_MAX);
		if (j % 2) node->bias = -1*node->bias;
		for (int i = 0;i < node->weight_count;i++) {
			node->weights[i] = 0.7*(rand()/(double)(RAND_MAX));
			if (i%2) node->weights[i] = -1*node->weights[i];
		}
		ptr += node->size;
	}

	layer += ((Layer *)layer)->size;
	node_count = ((Layer *)layer)->node_count;
	ptr = (uint8_t *)(((Layer *)layer)->nodes);
	for (int j = 0;j < node_count;j++) {
		Node *node = (Node *)ptr;
		node->bias = rand()/(double)(RAND_MAX);
		if (j % 2) node->bias = -1*node->bias;
		for (int i = 0;i < node->weight_count;i++) {
			node->weights[i] = 0.7*(rand()/(double)(RAND_MAX));
			if (i%2) node->weights[i] = -1*node->weights[i];
		}
		ptr += node->size;
	}

}

void printNetwork(Network *nn) {
	Layer *inputLayer = nn->layers;
	printf("==== Input Layer ====\n");
	printf("Number of nodes: %d\n", inputLayer->node_count);
	Node *curNode = inputLayer->nodes;
	// for (int i = 0;i < inputLayer->node_count;i++) {
	// 	printf("Node %d output is: %f\n", i, curNode->output);
	// 	curNode++;
	// }
	// printf("input size %d\n", inputLayer->size);
	Layer *hiddenLayer = (Layer *)((uint8_t *)nn->layers + inputLayer->size);
	printf("==== Hidden Layer ====\n");
	printf("Number of nodes: %d\n", hiddenLayer->node_count);
	curNode = hiddenLayer->nodes;
	for (int i = 0;i < hiddenLayer->node_count;i++) {
		printf("Node %d output is: %f  error is: %f bias is %f \n", 
			i, curNode->output,curNode->error, curNode->bias);
		printf("weights are\n");
		// for (int j = 0;j < curNode->weight_count;j++) {
		// 	printf("   %f\n", curNode->weights[j]);
		// }
		printf("\n");
		curNode = (Node *)((uint8_t *)curNode + curNode->size);
	}

	Layer *outputLayer = (Layer *)((uint8_t *)hiddenLayer + hiddenLayer->size);
	printf("==== Output Layer ====\n");
	printf("Number of nodes: %d\n", outputLayer->node_count);
	curNode = outputLayer->nodes;
	for (int i = 0;i < outputLayer->node_count;i++) {
		printf("Node %d output is: %f  error is: %f bias is %f \n", 
			i, curNode->output,curNode->error, curNode->bias);
		printf("weights are\n");
		for (int j = 0;j < curNode->weight_count;j++) {
			printf("   %f\n", curNode->weights[j]);
		}
		printf("\n");
		curNode = (Node *)((uint8_t *)curNode + curNode->size);
	}
}

Vector *expectedFromLabel(int label) {
	Vector *expected = malloc(sizeof(Vector) + sizeof(double)*NUM_DIGITS);
	expected->size =NUM_DIGITS;
	for (int i = 0;i < NUM_DIGITS; i++) {
		if (i == label) {
			expected->vals[i] = 1;
		} else {
			expected->vals[i] = 0; 
		}
	}

	return expected;
}

int getClassification(Network *nn) {
	Layer *inputLayer = nn->layers;
	Layer *hiddenLayer = (Layer *)((uint8_t *)nn->layers + inputLayer->size);
	Layer *outputLayer = (Layer *)((uint8_t *)hiddenLayer + hiddenLayer->size);
	Node *curNode = outputLayer->nodes;
	double maxProb = 0;
	int res = 0;
	for (int i = 0;i < outputLayer->node_count;i++) {
		if (curNode->output > maxProb) {
			maxProb = curNode->output;
			res = i;
		}
		curNode = (Node *)((uint8_t *)curNode + curNode->size);
	}

	return res;

}

void trainNeuralNetwork(Network *nn) {
	for (int i = 0;i < 50;i++) {
		load_minist_init();
		for (int j = 0;j < 1000; j ++) {
			Vector *input = getNextImage();
			feedInput(nn,input);
			int label = getNextLabel();
			Vector *expected = expectedFromLabel(label);
			forwardPropagate(nn);
			backwardPropagate(nn,expected);
			updateWeights(nn);
			int class = getClassification(nn);
			printf("label is %d classified as %d\n", label, class);			
		}
		free_mninst();
	}
}

void testNeuralNetwork(Network *nn) {
	load_minist_init();
	for (int j = 0;j < 10; j ++) {
		Vector *input = getNextImage();
		feedInput(nn,input);
		int label = getNextLabel();
		forwardPropagate(nn);
		int class = getClassification(nn);
		printf("label is %d classified as %d\n", label, class);
	}
	free_mninst();

}

int main() {
	Network *nn = createNeuralNetwork(724,30,10);
	load_minist_init();
	// Vector *inputs = malloc(sizeof(Vector) + sizeof(double)*2);
	// inputs->size = 2;
	// inputs->vals[0] = 1;
	// inputs->vals[1] = 2;
	// feedInput(nn,inputs);
	initWeights(nn);
	// Vector *expected = malloc(sizeof(Vector) + sizeof(double)*3);
	// expected->size = 3;
	// expected->vals[0] = 1;
	// expected->vals[1] = 0;
	// expected->vals[2] = 0;
	// forwardPropagate(nn);
	// backwardPropagate(nn,expected);
	// printNetwork(nn);
	// updateWeights(nn);
	// printNetwork(nn);
	trainNeuralNetwork(nn);
	testNeuralNetwork(nn);
	free(nn);
	return 1;
	
}