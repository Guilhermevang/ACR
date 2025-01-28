# C-WAV-Reader

Lê arquivos de áudio WAV usando a linguagem C.
O objetivo é aprender sobre a manipulação de arquivos de áudio WAVE.

## Compilação

### Em Linux e macOS (ou Windows com WSL/MinGW)
Certifique-se de ter o GCC instalado.

Para executar, primeiro você deve criar o diretório que conterá os arquivos compilados ('build/'). Use:
```bash
mkdir build # Cria o diretório build/
cd build    # Acessa o diretório
cmake ..
``` 

Para compilar, dentro do diretório `build/`, use:
```bash
make
```