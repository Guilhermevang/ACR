# Variáveis
CC = gcc                    # Compilador
CFLAGS = -lm                # Flags (referenciar a biblioteca math por exemplo)
SRC = main.c            # Arquivo de origem
OUT = dist/main             # Executável de saída

# Alvo padrão (executado ao rodar o comando "make")
all: clean $(OUT)

# Regra para compilar o programa
$(OUT): $(SRC)
	mkdir -p dist
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

# Limpar arquivos compilados
clean:
	rm -rf dist/

# Rodar o programa
run: $(OUT)
	./$(OUT)
