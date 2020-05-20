ROOT_DIR ?= $(CURDIR)
BIN_DIR ?= bin/

COM_DIR := $(ROOT_DIR)/common/
OBJ_COM_DIR := $(COM_DIR)$(OBJ_DIR)

OUT_DIR := $(ROOT_DIR)/$(BIN_DIR)
SRC_DIR := src/
SRC_CL_DIR := src_cl/
DEP_DIR := dep/
OBJ_DIR := obj/
OBJ_CL_DIR := $(OUT_DIR)opencl/

SRC := $(shell find $(SRC_DIR) -type f -name "*.cpp" -exec basename {} \;)
DEP := $(SRC:%.cpp=$(DEP_DIR)%.d)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)%.obj, $(SRC))
OBJ_CL := $(patsubst %.cl, $(OBJ_CL_DIR)%.cl, $(shell find $(SRC_CL_DIR) -type f -name "*.cl" -exec basename {} \;))
OBJ_COM = $(shell find $(OBJ_COM_DIR) -type f -name "*.obj")

INC := -I inc/
INC_COM := -I $(COM_DIR)inc/
CXX_FLAGS = -std=c++2a -Wall -MT $@ -MMD -MP -MF $(DEP_DIR)$*.d $(INC) $(INC_COM) -c
LD_FLAGS := -lOpenCL -lpng
CXX := @mpic++

.PHONY: all
all: create_dirs $(OBJ_CL) common $(OUT_DIR)fractal_cluster

.PHONY: create_dirs
create_dirs:
	@mkdir -p $(DEP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_CL_DIR)

.PHONY: common
common:
	@cd $(ROOT_DIR)/common && $(MAKE) --no-print-directory CXX=$(CXX)

$(OUT_DIR)fractal_cluster: $(OBJ)
	$(CXX) $(LD_FLAGS) $(OBJ_COM) $(OBJ) -o $@

$(OBJ_CL_DIR)%.cl: $(SRC_CL_DIR)%.cl
	@cp $< $@

$(OBJ_DIR)%.obj: $(SRC_DIR)%.cpp $(DEP_DIR)%.d
	$(CXX) $(CXX_FLAGS) $< -o $@

$(OBJ_DIR)%.obj: $(SRC_DIR)MPI/%.cpp $(DEP_DIR)%.d
	$(CXX) $(CXX_FLAGS) $(INC)MPI $< -o $@

$(DEP):

include $(wildcard $(DEP))

.PHONY: clean
clean:
	@$(RM) -rf $(DEP_DIR)* $(OBJ_DIR)*