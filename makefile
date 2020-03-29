CXX := @mpic++
SRC := $(shell find src/ -type f -name "*.cpp" -exec basename {} \;)
OBJ := $(patsubst %.cpp, obj/%.obj, $(SRC))
SRC_CL := $(shell find src_cl/ -type f -name "*.cl" -exec basename {} \;)
OBJ_CL := $(patsubst %.cl, bin/opencl/%.cl, $(SRC_CL))
DEP := $(SRC:%.cpp=dep/%.d)
INCLUDE := -I inc
CXX_FLAGS = -std=c++2a -Wall -MT $@ -MMD -MP -MF dep/$*.d $(INCLUDE) -c
LD_FLAGS := -lOpenCL -lpng

.PHONY: all
all: create_dirs $(OBJ_CL) bin/fractal_cluster

.PHONY: create_dirs
create_dirs:
	@mkdir -p bin/opencl

bin/opencl/%.cl: src_cl/%.cl
	@cp $< $@

bin/fractal_cluster: $(OBJ)
	$(CXX) $(LD_FLAGS) $(OBJ) -o $@

obj/%.obj: src/%.cpp dep/%.d
	$(CXX) $(CXX_FLAGS) $< -o $@

obj/%.obj: src/MPI/%.cpp dep/%.d
	$(CXX) $(CXX_FLAGS) -I inc/MPI $< -o $@

obj/%.obj: src/Net/%.cpp dep/%.d
	$(CXX) $(CXX_FLAGS) -I inc/Net $< -o $@

$(DEP):

include $(wildcard $(DEP))

.PHONY: clean
clean:
	@$(RM) -rf dep/* obj/* bin/*