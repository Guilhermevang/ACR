# ACR - Audio Content Reader

O **ACR** é um módulo do sistema modular **cAM-Sys** focado na leitura de dados de conteúdo de áudio em arquivos. Ele abstrai a complexidade de ler e interpretar os dados de áudio, permitindo que outros módulos se concentrem no processamento e manipulação dos dados.

## **Recursos**
- Suporte inicial para o formato **WAV**.
- Leitura e abstração de conteúdo em chunks de dados.
- Integração com outros módulos do **cAM-Sys**.

## **Instalação**

### **1. Clonando o Repositório**
Adicione o ACR como um submódulo ao seu projeto:
```bash
git submodule add https://github.com/guilhermevang/ACR modules/acr
