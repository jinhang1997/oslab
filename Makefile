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
COMMON := $(OBJ_DIR)/common.o

# Lists of File I/O part
SRCS_FILE = $(shell find $(SRC_DIR)/file -name "*.c")
OBJS_FILE := $(SRCS_FILE:$(SRC_DIR)/file/%.c=$(OBJ_DIR)/file/%.o)
FILEIO := $(OBJS_FILE:$(OBJ_DIR)/file/%.o=$(BUILD_DIR)/file/%)

# Lists of Multi-proc part
PROC_MAINS := $(shell find $(SRC_DIR)/proc -name "*main.c")
PROC_COMPS := $(shell find $(SRC_DIR)/proc/comp -name "*.c")
SRCS_PROC := $(PROC_MAINS) $(PROC_COMPS)
OBJS_COMPS := $(PROC_COMPS:$(SRC_DIR)/proc/comp/%.c=$(OBJ_DIR)/proc/comp/%.o)
OBJS_PROC := $(SRCS_PROC:$(SRC_DIR)/proc/%.c=$(OBJ_DIR)/proc/%.o)
#OBJS_KINSHELL := $(OBJ_DIR)/proc/comp/kinshell/*.o
MULTIPROC := $(PROC_MAINS:$(SRC_DIR)/proc/%main.c=$(BUILD_DIR)/proc/%main)

# Don't remove *.o files automatically
.SECONDARY: $(OBJS)

all: $(COMMON) $(FILEIO) $(MULTIPROC) 

# Compile each *.c file as *.o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	
# Link each *.o file in fileio as executable files
$(BUILD_DIR)/file/%: $(OBJ_DIR)/file/%.o $(COMMON)
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) $(CFLAGS) -o $@ $^ 

# Rule to build mysysmain
$(BUILD_DIR)/proc/mysysmain: $(OBJ_DIR)/proc/mysysmain.o $(OBJ_DIR)/proc/comp/mysys.o $(COMMON)
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LFLAGS)
	
# Rule to build Kin Shell
$(BUILD_DIR)/proc/kinshellmain: $(OBJ_DIR)/proc/kinshellmain.o $(OBJS_COMPS)  $(COMMON) 
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LFLAGS) 
	
.PHONY: all fileio proc kinshell lines push clean

fileio: $(COMMON) $(FILEIO)

proc: $(COMMON) $(MULTIPROC) 

kinshell: proc
	@$(BUILD_DIR)/proc/kinshellmain

lines:
	@echo Total Code Lines: $(shell find ./ -name '*.[c|h]' | xargs cat | wc -l)

push:
	git add .
	git commit --author='oslab-tracer <>' -m 'auto commit at $(shell date +"%Y-%m-%d %H:%M:%S")' --no-verify --allow-empty
	git push origin master

clean:
	rm -rf $(BUILD_DIR)
