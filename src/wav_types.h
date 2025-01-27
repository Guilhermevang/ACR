#ifndef WAV_TYPES_H
#define WAV_TYPES_H

// #define WAV_HEADER_SIZE 44 // Um arquivo WAV tem cabeçalho de 44 bytes

struct wav_chunk_struct
{
    char chunk_id[4]; // 'RIFF'
    int chunk_size;   // O tamanho do resto dos chunks seguidos desse
    char format[4];   // 'WAVE'
};

union wav_chunk
{
    unsigned char bytes[sizeof(struct wav_chunk_struct)];
    struct wav_chunk_struct data;
};

struct wav_fmt_subchunk_struct
{
    char subchunk1_id[4];  // 'fmt '
    int subchunk1_size;    // 16 para PCM (novamente, o resto dos chunks após esse número)
    short audio_format;    // 1 para PCM, valores diferentes de 1 indica algum nível de compressão
    short num_channels;    // Número de canais: Mono = 1, Stereo = 2
    int sample_rate;       // Taxa de amostragem: 44100 Hz
    int byte_rate;         // sample_rate * num_channels * (bits_per_sample / 8) (Bytes por segundo -> Bytes por amostra * quantidade de amostras)
    short block_align;     // num_channels * (bits_per_sample / 8) (Bytes por amostra * todos canais)
    short bits_per_sample; // 8 bits, 16 bits, ...

    short extra_params_size;
    unsigned char *extra_params;
};

union wav_fmt_subchunk
{
    unsigned char bytes[sizeof(struct wav_fmt_subchunk_struct)];
    struct wav_fmt_subchunk_struct data;
};

struct wav_data_subchunk_struct
{
    unsigned char subchunk2_id[4]; // 'data'
    int subchunk2_size;            // total_samples * num_channels * (bits_per_sample / 8)
};

union wav_data_subchunk
{
    unsigned char bytes[sizeof(struct wav_data_subchunk_struct)];
    struct wav_data_subchunk_struct data;
};

struct wav_header_struct
{
    struct wav_chunk_struct chunk;
    struct wav_fmt_subchunk_struct subchunk1;
    struct wav_data_subchunk_struct subchunk2;
};

union wav_header
{
    unsigned char bytes[sizeof(struct wav_header_struct)];
    struct wav_header_struct data;
};

struct wav_fact_chunk_struct
{
    unsigned char chunk_id[4];
    int chunk_size;
};

union wav_fact_chunk
{
    unsigned char bytes[sizeof(struct wav_fact_chunk_struct)];
    struct wav_fact_chunk_struct data;
};

struct wav_header_chunks_struct
{
    union wav_chunk primary_chunk;
    union wav_fmt_subchunk format_chunk;
    union wav_fact_chunk fact_chunk;
    union wav_data_subchunk data_chunk;
};

union wav_header_chunks
{
    unsigned char bytes[sizeof(struct wav_header_chunks_struct)];
    struct wav_header_chunks_struct data;
};

struct wav_single_chunk_struct
{
    char chunk_id[4]; // ID do chunk
    int chunk_size;   // O tamanho do resto dos chunks seguidos desse
};

union wav_single_chunk
{
    unsigned char bytes[sizeof(struct wav_single_chunk_struct)];
    struct wav_single_chunk_struct data;
};

#endif