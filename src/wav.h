#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

static const uint8_t WAV_HEADER_SIZE = 44;

typedef enum
{
  WAV_FORMAT_PCM = 1,
  WAV_FORMAT_A_LAW = 6,
  WAV_FORMAT_MU_LAW = 7,
} WAVFormatType;

typedef struct
{
  uint32_t chunkSize;
  uint16_t formatType;
  uint16_t channels;
  uint32_t sampleRate;
  uint32_t bitrate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  uint32_t dataSize;
} WAVHeader;

typedef void *WAVData;

typedef struct
{
  WAVHeader *header;
  WAVData data;
} WAVFile;

WAVFile *WAVFile_Open(const char *path);
void WAVFile_Free(WAVFile *wf);

int WAVFile_ParseHeader(WAVHeader *h, FILE *f);
int WAVFile_ParseData(WAVFile *wf, FILE *f);
const char *WAVFile_FormatTypeToString(WAVFormatType type);
bool WAVFile_FormatTypeIsValid(WAVFormatType type);