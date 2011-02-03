# ==========================
# BEDTools Makefile
# (c) 2009 Aaron Quinlan
# ==========================

# define our object and binary directories
export OBJ_DIR	= obj
export BIN_DIR	= bin
export SRC_DIR	= src
export CXX		= g++
export CXXFLAGS = -Wall -O2
export LIBS		= -lz




SUBDIRS = $(SRC_DIR)/groupBy \
          $(SRC_DIR)/shuffle \

UTIL_SUBDIRS =	$(SRC_DIR)/common/tabFile \
				$(SRC_DIR)/common/gzstream \
				$(SRC_DIR)/common/fileType \

all:
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	
	@echo "========================================================="
	@echo "Building filo:"
	@echo "========================================================="
	
	@for dir in $(UTIL_SUBDIRS); do \
		echo "- Building in $$dir"; \
		$(MAKE) --no-print-directory -C $$dir; \
		echo ""; \
	done

	@for dir in $(SUBDIRS); do \
		echo "- Building in $$dir"; \
		$(MAKE) --no-print-directory -C $$dir; \
		echo ""; \
	done


.PHONY: all

clean:
	@echo "Cleaning up."
	@rm -f $(OBJ_DIR)/* $(BIN_DIR)/*

.PHONY: clean
