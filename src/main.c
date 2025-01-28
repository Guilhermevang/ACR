// Fontes:
// Cabeçalho WAV: http://soundfile.sapp.org/doc/WaveFormat/
// Unions: https://www.programiz.com/c-programming/c-unions
// Áudios de exemplo:
// Sobre o 'fact' em WAV não-PCM: https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helper-functions.h"
#include "resource-manager.h"
#include "wav_header.h"

int main()
{
    char file_name[] = "/home/agroflux/Documentos/Pessoal/C-Projects/audio-manipulation/c-audio-manipulation/src/addf8-Alaw-GW.wav";
    union wav_header_chunks chunks;

    // Abre o arquivo WAV
    FILE *file_in = open_file(file_name);

    // Lê o cabeçalho do arquivo
    read_wav_header(file_in, &chunks);

    // Libera a memória alocada na heap
    free_all_resources();

    exit(EXIT_SUCCESS);
}