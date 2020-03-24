CXX := @mpic++
SRC := $(shell find src/ -type f -name "*.cpp" -exec basename {} \;)
OBJ := $(patsubst %.cpp, obj/%.obj, $(SRC))
DEP := $(SRC:%.cpp=dep/%.d)
INCLUDE := -I inc
CXX_FLAGS = -std=c++2a -Wall -MT $@ -MMD -MP -MF dep/$*.d $(INCLUDE) -c
LD_FLAGS := -lOpenCL -lpng

.PHONY: all
all: bin/frac_cluster

bin/frac_cluster: $(OBJ)
	$(CXX) $(LD_FLAGS) $(OBJ) -o $@

obj/%.obj: src/%.cpp dep/%.d
	$(CXX) $(CXX_FLAGS) $< -o $@
	
obj/%.obj: src/MPI/%.cpp dep/%.d
	$(CXX) $(CXX_FLAGS) -I inc/MPI $< -o $@

$(DEP):

include $(wildcard $(DEP))

.PHONY: clean
clean:
	@$(RM) -f dep/* obj/* bin/*