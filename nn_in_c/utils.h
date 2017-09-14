typedef enum LayerType {CONV} LayerType;
typedef enum AcFunType {SIGMOID} AcFunType;

typedef struct Node Node;
typedef struct Layer Layer;
typedef struct Vector Vector;
typedef struct Network Network;

struct Node {
	double output;
	double weights[];
	int weight_count;
};

struct Layer {
	LayerType layer_type;
	AcFunType acfun_type;
	Node nodes[];
	int node_count;

};

struct Vector {
	int size;
	double vals[];
};

struct Network {
	double learning_rate;
	Layer layers[];
};


void feedInput(Network *nn, Vector *v);
void feedForwardNetwork(Network *nn);
void backPropagateNetwork(Network *nn, int targetClassification);

