# Compiler
CC = clang++

# One of the following options: binary (for executables), shared (for shared libraries -- DLLs) or static (for static libraries).
OUTPUT_TYPE = binary

# Name of the resulting file.
OUTPUT_NAME = tool.exe

# Paths
INCLUDE_PATH = -Iinclude
LIBRARY_PATH =
SRC_PATH = src
OBJ_PATH = obj

# Compiler flags and linker options
CFLAGS = $(INCLUDE_PATH) -g -O0 -Wall -std=c++14
LFLAGS = $(LIBRARY_PATH) -lWs2_32

# Conditional options
ifeq ($(OUTPUT_TYPE), shared)
CFLAGS += -fPIC
endif
ifeq ($(CC), clang++)
CFLAGS += -fcxx-exceptions -fms-compatibility-version=19
endif

# Select the appropriate linking commands
ifeq ($(OUTPUT_TYPE), binary)
LINK = $(CC) -o
endif
ifeq ($(OUTPUT_TYPE), shared)
LINK = $(CC) -shared -o
endif
ifeq ($(OUTPUT_TYPE), static)
ifeq ($(CC), g++)
LINK = ar rcs
endif
ifeq ($(CC), clang++)
LINK = llvm-ar rcs
endif
endif

#
OBJECTS = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, \
          $(wildcard $(SRC_PATH)/*.cpp))

all: $(OUTPUT_NAME)

# Create directory for object files
$(OBJECTS): | $(OBJ_PATH)
$(OBJ_PATH):
	mkdir $(OBJ_PATH)

# Compilation
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

# Linking
$(OUTPUT_NAME): $(OBJECTS)
	$(LINK) $@ $^ $(LFLAGS)

# Clean logic
.PHONY: clean
clean:
	del $(OUTPUT_NAME) $(OBJ_PATH)\*.o