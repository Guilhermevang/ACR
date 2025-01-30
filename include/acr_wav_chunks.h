#ifndef ACR_WAV_CHUNKS_H
#define ACR_WAV_CHUNKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acr_wav_header.h"

#define ACR_WAV_CHUNK_ID_RIFF "RIFF"  // 'RIFF'
#define ACR_WAV_CHUNK_ID_FMT "fmt "	  // 'fmt '
#define ACR_WAV_CHUNK_ID_DATA "data"  // 'data'
#define ACR_WAV_CHUNK_ID_FACT "fact"  // 'fact'

#define ACR_WAV_MAX_EXTRA_BUFFER_SIZE 512

int ACR_WAV_ReadHeaderChunks(FILE *File, union ACR_WAV_Header *Header);

void ACR_WAV_GetNextChunk(FILE *File, union ACR_WAV_ChunkHeader *ChunkHeader, short int *IsComplete);

void ACR_WAV_ReadChunk(FILE *File, union ACR_WAV_ChunkHeader *ChunkHeader, union ACR_WAV_Header *Header);

static short int ACR_WAV_IsChunkId(union ACR_WAV_ChunkHeader *ChunkHeader, char ChunkId[ACR_WAV_CHUNK_ID_SIZE]);

void ACR_WAV_WriteToChunk(unsigned char *Chunk, int ChunkSize, int *ExtraBufferSize, union ACR_WAV_ChunkHeader *ChunkHeader, FILE *File);

void ACR_WAV_WriteExtraBuffer(char *Buffer, int BufferSize, FILE *File);

#endif