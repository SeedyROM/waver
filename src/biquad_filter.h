#pragma once

#include "sample.h"

typedef enum
{
  BiquadFilterType_LowPass = 0,
} BiquadFilterType;

typedef struct
{
  double a0, a1, a2;
  double b0, b1, b2;
  // Bruh, xm1/ym1 meant x[n-1], y[n-1]...
  // Fuck...
  Sample xm1, xm2;
  Sample ym1, ym2;
  BiquadFilterType type;
} BiquadFilter;

BiquadFilter *BiquadFilter_Create(BiquadFilterType type, double frequency, double Q, double dbGain, int sampleRate);
void BiquadFilter_Free(BiquadFilter *bf);

void BiquadFilter_CalcCoeffs(BiquadFilter *bf, double frequency, double Q, double dbGain, int sampleRate);
Sample BiquadFilter_Tick(BiquadFilter *bf, Sample input);