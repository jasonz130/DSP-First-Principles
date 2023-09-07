#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <stdint.h>

#define FIR_FILTER_LENGTH 181

typedef struct{
    float buff[FIR_FILTER_LENGTH];
    uint8_t buffIndex;
    float out;
} FIRFilter;

void FIRFilter_init(FIRFilter *fir);
float FIRFilter_calc(FIRFilter *fir, float inputVal);

#endif