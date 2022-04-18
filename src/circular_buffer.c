#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "circular_buffer.h"

CircularBuffer *CircularBuffer_Create(size_t size)
{
  CircularBuffer *c = malloc(sizeof(CircularBuffer));
  if (c == NULL)
    goto error;

  c->data = malloc(sizeof(Sample) * size);
  if (c->data == NULL)
    goto error;

  c->size = size;
  c->index = 0;
  memset(c->data, (Sample)0, size);

  return c;

error:
  fprintf(stderr, "Failed to allocate memory for circular buffer\n");
  return NULL;
}

void CircularBuffer_Free(CircularBuffer *c)
{
  if (c->data != NULL)
    free(c->data);

  if (c != NULL)
    free(c);
}

void CircularBuffer_Write(CircularBuffer *c, Sample input)
{
  c->data[c->index] = input;

  if (c->index == c->size - 1)
  {
    c->index = 0;
  }
  else
  {
    c->index++;
  }
}

Sample CircularBuffer_Read(CircularBuffer *c)
{
  return c->data[c->index];
}