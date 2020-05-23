ROOT_DIR ?= $(CURDIR)
BIN_DIR ?= bin/

COM_DIR := $(ROOT_DIR)/common/
OBJ_COM_DIR := $(COM_DIR)$(OBJ_DIR)

OUT_DIR := $(ROOT_DIR)/$(BIN_DIR)
SRC_DIR := src/
DEP_DIR := dep/
OBJ_DIR := obj/

SRC := $(shell find $(SRC_DIR) -type f -name "*.cpp" -exec basename {} \;)
DEP := $(SRC:%.cpp=$(DEP_DIR)%.d)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)%.obj, $(SRC))
OBJ_COM = $(shell find $(OBJ_COM_DIR) -type f -name "*.obj")

INC := -I inc/
INC_COM := -I $(COM_DIR)inc/
CXX_FLAGS = -std=c++2a -Wall -MT $@ -MMD -MP -MF $(DEP_DIR)$*.d $(INC) $(INC_COM) -c
LD_FLAGS := -pthread
CXX := @g++

.PHONY: all
all: create_dirs common $(OUT_DIR)cluster_test

.PHONY: create_dirs
create_dirs:
	@mkdir -p $(DEP_DIR)
	@mkdir -p $(OBJ_DIR)

.PHONY: common
common:
	@cd $(ROOT_DIR)/common && $(MAKE) --no-print-directory CXX=$(CXX)

$(OUT_DIR)cluster_test: $(OBJ)
	$(CXX) $(LD_FLAGS) $(OBJ_COM) $(OBJ) -o $@

$(OBJ_DIR)%.obj: $(SRC_DIR)%.cpp $(DEP_DIR)%.d
	$(CXX) $(CXX_FLAGS) $< -o $@

$(DEP):

include $(wildcard $(DEP))

.PHONY: clean
clean:
	@$(RM) -rf $(DEP_DIR)* $(OBJ_DIR)*