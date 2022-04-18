#include <stdlib.h>
#include <math.h>

#include "biquad_filter.h"

/**
 * @brief Create a new 2nd order BiquadFilter of type with the specified params
 *
 * @param type
 * @param frequency
 * @param Q
 * @param dbGain
 * @param sampleRate
 * @return BiquadFilter*
 */
BiquadFilter *BiquadFilter_Create(BiquadFilterType type, double frequency, double Q, double dbGain, int sampleRate)
{
  BiquadFilter *bf = malloc(sizeof(BiquadFilter));
  if (bf == NULL)
    return bf;

  // Set the type of our filter, only
  bf->type = type;

  // Calculate the coeffs for our filter
  BiquadFilter_CalcCoeffs(bf, frequency, Q, dbGain, sampleRate);

  // Initialize our state vars to 0
  bf->xm1 = bf->xm2 = bf->ym1 = bf->ym2 = 0;

  return bf;
}

/**
 * @brief Free the biquad filter
 *
 * @param bf
 */
void BiquadFilter_Free(BiquadFilter *bf)
{
  if (bf != NULL)
  {
    free(bf);
  }
}

/**
 * @brief Calculate the coefficients for our biquad filter
 *
 * @param bf
 * @param frequency
 * @param Q
 * @param dbGain
 * @param sampleRate
 */
void BiquadFilter_CalcCoeffs(BiquadFilter *bf, double frequency, double Q, double dbGain, int sampleRate)
{
  // TODO: I think, I saw this implemented better elsewhere, ignore for now.
  // TODO: Also are we prewarping, is this needed here? No tan in sight.

  // Pre calcs
  double A = pow(10, dbGain / 40); // convert to db
  double omega = 2 * M_PI * frequency / sampleRate;
  double sn = sin(omega);
  double cs = cos(omega);
  double alpha = sn / (2 * Q);
  double beta = sqrt(A + A);

  // Set our coeffs based on filter type
  // NOTE: a0 will usually be generalized to 1 + alpha
  switch (bf->type)
  {
  case BiquadFilterType_LowPass:
  default:
    bf->b0 = (1.0 - cs) / 2.0;
    bf->b1 = 1.0 - cs;
    bf->b2 = (1.0 - cs) / 2.0;
    bf->a0 = 1.0 + alpha;
    bf->a1 = -2.0 * cs;
    bf->a2 = 1.0 - alpha;
    break;
  }

  // Scale the cocksuckers
  bf->a1 /= (bf->a0);
  bf->a2 /= (bf->a0);
  bf->b0 /= (bf->a0);
  bf->b1 /= (bf->a0);
  bf->b2 /= (bf->a0);
}

/**
 * @brief Process an incoming sample for a given BiquadFilter using the binlinear transform
 *
 * @param bf
 * @param input
 * @return double
 */
Sample BiquadFilter_Tick(BiquadFilter *bf, Sample input)
{
  // Process the incoming sample
  float output = (bf->b0 * input) +
                 (bf->b1 * bf->xm1) +
                 (bf->b2 * bf->xm2) -
                 (bf->a1 * bf->ym1) -
                 (bf->a2 * bf->ym2);

  // Store state
  bf->xm2 = bf->xm1;
  bf->xm1 = input;
  bf->ym2 = bf->ym1;
  bf->ym1 = output;
  return output;
}