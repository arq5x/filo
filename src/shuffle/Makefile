UTILITIES_DIR = ../common/
OBJ_DIR = ../../obj/
BIN_DIR = ../../bin/


# -------------------
# define our includes
# -------------------
INCLUDES = 

# ----------------------------------
# define our source and object files
# ----------------------------------
SOURCES= shuffle.cpp
OBJECTS= $(SOURCES:.cpp=.o)
EXT_OBJECTS=$(patsubst %,$(OBJ_DIR)/%
PROGRAM= shuffle


all: $(PROGRAM)

.PHONY: all


$(PROGRAM):
	@echo "  * linking $(PROGRAM)"
	@$(CXX) $(LDFLAGS) $(CXXFLAGS) $(SOURCES) -o $(BIN_DIR)/$@

