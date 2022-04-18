#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "wav.h"
#include "sample.h"

int main(int argc, char **argv)
{
  // Where our WAV file lives for testing
  const char *path = "./data/snare.wav";

  // Open the WAV file
  WAVFile *wf = WAVFile_Open(path);
  assert(wf != NULL);

  // Get the WAV header
  WAVHeader *h = wf->header;

  // Check other file info, explanitory
  assert(h->formatType == 1);
  assert(h->channels == 2);
  assert(h->sampleRate == 44100);
  assert(h->bitrate == 176400);
  assert(h->blockAlign == 4);
  assert(h->bitsPerSample == 16);
  // Assert we read the entire 44 byte header and our data is ready to be read
  assert(h->chunkSize - h->dataSize == WAV_HEADER_SIZE - 8);

  // Read our data
  int16_t *data = wf->data;

  // Some local vars
  Sample left, right, rms, x;
  left = right = rms = x = 0;
  int rmsLookback = 32;

  // Print to stdout so we can plot for debugging...
  // Only get every 4th sample for now...
  for (int i = 0; i < 44100; i += h->channels * 8)
  {
    // Simple RMS calc attempt, only the left channel, using rmsLookback sample lookback
    if (i > rmsLookback)
    {
      rms = 0;
      for (int j = rmsLookback; j > 0; j--)
      {
        x = Sample_ConvertFromShort(data[i - j]);
        rms += x * x;
      }
      rms = sqrt(rms);
    }

    // Get sample data
    left = Sample_ConvertFromShort(data[i]);
    right = Sample_ConvertFromShort(data[i + 1]);

    // Print to stdout
    printf("%d %f %f %f\n", i, left, right, rms);
  }

  // Free the file
  WAVFile_Free(wf);

  return 0;
}