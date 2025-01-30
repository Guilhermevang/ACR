#ifndef ACR_H
#define ACR_H

#include <stddef.h>
#include <stdint.h>

#include "acr_wav_header.h"

// Inicializa o leitor ACR
int ACR_Init(const char *file_path);

// Lê o próximo chunk do arquivo
int ACR_ReadHeader();

// Finaliza o leitor ACR
void ACR_Terminate(void);

#endif	// ACR_H
