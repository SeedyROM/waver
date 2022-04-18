#include "wav.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Open and parse a WAV file
 *
 * @param path
 * @return WAVFile*
 */
WAVFile *WAVFile_Open(const char *path)
{
  FILE *f = fopen(path, "rb");
  if (!f)
    goto error;

  WAVFile *wf = malloc(sizeof(WAVFile));
  if (!wf)
    goto error;

  WAVHeader *h = malloc(sizeof(WAVHeader));
  if (!h)
    goto error;

  if (WAVFile_ParseHeader(h, f))
    goto error;

  wf->header = h;

  if (WAVFile_ParseData(wf, f))
    goto error;

  fclose(f);
  return wf;

error:
  fprintf(stderr, "Failed to open WAV file %s\n", path);
  return NULL;
}

/**
 * @brief Free the WAV file
 *
 * @param wf
 */
void WAVFile_Free(WAVFile *wf)
{
  // If we've malloc'd the header, free it
  if (wf->header != NULL)
  {
    free(wf->header);
  }

  // If we've malloced a data buffer for our samples, free it.
  if (wf->data != NULL)
  {
    free(wf->data);
  }

  // If the struct itself was malloc'd
  if (wf != NULL)
  {
    free(wf);
  }
}

/**
 * @brief Parse the WAV header
 *
 * @param h
 * @param f
 * @return int
 */
int WAVFile_ParseHeader(WAVHeader *h, FILE *f)
{
  // Read from the beginning of the file, if the cursor isn't there
  size_t offset = ftell(f);
  if (!offset)
    fseek(f, SEEK_SET, 0);

  // Create a 4 byte buffer
  uint8_t buf[4];

  // Since we're pretty much always reading a specific
  // number of bytes, let's use a bool to store if we read them.
  bool read;

  // Read the RIFF header
  read = fread(&buf, sizeof(buf), 1, f);
  if (!read && !strncmp(buf, "RIFF", sizeof(buf)))
    goto error;

  // Read the file size
  read = fread(&h->chunkSize, sizeof(h->chunkSize), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Chunk Size: %d\n", h->chunkSize);

  // Verify the rest of the next marker
  read = fread(&buf, sizeof(buf), 1, f);
  if (!read && !strncmp(buf, "WAVE", sizeof(buf)))
    goto error;

  // Skip the "fmt " marker & last values size, AKA 16 bytes
  read = fread(&buf, sizeof(buf), 1, f);
  if (!read)
    goto error;
  read = fread(&buf, sizeof(buf), 1, f);
  if (!read)
    goto error;

  // Read the WAV encoded data format type
  read = fread(&h->formatType, sizeof(h->formatType), 1, f);
  if (!read)
    goto error;

  // Check if we recognize the WAV encoding format
  if (!WAVFile_FormatTypeIsValid(h->formatType))
  {
    fprintf(stderr, "Invalid WAV format type\n", h->formatType);
    goto error;
  }
  fprintf(stderr, "Sample Type: %s\n", WAVFile_FormatTypeToString(h->formatType));

  // Get the number of channels from the file
  read = fread(&h->channels, sizeof(h->channels), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Number of Channels: %d\n", h->channels);

  // Get the sample rate
  read = fread(&h->sampleRate, sizeof(h->sampleRate), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Sample Rate: %d\n", h->sampleRate);

  // Get the bitrate
  read = fread(&h->bitrate, sizeof(h->bitrate), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Bit Rate: %d\n", h->bitrate);

  // Get the bitrate
  read = fread(&h->blockAlign, sizeof(h->blockAlign), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Block Align: %d\n", h->blockAlign);

  // Get the bits per sample
  read = fread(&h->bitsPerSample, sizeof(h->bitsPerSample), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Bits Per Sample: %d\n", h->bitsPerSample);

  // Found the data marker
  read = fread(&buf, sizeof(buf), 1, f);
  if (!read && !strncmp(buf, "data", sizeof(buf)))
    goto error;

  // Get the data chunk size
  read = fread(&h->dataSize, sizeof(h->dataSize), 1, f);
  if (!read)
    goto error;
  fprintf(stderr, "Chunk Data Size: %d\n", h->dataSize);

  // Confirm we've read the 44 byte header.
  // TODO: Ignore meta data and other edge cases...
  offset = ftell(f);
  if (offset != WAV_HEADER_SIZE)
    goto error;

  return 0;

error:
  fprintf(stderr, "Failed to parse WAVE file header\n");
  return -1;
}

/**
 * @brief Parse the incoming data
 *
 * @param wf
 * @param f
 * @return int
 */
int WAVFile_ParseData(WAVFile *wf, FILE *f)
{
  bool read;
  WAVHeader *h = wf->header;

  // Allocate enough memory for all our samples
  WAVData data = malloc(h->dataSize);
  if (data == NULL)
    goto error;

  // Seek to the end of the WAV header
  fseek(f, SEEK_SET, 44);

  // Read our sample data into the data buffer.
  read = fread(data, h->dataSize, 1, f);
  if (!read)
    goto error;

  // Pass our data on
  wf->data = data;

  return 0;

error:
  fprintf(stderr, "Failed to parse WAV file data\n");
  return -1;
}

/**
 * @brief Convert the WAV sample type to a string
 *
 * @param type
 * @return const char*
 */
const char *WAVFile_FormatTypeToString(WAVFormatType type)
{
  switch (type)
  {
  case WAV_FORMAT_PCM:
    return "PCM";
  case WAV_FORMAT_A_LAW:
    return "A-Law";
  case WAV_FORMAT_MU_LAW:
    return "Mu-Law";
  default:
    return "Unrecognized Format";
  }
}

/**
 * @brief Determine if the parsed WAV file encoding is valid
 *
 * @param wf
 * @return true
 * @return false
 */
bool WAVFile_FormatTypeIsValid(WAVFormatType type)
{
  return (
      type == WAV_FORMAT_PCM ||
      type == WAV_FORMAT_A_LAW ||
      type == WAV_FORMAT_MU_LAW);
}
