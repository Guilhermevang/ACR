#include "acr_wav_chunks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acr_wav_header.h"
#include "hlp.h"

// Lê todos os chunks do cabeçalho
int ACR_WAV_ReadHeaderChunks(FILE *File, union ACR_WAV_Header *Header)
{
	if (!File || !Header)
	{
		return -1;	// Erro: parâmetros inválidos
	}

	HLP_Log("Executando a leitura dos chunks do cabeçalho");

	union ACR_WAV_ChunkHeader ChunkHeader;
	short int				  IsComplete = 0;  // Indica se todos os chunks já foram lidos
	short int				  ReadChunks = 0;  // Chunks lidos

	// Itera sobre todos os chunks e lê cada um
	while (!IsComplete && ReadChunks < 8)
	{
		HLP_Log("Obtendo dados do chunk");

		ACR_WAV_GetNextChunk(File, &ChunkHeader, &IsComplete);
		ACR_WAV_ReadChunk(File, &ChunkHeader, Header);
		ReadChunks++;

		HLP_Log("Chunks lidos: %d", ReadChunks);
	}
}

// Obtém o cabeçalho do chunk
void ACR_WAV_GetNextChunk(FILE *File, union ACR_WAV_ChunkHeader *ChunkHeader, short int *IsComplete)
{
	HLP_Log("Buscando chunk");

	HLP_Log("Posição do cursor: %d", ftell(File));

	// Lê o cabeçalho do chunk (id + tamanho)
	if (fread(ChunkHeader, sizeof(ChunkHeader), 1, File) != 1)
	{
		HLP_LogError("Não foi possível ler o cabeçalho do chunk\n");
		return;
	}

	HLP_Log("ID do Chunk: '%s' | Tamanho: '%d'", ChunkHeader->Data.ChunkId, ChunkHeader->Data.ChunkSize);

	// Verifica se o chunk é o último ('data'), e conclui a leitura do cabeçalho WAV
	short int IsDataChunk = ACR_WAV_IsChunkId(ChunkHeader, ACR_WAV_CHUNK_ID_DATA);
	*IsComplete = IsDataChunk;
}

void ACR_WAV_ReadChunk(FILE *File, union ACR_WAV_ChunkHeader *ChunkHeader, union ACR_WAV_Header *Header)
{
	HLP_Log("Iniciando leitura do conteúdo do chunk");

	char ExtraBuffer[ACR_WAV_MAX_EXTRA_BUFFER_SIZE];  // Espaço para 512 bytes (0.5 kb) de informações extras
	int	 ExtraBufferSize = 0;						  // Excesso de informação além do tamanho padrão do chunk

	// Caso seja o chunk primário, terá condição especial/específica de leitura
	// short int IsRiffChunk = ACR_WAV_IsChunkId(ChunkHeader, ACR_WAV_CHUNK_ID_RIFF);
	short int IsRiffChunk = ACR_WAV_IsChunkId(ChunkHeader, ACR_WAV_CHUNK_ID_RIFF);
	if (IsRiffChunk)
	{
		HLP_Log("Lendo conteúdo do chunk primário");

		int	 ContentBufferSize = sizeof(ACR_WAV_PrimaryChunk_t) - sizeof(ACR_WAV_ChunkHeader_t);
		char ContentBytes[ContentBufferSize];

		// Lê o formato do arquivo ('WAVE')
		fread(&ContentBytes, ContentBufferSize, 1, File);

		// Escreve o cabeçalho do chunk
		memcpy(Header->Data.PrimaryChunk.Bytes, ChunkHeader->Bytes, sizeof(ACR_WAV_ChunkHeader_t));

		// Escreve o conteúdo do chunk
		// - No caso do chunk primário (RIFF), não é usado a quantidade de bytes do chunk pois esse diz
		//   o tamanho de todo o conteúdo e não o próprio chunk
		memcpy(Header->Data.PrimaryChunk.Bytes + sizeof(ACR_WAV_ChunkHeader_t), ContentBytes, ContentBufferSize);
		return;
	}

	HLP_Log("Lendo conteúdo do chunk '%s'", ChunkHeader->Data.ChunkId);

	short int IsFmtChunk = ACR_WAV_IsChunkId(ChunkHeader, ACR_WAV_CHUNK_ID_FMT);
	short int IsFactChunk = ACR_WAV_IsChunkId(ChunkHeader, ACR_WAV_CHUNK_ID_FACT);
	short int IsDataChunk = ACR_WAV_IsChunkId(ChunkHeader, ACR_WAV_CHUNK_ID_DATA);

	unsigned char *ReferChunkBytes;
	int			   ChunkSize;

	if (IsFmtChunk)
	{
		ReferChunkBytes = Header->Data.FormatChunk.Bytes;
		ChunkSize = sizeof(Header->Data.FormatChunk.Bytes);
	}
	else if (IsFactChunk)
	{
		ReferChunkBytes = Header->Data.FactChunk.Bytes;
		ChunkSize = sizeof(Header->Data.FactChunk.Bytes);
	}
	else if (IsDataChunk)
	{
		ReferChunkBytes = Header->Data.DataChunk.Bytes;
		ChunkSize = sizeof(Header->Data.DataChunk.Bytes);
	}

	ACR_WAV_WriteToChunk(ReferChunkBytes, ChunkSize, ChunkHeader, File);
}

static short int ACR_WAV_IsChunkId(union ACR_WAV_ChunkHeader *ChunkHeader, char ChunkId[ACR_WAV_CHUNK_ID_SIZE])
{
	int IsSame = strncmp(ChunkHeader->Data.ChunkId, ChunkId, ACR_WAV_CHUNK_ID_SIZE) == 0;
	return IsSame;
}

void ACR_WAV_WriteToChunk(unsigned char *Chunk, int ChunkSize, union ACR_WAV_ChunkHeader *ChunkHeader, FILE *File)
{
	HLP_Log("Armazenando dados do chunk em uma estrutura");

	short int ReadCount = 0;  // = 1: Sucesso | < 1: Erro
	int		  ChunkHeaderSize = sizeof(ACR_WAV_ChunkHeader_t);
	int		  ContentSize = ChunkSize - ChunkHeaderSize;
	char	  Content[ContentSize];

	// Copia o cabeçalho para o chunk
	memcpy(Chunk, ChunkHeader->Bytes, ChunkHeaderSize);

	// Escreve o conteúdo do chunk e conta quantas vezes leu
	ReadCount = fread(Content, ContentSize, 1, File);

	// Se não foi lido nenhum conteúdo
	if (ReadCount < 1)
	{
		HLP_LogError("Não foi possível ler o conteúdo original do chunk '%s' (tamanho: %d)\n",
					 ChunkHeader->Data.ChunkId, ContentSize);
	}

	// Copia o conteúdo para o chunk
	memcpy(Chunk + ChunkHeaderSize, &Content, ContentSize);
}