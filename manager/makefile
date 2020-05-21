OUT_DIR ?= bin/

BUILD_DIR := build/

.PHONY: all
all: create_dirs $(OUT_DIR)fractal_manager

.PHONY: create_dirs
create_dirs:
	@mkdir -p $(BUILD_DIR)

$(OUT_DIR)fractal_manager:
	@cmake -S . -B $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(MAKE) --no-print-directory
	@mv $(BUILD_DIR)fractal_manager $@
	@$(RM) -f $(BUILD_DIR)*.a

.PHONY: clean
clean:
	@$(RM) -rf $(BUILD_DIR)*