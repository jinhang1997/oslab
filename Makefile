# File paths
SRC_DIR := ./src
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj

# Compilation flags
CC := gcc
LD := gcc
CFLAGS := -Wall

# Files to be compiled
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BUILD := $(OBJS:$(OBJ_DIR)/%.o=$(BUILD_DIR)/%)

# Don't remove *.o files automatically
.SECONDARY: $(OBJS)

all: $(BUILD)

# Compile each *.c file as *.o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@echo + CC $<
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $<
	
# Link each *.o file as executable files
$(BUILD_DIR)/%: $(OBJS)
	@echo + LD $@
	@mkdir -p $(BUILD_DIR)
	@$(LD) $(CFLAGS) -o $@ $<
	
.PHONY: all time push clean

time:
	@echo $(shell date +"%Y-%m-%d %H:%M:%S")

push:
	git add .
	git commit --author='auto-commit <abc@abc.com>' -m 'auto commit at $(shell date +"%Y-%m-%d %H:%M:%S")' --no-verify --allow-empty
	git push origin master

clean:
	rm -rf $(BUILD_DIR)
