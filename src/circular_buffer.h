#pragma once

#include "sample.h"

typedef struct
{
  Sample *data;
  size_t size;
  size_t index;
} CircularBuffer;

CircularBuffer *CircularBuffer_Create(size_t size);
void CircularBuffer_Free(CircularBuffer *c);
void CircularBuffer_Write(CircularBuffer *c, Sample input);
Sample CircularBuffer_Read(CircularBuffer *c);