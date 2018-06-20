# File paths
SRC_DIR := ./src
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj
INC_DIR := ./include

# Compilation flags
CC := gcc
LD := gcc
CFLAGS := -Wall -I $(INC_DIR)
LFLAGS := $(CFLAGS) -lreadline 

# All files to be compiled
SRCS = $(shell find $(SRC_DIR)/ -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS_ALL := $(OBJ_DIR)/*.o

# The commonly depended file
# COMMON := $(OBJ_DIR)/common.o

# Lists of File I/O part
SRCS_FILE = $(shell find $(SRC_DIR)/file -name "*.c")
OBJS_FILE := $(SRCS_FILE:$(SRC_DIR)/file/%.c=$(OBJ_DIR)/file/%.o)
FILEIO := $(OBJS_FILE:$(OBJ_DIR)/file/%.o=$(BUILD_DIR)/%)

# Lists of Multi-proc part
SRCS_PROCMAINS := $(shell find $(SRC_DIR)/proc -name "*main.c")
SRCS_KINSHELL := $(shell find $(SRC_DIR)/proc/kinshell -name "*.c")
SRCS_PROC := $(SRCS_PROCMAINS) $(SRCS_KINSHELL)
#OBJS_COMPS := $(PROC_COMPS:$(SRC_DIR)/proc/comp/%.c=$(OBJ_DIR)/proc/comp/%.o)
OBJS_KINSHELL := $(SRCS_KINSHELL:$(SRC_DIR)/proc/kinshell/%.c=$(OBJ_DIR)/proc/kinshell/%.o)
OBJS_PROC := $(SRCS_PROC:$(SRC_DIR)/proc/%.c=$(OBJ_DIR)/proc/%.o)
MULTIPROC := $(SRCS_PROCMAINS:$(SRC_DIR)/proc/%main.c=$(BUILD_DIR)/%main)

# Don't remove *.o files automatically
.SECONDARY: $(OBJS)

all: $(FILEIO) $(MULTIPROC) 

# Compile each *.c file as *.o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	
# Link each *.o file in fileio as executable files
$(BUILD_DIR)/%: $(OBJ_DIR)/file/%.o
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) $(CFLAGS) -o $@ $^ 

# Rule to build mysysmain
$(BUILD_DIR)/mysysmain: $(OBJ_DIR)/proc/mysysmain.o
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LFLAGS)
	
# Rule to build Kin Shell
$(BUILD_DIR)/kinshellmain: $(OBJ_DIR)/proc/kinshellmain.o $(OBJS_KINSHELL) 
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LFLAGS) 
	
.PHONY: all fileio proc kinshell lines push clean

fileio: $(FILEIO)

proc: $(MULTIPROC) 

kinshell: proc
	@$(BUILD_DIR)/kinshellmain

lines:
	@echo Total Code Lines: $(shell find ./ -name '*.[c|h]' | xargs cat | wc -l)

push:
	git add .
	git commit --author='oslab-tracer <>' -m 'auto commit at $(shell date +"%Y-%m-%d %H:%M:%S")' --no-verify --allow-empty
	git push origin master

clean:
	rm -rf $(BUILD_DIR)
