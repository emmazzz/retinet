// # Author Bernie Yang, modified from 10601 sample code
#ifndef _READ_H
#define _READ_H

#include "utils.h"
#include <stdint.h>
#include <string.h>


void free_mnist_test();
Vector *getNextImageTest();
int getNextLabelTest();
int load_mnist_init_test();

void free_mnist_train();
Vector *getNextImageTrain();
int getNextLabelTrain();
int load_mnist_init_train();

#endif