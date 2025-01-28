cmake_minimum_required(VERSION 3.10)
project(CAudioReader)

# Configuração de Debug por padrão
set(CMAKE_BUILD_TYPE Debug)

# Inclui os diretórios com os cabeçalhos
include_directories(src)

# Lista os arquivos de origem
set(SOURCES
    src/main.c
    src/helper-functions.c
    src/logger.c
    src/resource-manager.c
    src/wav_header.c
)

# Cria o executável
add_executable(c-audio-reader ${SOURCES})

# Adiciona a biblioteca de matemática
target_link_libraries(c-audio-reader m)
