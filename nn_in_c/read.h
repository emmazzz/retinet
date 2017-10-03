#ifndef _READ_H
#define _READ_H

#include "utils.h"
#include <stdint.h>
#include <string.h>


void free_mninst();
Vector *getNextImage();
int getNextLabel();
int load_minist_init();

#endif