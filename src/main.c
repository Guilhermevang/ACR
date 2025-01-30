#include <stdio.h>

#include "acr.h"

int main()
{
	ACR_Init("/home/agroflux/Documentos/Pessoal/C-Projects/audio-manipulation/ACR/src/addf8-Alaw-GW.wav");
	ACR_ReadHeader();
	ACR_Terminate();
}