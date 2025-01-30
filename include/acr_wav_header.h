#ifndef ACR_WAV_HEADER_H
#define ACR_WAV_HEADER_H

#define ACR_WAV_CHUNK_ID_SIZE 4

// Estrutura para armazenar o cabeçalho do chunk
// >> START
typedef struct
{
	char ChunkId[ACR_WAV_CHUNK_ID_SIZE];  // ID do chunk (ex.: "RIFF", "fmt ", "data")
	int	 ChunkSize;						  // Tamanho do conteúdo do chunk
} ACR_WAV_ChunkHeader_t;

union ACR_WAV_ChunkHeader
{
	unsigned char		  Bytes[sizeof(ACR_WAV_ChunkHeader_t)];
	ACR_WAV_ChunkHeader_t Data;
};
// << END

// Primary Chunk (RIFF Chunk)
// >> START
typedef struct
{
	char ChunkId[ACR_WAV_CHUNK_ID_SIZE];  // 'RIFF'
	int	 ChunkSize;						  // O tamanho do resto dos chunks seguidos desse
	char Format[ACR_WAV_CHUNK_ID_SIZE];	  // 'WAVE'
} ACR_WAV_PrimaryChunk_t;

union ACR_WAV_PrimaryChunk
{
	unsigned char		   Bytes[sizeof(ACR_WAV_PrimaryChunk_t)];
	ACR_WAV_PrimaryChunk_t Data;
};
// << END

// Format Chunk
// >> START
typedef struct
{
	char  ChunkId[ACR_WAV_CHUNK_ID_SIZE];  // 'fmt '
	int	  ChunkSize;					   // 16 para PCM (novamente, o resto dos chunks após esse número)
	short AudioFormat;					   // 1 para PCM, valores diferentes de 1 indica algum nível de compressão
	short NumChannels;					   // Número de canais: Mono = 1, Stereo = 2
	int	  SampleRate;					   // Taxa de amostragem: 44100 Hz
	int	  ByteRate;						   // sample_rate * num_channels * (bits_per_sample / 8) (Bytes por segundo -> Bytes por amostra * quantidade de amostras)
	short BlockAlign;					   // num_channels * (bits_per_sample / 8) (Bytes por amostra * todos canais)
	short BitsPerSample;				   // 8 bits, 16 bits, ...
} ACR_WAV_FmtChunk_t;

union ACR_WAV_FmtChunk
{
	unsigned char	   Bytes[sizeof(ACR_WAV_FmtChunk_t)];
	ACR_WAV_FmtChunk_t Data;
};
// << END

// Data Chunk
// >> START
typedef struct
{
	unsigned char ChunkId[ACR_WAV_CHUNK_ID_SIZE];  // 'data'
	int			  ChunkSize;					   // total_samples * num_channels * (bits_per_sample / 8)
} ACR_WAV_DataChunk_t;

union ACR_WAV_DataChunk
{
	unsigned char		Bytes[sizeof(ACR_WAV_DataChunk_t)];
	ACR_WAV_DataChunk_t Data;
};
// << END

// Fact Chunk
// >> START
typedef struct
{
	unsigned char ChunkId[ACR_WAV_CHUNK_ID_SIZE];
	int			  ChunkSize;
} ACR_WAV_FactChunk_t;

union ACR_WAV_FactChunk
{
	unsigned char		Bytes[sizeof(ACR_WAV_FactChunk_t)];
	ACR_WAV_FactChunk_t Data;
};
// << END

// WAV * Header Chunks
// >> START
typedef struct
{
	union ACR_WAV_PrimaryChunk PrimaryChunk;
	union ACR_WAV_FmtChunk	   FormatChunk;
	union ACR_WAV_FactChunk	   FactChunk;
	union ACR_WAV_DataChunk	   DataChunk;
} ACR_WAV_Header_t;

union ACR_WAV_Header
{
	unsigned char	 Bytes[sizeof(ACR_WAV_Header_t)];
	ACR_WAV_Header_t Data;
};
// << END

#endif