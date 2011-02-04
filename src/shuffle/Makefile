UTILITIES_DIR = ../common/
OBJ_DIR = ../../obj/
BIN_DIR = ../../bin/


# -------------------
# define our includes
# -------------------
INCLUDES = -I$(UTILITIES_DIR)/version/

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
	@$(CXX) $(LDFLAGS) $(CXXFLAGS) $(SOURCES) $(INCLUDES) -o $(BIN_DIR)/$@

