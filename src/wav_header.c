#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wav_header_types.h"
#include "wav_header.h"
#include "logger.h"
#include "helper-functions.h"
#include "resource-manager.h"

#define CHUNK_ID_SIZE 4
#define PCM_FMT_CHUNK_SIZE 16

void copy_bytes_to_chunk(unsigned char *dest,
                         unsigned char *bytes,
                         union wav_chunk *prop)
{
    memcpy(dest, prop->bytes, sizeof(prop));
    memcpy(dest + sizeof(prop), bytes, PCM_FMT_CHUNK_SIZE);
}

void write_generic_chunk(union wav_chunk *chunk_properties,
                         union wav_header_chunks *chunks,
                         unsigned char *content_bytes)
{
    char chunk_id[CHUNK_ID_SIZE];
    strncpy(chunk_id, chunk_properties->data.chunk_id, CHUNK_ID_SIZE);

    short is_fmt_chunk = strncmp(chunk_id, "fmt ", CHUNK_ID_SIZE) == 0;
    short is_fact_chunk = strncmp(chunk_id, "fact", CHUNK_ID_SIZE) == 0;
    short is_data_chunk = strncmp(chunk_id, "data", CHUNK_ID_SIZE) == 0;

    if (is_fmt_chunk)
    {
        memcpy(chunks->data.format_chunk.bytes, chunk_properties->bytes, sizeof(chunk_properties));
        memcpy(chunks->data.format_chunk.bytes + sizeof(chunk_properties), content_bytes, PCM_FMT_CHUNK_SIZE);

        // Se o conteúdo do chunk for maior que o padrão PCM, então copia o resto como parâmetros extras
        if (chunk_properties->data.chunk_size > PCM_FMT_CHUNK_SIZE)
        {
            // Quantos bytes restam ser inseridos
            int remaining_bytes_count = chunk_properties->data.chunk_size - PCM_FMT_CHUNK_SIZE;

            // Aloca memória para os parâmetros extras. É removido 2 bytes (sizeof short) pois
            // há o "extra params size", que dita o tamanho do conteúdo extra
            chunks->data.format_chunk.data.extra_params = safe_malloc(remaining_bytes_count - sizeof(short));

            // Copia os bytes/dados restantes, que são os parâmetros extras
            memcpy(chunks->data.format_chunk.bytes + sizeof(chunk_properties->bytes) + PCM_FMT_CHUNK_SIZE,
                   content_bytes + PCM_FMT_CHUNK_SIZE, remaining_bytes_count);
        }
    }
    else if (is_fact_chunk)
    {
        // Aloca memória para o conteúdo (ondas sonoras)
        chunks->data.fact_chunk.data.content = safe_malloc(chunk_properties->data.chunk_size);

        // Atribui os valores no chunk
        copy_bytes_to_chunk(chunks->data.fact_chunk.bytes, content_bytes, chunk_properties);
    }
    else if (is_data_chunk)
    {
        // Aloca memória para o conteúdo (ondas sonoras)
        chunks->data.data_chunk.data.content = safe_malloc(chunk_properties->data.chunk_size);

        // Atribui os valores no chunk
        copy_bytes_to_chunk(chunks->data.data_chunk.bytes, content_bytes, chunk_properties);
    }
}

void read_chunk_content(union wav_chunk *prop,
                        union wav_header_chunks *chunks,
                        short *is_complete,
                        FILE *file)
{
    // Aloca memória para o conteúdo do chunk
    unsigned char *content_bytes = safe_malloc(prop->data.chunk_size);

    // Lê o conteúdo do chunk
    fread(content_bytes, prop->data.chunk_size, 1, file);

    // Se o chunk for de dados, é o fim do cabeçalho, tudo após esse chunk é conteúdo das ondas de áudio
    if (strncmp(prop->data.chunk_id, "data", CHUNK_ID_SIZE) == 0)
        *is_complete = 1;

    write_generic_chunk(prop, chunks, content_bytes);

    // Libera a memória alocada na heap
    free_resource(content_bytes);
};

void read_riff_chunk(union wav_chunk *chunk_properties,
                     union wav_header_chunks *chunks,
                     FILE *file)
{
    // Conteúdo (em bytes)
    unsigned char content_bytes[4];

    // Lê os 4 bytes que vem o tipo 'WAVE'
    fread(content_bytes, 1, 4, file);

    memcpy(chunks->data.primary_chunk.bytes, chunk_properties->bytes, sizeof(chunk_properties->bytes));
    memcpy(chunks->data.primary_chunk.bytes + sizeof(chunk_properties->bytes), content_bytes, sizeof(content_bytes));
}

void read_wav_header(FILE *file, union wav_header_chunks *chunks)
{
    // ID e tamanho do chunk atual
    union wav_chunk chunk_properties;

    short is_complete = 0; // Indica se o cabeçalho já foi lido por completo
    short i = 8;           // Máximo de interações para impedir loop infinito

    // Enquanto não tiver lido todo o cabeçalho (todos os chunks)
    while (!is_complete && i > 0)
    {
        // Lê as propriedades do chunk (ID e tamanho do conteúdo do chunk)
        fread(&chunk_properties, sizeof(chunk_properties), 1, file);

        if (strncmp(chunk_properties.data.chunk_id, "RIFF", CHUNK_ID_SIZE) == 0)
        {
            // Se o chunk for o primeiro (RIFF), lê apenas os próximos 4 bytes
            read_riff_chunk(&chunk_properties, chunks, file);
        }
        else
        {
            // Lé o chunk atual
            read_chunk_content(&chunk_properties, chunks, &is_complete, file);
        }

        i--;
    }
}