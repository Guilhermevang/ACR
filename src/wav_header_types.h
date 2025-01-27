#ifndef WAV_HEADER_TYPES_H
#define WAV_HEADER_TYPES_H

// #define WAV_HEADER_SIZE 44 // Um arquivo WAV tem cabeçalho de 44 bytes

struct wav_primary_chunk_struct
{
    char chunk_id[4]; // 'RIFF'
    int chunk_size;   // O tamanho do resto dos chunks seguidos desse
    char format[4];   // 'WAVE'
};

union wav_primary_chunk
{
    unsigned char bytes[sizeof(struct wav_primary_chunk_struct)];
    struct wav_primary_chunk_struct data;
};

struct wav_fmt_chunk_struct
{
    char chunk_id[4];      // 'fmt '
    int chunk_size;        // 16 para PCM (novamente, o resto dos chunks após esse número)
    short audio_format;    // 1 para PCM, valores diferentes de 1 indica algum nível de compressão
    short num_channels;    // Número de canais: Mono = 1, Stereo = 2
    int sample_rate;       // Taxa de amostragem: 44100 Hz
    int byte_rate;         // sample_rate * num_channels * (bits_per_sample / 8) (Bytes por segundo -> Bytes por amostra * quantidade de amostras)
    short block_align;     // num_channels * (bits_per_sample / 8) (Bytes por amostra * todos canais)
    short bits_per_sample; // 8 bits, 16 bits, ...

    short extra_params_size;     // Tamanho do subchunk de parâmetros extras
    unsigned char *extra_params; // Parâmetros extras
};

union wav_fmt_chunk
{
    unsigned char bytes[sizeof(struct wav_fmt_chunk_struct)];
    struct wav_fmt_chunk_struct data;
};

struct wav_data_chunk_struct
{
    unsigned char chunk_id[4]; // 'data'
    int chunk_size;            // total_samples * num_channels * (bits_per_sample / 8)
    unsigned char *content;    // Conteúdo das ondas sonoras do arquivo de áudio WAV
};

union wav_data_chunk
{
    unsigned char bytes[sizeof(struct wav_data_chunk_struct)];
    struct wav_data_chunk_struct data;
};

struct wav_fact_chunk_struct
{
    unsigned char chunk_id[4];
    int chunk_size;
    unsigned char *content;
};

union wav_fact_chunk
{
    unsigned char bytes[sizeof(struct wav_fact_chunk_struct)];
    struct wav_fact_chunk_struct data;
};

struct wav_header_chunks_struct
{
    union wav_primary_chunk primary_chunk;
    union wav_fmt_chunk format_chunk;
    union wav_fact_chunk fact_chunk;
    union wav_data_chunk data_chunk;
};

union wav_header_chunks
{
    unsigned char bytes[sizeof(struct wav_header_chunks_struct)];
    struct wav_header_chunks_struct data;
};

struct wav_chunk_struct
{
    char chunk_id[4]; // ID do chunk
    int chunk_size;   // O tamanho do resto dos chunks seguidos desse
};

union wav_chunk
{
    unsigned char bytes[sizeof(struct wav_chunk_struct)];
    struct wav_chunk_struct data;
};

#endif