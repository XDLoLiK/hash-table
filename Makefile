BUILD = debug

CXX = g++
CXXFLAGS = -Wall -Wextra -no-pie -msse4.2 -mavx2 -I $(INC_DIR)

CC = gcc
CFLAGS = -Wall -Wextra -no-pie -msse4.2 -mavx2 -I $(INC_DIR)

AS = nasm
ASFLAGS = -felf64

ifeq ($(BUILD), debug)
	CFLAGS += -O0 -g -fdiagnostics-color=always
	CXXFLAGS += -O0 -g -fdiagnostics-color=always
	BUILD_DIR = build-debug
else
	CFLAGS += -s
	ASFLAGS += -s
	CXXFLAGS += -s
	BUILD_DIR = build-release
endif

SRC_DIR  = ./src
BIN_DIR  = ./bin
INC_DIR  = ./inc

SRC =  $(notdir $(shell find $(SRC_DIR) -name "*.c"))
ASM =  $(notdir $(shell find $(SRC_DIR) -name "*.asm"))
OBJ =  $(addprefix $(BIN_DIR)/, $(SRC:.c=.o))
OBJ += $(addprefix $(BIN_DIR)/, $(ASM:.asm=.o))

$(BUILD_DIR)/hash_table.out: $(OBJ) | folders
	$(CC) -o $@ $^ $(CFLAGS)

-include $(addprefix $(BIN_DIR)/, $(SRC:.c=.d))
-include $(addprefix $(BIN_DIR)/, $(SRC:.asm=.d))

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | folders
	$(CC) -MD -c -o $@ $< $(CFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm | folders
	$(AS) -MD -o $@ $< $(ASFLAGS)

run:
	$(BUILD_DIR)/hash_table.out

.PHONY: clean folders

folders:
	[ ! -d $(BUILD_DIR) ] && mkdir $(BUILD_DIR) || true
	[ ! -d $(BIN_DIR) ]   && mkdir $(BIN_DIR)   || true

clean:
	[ -d $(BIN_DIR) ] && rm -rf $(BIN_DIR) || true
