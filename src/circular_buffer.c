#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "circular_buffer.h"

CircularBuffer *CircularBuffer_Create(size_t size)
{
  CircularBuffer *c = malloc(sizeof(CircularBuffer));
  if (c == NULL)
    goto error;

  c->data = malloc(sizeof(void *) * size);
  if (c->data == NULL)
    goto error;

  c->size = size;
  c->index = 0;
  memset(c->data, (void *)0, size);

  return c;

error:
  fprintf(stderr, "Failed to allocate memory for circular buffer\n");
  return NULL;
}

void CircularBuffer_Push(CircularBuffer *c, void *input)
{
  c->data[c->index++] = input;

  if (c->index >= c->size)
  {
    c->index = 0;
  }
}

void *CircularBuffer_Pop(CircularBuffer *c)
{
  return c->data[c->index];
}