#include "acr.h"

#include <stdio.h>
#include <string.h>

#include "acr_internal.h"
#include "acr_wav_chunks.h"
#include "hlp.h"

FILE *ACR_File = NULL;

int ACR_Init(const char *FilePath)
{
	HLP_Log("Inicializando serviço ACR (Audio Content Reader)");

	HLP_Log("Procurando por arquivo '%s'", FilePath);

	ACR_File = fopen(FilePath, "rb");
	if (!ACR_File)
	{
		HLP_LogError("Não foi possível encontrar o arquivo");
		return -1;	// Erro ao abrir o arquivo
	}

	HLP_Log("Arquivo encontrado e aberto");
	return 0;
}

int ACR_ReadHeader()
{
	if (!ACR_File)
	{
		HLP_LogError("Não foi possível encontrar o arquivo");
		return -1;	// Erro: parâmetros inválidos
	}

	HLP_Log("Executando leitura de cabeçalhos WAV");

	// Cabeçalho/Chunks do arquivo
	union ACR_WAV_Header Header;

	// Lẽ chunk por chunk do cabeçalho
	ACR_WAV_ReadHeaderChunks(ACR_File, &Header);

	return 0;
}

void ACR_Terminate(void)
{
	if (ACR_File)
	{
		fclose(ACR_File);
		ACR_File = NULL;
	}
}
