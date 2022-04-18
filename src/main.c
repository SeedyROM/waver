#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
  int oneSecond = h->sampleRate / h->channels;
  Sample left = 0, right = 0;

  // Print to stdout so we can plot for debugging...
  for (int i = 0; i < oneSecond; i++)
  {
    left = Sample_ConvertFromShort(*data++);
    right = Sample_ConvertFromShort(*data++);
    printf("%d %f %f\n", i, left, right);
  }

  // Free the file
  WAVFile_Free(wf);

  return 0;
}