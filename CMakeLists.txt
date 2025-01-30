cmake_minimum_required(VERSION 3.16)
project(ACR VERSION 1.0 LANGUAGES C)

# Adiciona o diretório de cabeçalhos públicos
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/hlp/include)

# Adiciona subdiretório da HLP
add_subdirectory(modules/hlp)

# Encontra todos os arquivos .c no diretório src/ (exceto main.c)
file(GLOB_RECURSE ACR_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.c)
list(FILTER ACR_SOURCES EXCLUDE REGEX "src/main\\.c") # Excluir main.c do conjunto de fontes da biblioteca

# Cria a biblioteca ACR
add_library(acr ${ACR_SOURCES})
target_link_libraries(acr cam_hlp)

set(CMAKE_BUILD_TYPE Debug)  # Define o modo de compilação como Debug

# Cria o executável ACR
add_executable(acr_exec src/main.c)
target_link_libraries(acr_exec acr cam_hlp)

# Adiciona os testes
file(GLOB ACR_TESTS tests/*.c)
if (ACR_TESTS)
    add_executable(acr_tests ${ACR_TESTS})
    target_link_libraries(acr_tests acr cam_hlp)
endif()