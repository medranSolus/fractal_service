OUT_DIR := bin/

.PHONY: all
all: create_out_dir cluster bench

.PHONY: create_out_dir
create_out_dir:
	@mkdir -p $(OUT_DIR)

.PHONY: cluster
cluster:
	@cd cluster && $(MAKE) --no-print-directory ROOT_DIR=$(CURDIR) BIN_DIR=$(OUT_DIR)

.PHONY: bench
bench:
	@cd benchmark && $(MAKE) --no-print-directory ROOT_DIR=$(CURDIR) BIN_DIR=$(OUT_DIR)

.PHONY: clean
clean:
	@cd common && $(MAKE) clean --no-print-directory
	@cd cluster && $(MAKE) clean --no-print-directory
	@cd benchmark && $(MAKE) clean --no-print-directory
	@$(RM) -rf $(OUT_DIR)*
