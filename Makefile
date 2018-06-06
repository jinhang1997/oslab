# File paths
SRC_DIR := ./src
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj
INC_DIR := ./include

# Compilation flags
CC := gcc
LD := gcc
CFLAGS := -Wall -I $(INC_DIR)

# Files to be compiled
#SRCS := $(wildcard $(SRC_DIR)/*.c)
SRCS = $(shell find $(SRC_DIR)/ -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS_ALL := $(OBJ_DIR)/*.o
#BUILD := $(OBJS:$(OBJ_DIR)/%.o=$(BUILD_DIR)/%)
FILEIO := $(OBJS:$(OBJ_DIR)/fileio/%.o=$(BUILD_DIR)/fileio/%)
MULTIPROC := $(OBJS:$(OBJ_DIR)/multiproc/%main.o=$(BUILD_DIR)/multiproc/%main)

# Don't remove *.o files automatically
.SECONDARY: $(OBJS_ALL)

all: $(FILEIO) $(MULTIPROC)

# Compile each *.c file as *.o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	
# Link each *.o file in fileio as executable files
$(BUILD_DIR)/fileio/%: $(OBJ_DIR)/fileio/%.o
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) $(CFLAGS) -o $@ $<

# Link needed *.o files in multiproc as target executables
$(BUILD_DIR)/multiproc/%main: $(OBJ_DIR)/multiproc/*.o
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) $(CFLAGS) -o $@ $<
	
.PHONY: all fileio multiproc lines push clean

fileio: $(FILEIO)

multiproc: $(MULTIPROC)

lines:
	@echo Total Code Lines: $(shell find ./ -name '*.[c|h]' | xargs cat | wc -l)

push:
	git add .
	git commit --author='oslab-tracer <>' -m 'auto commit at $(shell date +"%Y-%m-%d %H:%M:%S")' --no-verify --allow-empty
	git push origin master

clean:
	rm -rf $(BUILD_DIR)
