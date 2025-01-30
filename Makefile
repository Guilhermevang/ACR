# Diretórios
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

# Executáveis
EXEC = acr
TEST_EXEC = acr_test

# Comandos
CMAKE = cmake
MAKE = make

# Alvo padrão (build)
.PHONY: all
all: build

# Configurar e compilar o projeto
.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) .. && $(MAKE)

# Rodar o executável principal
.PHONY: run
run: build
	@$(BUILD_DIR)/$(EXEC)

# Rodar os testes
.PHONY: test
test: build
	@$(BUILD_DIR)/$(TEST_EXEC)

# Limpar arquivos gerados
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

# Recompilar tudo
.PHONY: rebuild
rebuild: clean build
