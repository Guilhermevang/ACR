// Fontes:
// Cabeçalho WAV: http://soundfile.sapp.org/doc/WaveFormat/
// Unions: https://www.programiz.com/c-programming/c-unions
// Áudios de exemplo:
// Sobre o 'fact' em WAV não-PCM: https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wav_types.h"

FILE *open_wav_file(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("ERRO: Não foi possível abrir o arquivo '%s'\n", file_name);
        exit(EXIT_FAILURE);
    }

    return file;
}

void write_to_chunk(union wav_single_chunk *chunk_properties,
                    union wav_header_chunks *chunks,
                    unsigned char *content_bytes)
{
    char chunk_id[4];
    strncpy(chunk_id, chunk_properties->data.chunk_id, 4);

    short is_fmt_chunk = strncmp(chunk_id, "fmt ", 4) == 0;
    if (is_fmt_chunk)
    {
        short def_pcm_fmt_chunk_size = 16;

        memcpy(chunks->data.format_chunk.bytes, chunk_properties->bytes, sizeof(chunk_properties->bytes));
        memcpy(chunks->data.format_chunk.bytes + sizeof(chunk_properties->bytes), content_bytes, def_pcm_fmt_chunk_size);

        // Se o conteúdo do chunk for maior que o padrão PCM, então copia o resto como parâmetros extras
        if (chunk_properties->data.chunk_size > def_pcm_fmt_chunk_size)
        {
            // Quantos bytes restam ser inseridos
            int remaining_bytes_count = chunk_properties->data.chunk_size - def_pcm_fmt_chunk_size;

            // Aloca memória para os parâmetros extras. É removido 2 bytes (sizeof short) pois
            // há o "extra params size", que dita o tamanho do conteúdo extra
            chunks->data.format_chunk.data.extra_params = malloc(remaining_bytes_count - sizeof(short));
            if (!chunks->data.format_chunk.data.extra_params)
            {
                printf("Não foi possível alocar memória para os parâmetros extras do chunk 'fmt'\n");
                exit(EXIT_FAILURE);
            }

            // Copia os bytes/dados restantes, que são os parâmetros extras
            memcpy(chunks->data.format_chunk.bytes + sizeof(chunk_properties->bytes) + def_pcm_fmt_chunk_size,
                   content_bytes + def_pcm_fmt_chunk_size, remaining_bytes_count);
        }
    }

    printf("asda\n");
}

void read_generic_chunk_content(union wav_single_chunk *chunk_properties,
                                union wav_header_chunks *chunks,
                                short *complete,
                                FILE *file)
{
    // Aloca memória para o conteúdo do chunk
    unsigned char *content_bytes = malloc(chunk_properties->data.chunk_size);
    if (!content_bytes)
    {
        printf("Não foi possível alocar memória para o conteúdo do chunk '%s'. Tamanho: %d\n",
               chunk_properties->data.chunk_id, chunk_properties->data.chunk_size);
        exit(EXIT_FAILURE);
    }

    // Lê o conteúdo do chunk
    fread(content_bytes, chunk_properties->data.chunk_size, 1, file);

    // Se o chunk for de dados, é o fim do cabeçalho, tudo após esse chunk é conteúdo das ondas de áudio
    if (strncmp(chunk_properties->data.chunk_id, "data", 4) == 0)
        *complete = 1;

    write_to_chunk(chunk_properties, chunks, content_bytes);

    // Libera a memória alocada na heap
    free(content_bytes);
};

void read_riff_chunk(union wav_single_chunk *chunk_properties,
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

void read_wav_header(FILE *file, union wav_header *header_bytes)
{
    union wav_header_chunks chunks;          // Todos os chunks WAV
    union wav_single_chunk chunk_properties; // ID e tamanho do chunk atual

    short complete = 0; // Indica se já leu todo o cabeçalho
    while (!complete)
    {
        // Lê as propriedades do chunk, como ID e tamanho do conteúdo
        fread(&chunk_properties, sizeof(union wav_single_chunk), 1, file);

        // Se o chunk for o primeiro (RIFF), lê apenas os próximos 4 bytes
        if (strncmp(chunk_properties.data.chunk_id, "RIFF", 4) == 0)
        {
            read_riff_chunk(&chunk_properties, &chunks, file);
        }
        else
        {
            // Lé o chunk atual
            read_generic_chunk_content(&chunk_properties, &chunks, &complete, file);
        }
    }
}

int main()
{
    char file_name[] = "addf8-Alaw-GW.wav";
    union wav_header header;

    FILE *file_in = open_wav_file(file_name);

    read_wav_header(file_in, &header);

    return EXIT_SUCCESS;
}