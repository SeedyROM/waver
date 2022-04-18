#include <math.h>

#include "rms.h"

RMSFilter *RMSFilter_Create(size_t size, double frequency, int sampleRate)
{
  RMSFilter *rf = malloc(sizeof(RMSFilter));
  if (rf == NULL)
    return NULL;

  // Create circular buffer
  CircularBuffer *cb = CircularBuffer_Create(size);
  if (cb == NULL)
    return NULL;

  // Create a biquad filter
  BiquadFilter *bf = BiquadFilter_Create(BiquadFilterType_LowPass, 200.0, 0.5, 0.0, sampleRate);
  if (bf == NULL)
    return NULL;

  rf->buffer = cb;
  rf->lowpass = bf;

  return rf;
}

void RMSFilter_Free(RMSFilter *rf)
{
  if (rf->buffer != NULL)
    CircularBuffer_Free(rf->buffer);

  if (rf->lowpass != NULL)
    BiquadFilter_Free(rf->lowpass);

  if (rf != NULL)
    free(rf);
}

Sample RMSFilter_Tick(RMSFilter *rf, Sample input)
{
  // TODO: Fix me, this is godawful
  CircularBuffer_Write(rf->buffer, input);
  Sample tmp = sqrt(CircularBuffer_Read(rf->buffer));
  tmp *= 1 / rf->buffer->size;
  return BiquadFilter_Tick(rf->lowpass, tmp);
}