#pragma once

typedef struct
{
  void **data;
  size_t size;
  size_t index;
} CircularBuffer;

CircularBuffer *CircularBuffer_Create(size_t size);
void CircularBuffer_Push(CircularBuffer *c, void *input);
void *CircularBuffer_Pop(CircularBuffer *c);