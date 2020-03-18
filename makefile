CXX = @mpic++
OBJ = $(patsubst %.cpp, obj/%.obj, $(shell find src/ -type f -name "*.cpp" -exec basename {} \;))

.PHONY: all
all: bin/frac_cluster

bin/frac_cluster: $(OBJ)
	$(CXX) $(OBJ) -o $@

obj/%.obj: src/%.cpp
	$(CXX) -c -Wall $< -o $@

.PHONY: clean
clean:
	@$(RM) -f obj/* bin/*