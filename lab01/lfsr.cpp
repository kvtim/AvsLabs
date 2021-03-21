#include <iostream>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    unsigned b = ((*reg >> 2) ^ (*reg >> 3) ^ (*reg >> 5) ^ *reg) & 1;
    *reg >>= 1;
    *reg = (*reg & ~ (1 << 15)) | (b << 15);
}