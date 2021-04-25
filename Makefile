# !!!!!!!!!!!!!!!!    Make sure this points to the cirq project    !!!!!!!!!!!!!!!!
CIRQ_PATH = ../cirq


# The path of the executable
OUTPUT_EXEC = build/main

# Library paths
INCLUDE_PATH = ${CIRQ_PATH}/src
LIBRARY_PATH = ${CIRQ_PATH}/build
LIBRARIES = cirq

# Use clang by default
CC = clang

# Flags for Clang
CFLAGS = -O0 -g -Weverything

# Disable annoying warnings
ANNOYING_WARNINGS = padded variadic-macros gnu-zero-variadic-macro-arguments
IGNORE_FLAGS = $(ANNOYING_WARNINGS:%=-Wno-%)

# Flags for GCC
GCC_FLAGS = -O0 -g -Wall -Wextra

# Default valgrind flags
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all -v

# Colors
RED := $(shell echo -ne "\e[0;31m")
LIGHT_RED := $(shell echo -ne "\e[0;91m")
GREEN := $(shell echo -ne "\e[0;32m")
LIGHT_GREEN := $(shell echo -ne "\e[0;92m")
YELLOW := $(shell echo -ne "\e[0;33m")
LIGHT_YELLOW := $(shell echo -ne "\e[0;93m")
BLUE := $(shell echo -ne "\e[0;34m")
LIGHT_BLUE := $(shell echo -ne "\e[0;94m")
MAGENTA := $(shell echo -ne "\e[0;35m")
LIGHT_MAGENTA := $(shell echo -ne "\e[0;95m")
CYAN := $(shell echo -ne "\e[0;36m")
LIGHT_CYAN := $(shell echo -ne "\e[0;96m")

WHITE := $(shell echo -ne "\e[0;97m")
LIGHT_GRAY := $(shell echo -ne "\e[0;37m")
DARK_GRAY := $(shell echo -ne "\e[0;90m")
BLACK := $(shell echo -ne "\e[0;30m")
RESET := $(shell echo -ne "\e[0m")

BOLD := $(shell echo -ne "\e[1m")

TAB := $(shell echo -ne "\t")

# All targets that are not files
.PHONY: help asan all shit release bench bench_rel bench_build valgrind valshit run clean remake cleanrun


# Show some help
help:
	@echo "make help      show this help"
	@echo "make all       build all files with ASAN on clang"
	@echo "make shit      build all files with ASAN on gcc"
	@echo "make release   build all files with -O3 on clang"
	@echo "make bench     build all files with ASAN on clang with benchmarking on"
	@echo "make bench_rel build all files with -O3 on clang with benchmarking on"
	@echo "make valgrind  build all files (clang), then run with valgrind"
	@echo "make valshit   build all files (gcc), then run with valgrind"
	@echo "make run       just run the executable"
	@echo "make clean     remove all build files"
	@echo "make remake    same as make clean; make all"
	@echo "make cleanrun  same as make remake, but runs formed executable directly after"
	@echo "make allrun    same as make all; make run"


# ASAN
asan: CFLAGS += -fsanitize=address
asan: $(OUTPUT_EXEC)


# Clang + ASAN
all: linkcirq asan


# Link cirq source directory to src/cirq
linkcirq:
	ln -rsf ${CIRQ_PATH}/src src/cirq


# GCC + ASAN
shit: CC = gcc
shit: CFLAGS = $(GCC_FLAGS)
shit: asan


release: CFLAGS = -O3 -DIGNORE_ASSERT
release: build/release

build/release: $(OUTPUT_EXEC)
	@mv $(OUTPUT_EXEC) build/release


# Debug + Clang + ASAN
bench_build: CFLAGS += -DBENCH
bench_build: cleanrun


bench:
	@rm -f /tmp/bench
	@touch /tmp/bench
	@make --no-print-directory bench_build
	@rm -f /tmp/bench.tmp
	@sort -nr /tmp/bench
	@rm -f /tmp/bench


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valgrind: clean $(OUTPUT_EXEC)
	valgrind $(VALGRIND_FLAGS) $(OUTPUT_EXEC)


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valshit: CC = gcc
valshit: CFLAGS = $(GCC_FLAGS)
valshit: valgrind


# Just run the executable
run:
	$(OUTPUT_EXEC)


# Clean all build files
clean:
	@echo "${RED}${BOLD}rm -f $(OUTPUT_EXEC) build/*${RESET}"
	@rm -rf $(OUTPUT_EXEC) build/*
	@mkdir -p build/tests


# Remove all files, then build again
remake: clean all


# Remove all files, build, then run
cleanrun: remake
	@echo -e "\n\n"
	$(OUTPUT_EXEC)

# Build all, then run
allrun: all run


# Find all C files
C_FILES = $(shell find src -iname '*.c')
C_FILES_O = $(C_FILES:src/%.c=build/%.o)

# Build C files
$(C_FILES_O): build/%.o: src/%.c
	@echo "${BOLD}$(CC)${RESET}${TAB}${LIGHT_YELLOW}$(CFLAGS)"
	@echo "${TAB}${DARK_GRAY}$(IGNORE_FLAGS)"
	@echo -n "${TAB}${LIGHT_CYAN}-c" $< "${TAB}${LIGHT_GREEN}-o" $@
	@echo " ${TAB}${LIGHT_MAGENTA}$(INCLUDE_PATH:%=-I %)"
	@echo "${RESET}"
	@$(CC)  $(CFLAGS) $(IGNORE_FLAGS) -c $< -o $@  $(INCLUDE_PATH:%=-I %)


# Find all header files
HEADER_FILES = $(shell find src -iname '*.h')
HEADER_FILES_O = $(HEADER_FILES:src/%.h=build/%.o)

# Add header files to c object
$(HEADER_FILES_O): build/%.o: src/%.h


# Build main executable
$(OUTPUT_EXEC): $(C_FILES_O) $(HEADER_FILES)
	@echo ""
	@echo "${TAB}${TAB}${TAB}${BOLD}Building cirq archive${RESET}"
	@echo ""

	@test -d ${CIRQ_PATH}/src || (echo "${RED}${BOLD}${CIRQ_PATH}/src does not exist!${RESET}" && exit 1)

	@# Don't build main.o file, since it will be deleted anyhow
	@touch ${CIRQ_PATH}/build/main.o

	@# Make archive
	@(cd ${CIRQ_PATH}; make --no-print-directory archive)

	@echo ""
	@echo "${TAB}${TAB}${TAB}${BOLD}Linking everything together${RESET}"
	@echo ""

	@echo "${BOLD}$(CC)${RESET}${TAB}${LIGHT_YELLOW}$(CFLAGS)"
	@echo "${TAB}${DARK_GRAY}$(IGNORE_FLAGS)"
	@echo "${TAB}${LIGHT_GREEN}$(C_FILES_O)"
	@echo -n "${TAB}${LIGHT_MAGENTA}$(LIBRARY_PATH:%=-L %) $(LIBRARIES:%=-l %)"
	@echo " ${TAB}${LIGHT_BLUE}-o $(OUTPUT_EXEC)"
	@echo "${RESET}"
	@$(CC)  $(CFLAGS) $(IGNORE_FLAGS) $(C_FILES_O) $(LIBRARY_PATH:%=-L %) $(LIBRARIES:%=-l %) -o $(OUTPUT_EXEC)
