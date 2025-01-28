#ifndef WAV_HEADER_H
#define WAV_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav_header_types.h"

// Função para escrever em um chunk do cabeçalho WAV
void write_generic_chunk(union wav_chunk *chunk_properties,
                         union wav_header_chunks *chunks,
                         unsigned char *content_bytes);

// Função para ler o conteúdo genérico de um chunk
void read_chunk_content(union wav_chunk *chunk_properties,
                        union wav_header_chunks *chunks,
                        short *complete,
                        FILE *file);

// Função para ler o chunk RIFF do cabeçalho WAV
void read_riff_chunk(union wav_chunk *chunk_properties,
                     union wav_header_chunks *chunks,
                     FILE *file);

// Função para ler todo o cabeçalho WAV
void read_wav_header(FILE *file, union wav_header_chunks *chunks);

#endif // WAV_HEADER_H
