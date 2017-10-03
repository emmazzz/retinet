#ifndef _UTILS_H
#define _UTILS_H



// typedef enum LayerType {CONV} LayerType;
// typedef enum AcFunType {SIGMOID} AcFunType;

typedef struct Node Node;
typedef struct Layer Layer;
typedef struct Vector Vector;
typedef struct Network Network;

struct Node {
	double output;
	double bias;
	double error;
	int weight_count;
	int size;
	double weights[];
};

struct Layer {
	// LayerType layer_type;
	// AcFunType acfun_type;
	int node_count;
	int size;
	Node nodes[];

};

struct Vector {
	int size;
	double vals[];
};

struct Network {
	double learning_rate;
	int layer_count;
	Layer layers[];
};


void feedInput(Network *nn, Vector *v);
void forwardPropagate(Network *nn);
void backwardPropagate(Network *nn, Vector *expected);
void updateWeights(Network *nn);

#endif