#pragma once

#include "sample.h"

typedef struct
{
  Sample *data;
  size_t size;
  size_t index;
} CircularBuffer;

CircularBuffer *CircularBuffer_Create(size_t size);
void CircularBuffer_Push(CircularBuffer *c, Sample input);
Sample CircularBuffer_Pop(CircularBuffer *c);