#pragma once

#include <stdint.h>

/**
 * @brief Get the max of 2 samples typesafely
 *
 */
#define Sample_Max(a, b)    \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })

/**
 * @brief Get the min of 2 samples typesafely
 *
 */
#define Sample_Min(a, b)    \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })

/**
 * @brief The default sample type
 *
 */
typedef double Sample;

/**
 * @brief Scale for conversion to a sample
 *
 */
static const Sample SampleShortScale = 1.0 / UINT16_MAX;

/**
 * @brief Convert from uint16_t to Sample
 *
 * @param input
 * @return Sample
 */
static inline Sample Sample_ConvertFromShort(int16_t input)
{
  // Scale
  return SampleShortScale * input;
}

/**
 * @brief Convertfrom Sample to uint16_t
 *
 * @param input
 * @return int16_t
 */
static inline int16_t Sample_ConvertToShort(Sample input)
{
  // Scale
  int16_t output = INT16_MAX * input;
  // Clip for min max
  output = Sample_Max(output, INT16_MIN);
  output = Sample_Min(output, INT16_MAX);
  return output;
}