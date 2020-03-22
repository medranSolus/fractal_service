CXX = @mpic++
OBJ = $(patsubst %.cpp, obj/%.obj, $(shell find src/ -type f -name "*.cpp" -exec basename {} \;))
INCLUDE = -I inc
CXX_FLAGS = -std=c++2a -Wall $(INCLUDE) -c
LD_FLAGS = -lOpenCL -lpng

.PHONY: all
all: bin/frac_cluster

bin/frac_cluster: $(OBJ)
	$(CXX) $(LD_FLAGS) $(OBJ) -o $@

obj/%.obj: src/%.cpp
	$(CXX) $(CXX_FLAGS) $< -o $@

.PHONY: clean
clean:
	@$(RM) -f obj/* bin/*