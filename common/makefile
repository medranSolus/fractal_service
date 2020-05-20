CXX ?= @g++

SRC_DIR := src/
DEP_DIR := dep/
OBJ_DIR := obj/

SRC := $(shell find $(SRC_DIR) -type f -name "*.cpp" -exec basename {} \;)
DEP := $(SRC:%.cpp=$(DEP_DIR)%.d)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)%.obj, $(SRC))

INC := -I inc/
CXX_FLAGS = -std=c++2a -Wall -MT $@ -MMD -MP -MF $(DEP_DIR)$*.d $(INC) -c

.PHONY: all
all: create_dirs $(OBJ)

.PHONY: create_dirs
create_dirs:
	@mkdir -p $(DEP_DIR)
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.obj: $(SRC_DIR)%.cpp $(DEP_DIR)%.d
	$(CXX) $(CXX_FLAGS) $< -o $@

$(OBJ_DIR)%.obj: $(SRC_DIR)Net/%.cpp $(DEP_DIR)%.d
	$(CXX) $(CXX_FLAGS) $(INC)Net $< -o $@

$(DEP):

include $(wildcard $(DEP))

.PHONY: clean
clean:
	@$(RM) -rf $(DEP_DIR)* $(OBJ_DIR)*