#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "wav.h"
#include "sample.h"
#include "biquad_filter.h"

WAVFile *test_open_wav();
void test_assert_wav_header(WAVHeader *h);
void test_wav_file(WAVFile *wf);

int main(int argc, char **argv)
{
  // Open a WAV file
  WAVFile *wf = test_open_wav();

  // Assert info about our file being decoded correctly
  test_assert_wav_header(wf->header);

  // Run some test code to process and graph the audio data
  test_wav_file(wf);

  // Free the file
  WAVFile_Free(wf);

  return 0;
}

WAVFile *test_open_wav()
{
  // Where our WAV file lives for testing
  const char *path = "./data/snare.wav";

  // Open the WAV file
  WAVFile *wf = WAVFile_Open(path);
  assert(wf != NULL);

  return wf;
}

void test_assert_wav_header(WAVHeader *h)
{
  // Check other file info, explanitory
  assert(h->formatType == 1);
  assert(h->channels == 2);
  assert(h->sampleRate == 44100);
  assert(h->bitrate == 176400);
  assert(h->blockAlign == 4);
  assert(h->bitsPerSample == 16);
  // Assert we read the entire 44 byte header and our data is ready to be read
  assert(h->chunkSize - h->dataSize == WAV_HEADER_SIZE - 8);
}

void test_wav_file(WAVFile *wf)
{
  WAVHeader *h = wf->header;

  // Read our data
  int16_t *data = wf->data;

  // Some local vars
  Sample left, right, rms, x;
  left = right = rms = x = 0;
  int rmsLookback = 32;

  // Create a lowpass biquad filter
  BiquadFilter *lp = BiquadFilter_Create(BiquadFilterType_LowPass, 200, 0.5, 0, h->sampleRate);

  // Print to stdout so we can plot for debugging...
  // Only get every 8th sample for now...
  for (int i = 0; i < 44100; i += h->channels * 8)
  {
    // Simple RMS calc attempt, only the left channel, using rmsLookback sample lookback
    if (i > rmsLookback)
    {
      // Reset our current RMS for the lookback
      rms = 0;
      // Integrate discretely by rmsLookback samples
      for (int j = rmsLookback; j > 0; j--)
      {
        // Get the x[n-j]th sample
        x = Sample_ConvertFromShort(data[i - j]);
        // Square up
        rms += x * x;
      }
      // Lowpass filter our RMS
      rms = BiquadFilter_Tick(lp, sqrt(rms));
    }

    // Get sample data
    left = Sample_ConvertFromShort(data[i]);
    right = Sample_ConvertFromShort(data[i + 1]);

    // Print to stdout
    printf("%d %f %f %f\n", i, left, right, rms);
  }
}