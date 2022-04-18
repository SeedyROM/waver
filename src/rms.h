#pragma once

#include <stdlib.h>

#include "sample.h"
#include "circular_buffer.h"
#include "biquad_filter.h"

typedef struct
{
  CircularBuffer *buffer;
  BiquadFilter *lowpass;
} RMSFilter;

RMSFilter *RMSFilter_Create(size_t size, double frequency, int sampleRate);
void RMSFilter_Free(RMSFilter *rf);

Sample RMSFilter_Tick(RMSFilter *rf, Sample input);