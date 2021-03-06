OUT_DIR := bin/
MSG := @echo

.PHONY: all
all: cluster
	$(MSG) "Fractal service operational."

.PHONY: create_out_dir
create_out_dir:
	@mkdir -p $(OUT_DIR)

.PHONY: cluster
cluster: create_out_dir
	@cd cluster && $(MAKE) --no-print-directory ROOT_DIR=$(CURDIR) BIN_DIR=$(OUT_DIR)
	$(MSG) "Cluster assembled."

.PHONY: manager
manager: cluster
	@cd manager && $(MAKE) --no-print-directory OUT_DIR=$(CURDIR)/$(OUT_DIR)

.PHONY: bench
bench: cluster
	@cd benchmark && $(MAKE) --no-print-directory ROOT_DIR=$(CURDIR) BIN_DIR=$(OUT_DIR)
	$(MSG) "Benchmark created."

.PHONY: clean
clean:
	@cd common && $(MAKE) clean --no-print-directory
	@cd cluster && $(MAKE) clean --no-print-directory	
	@cd manager && $(MAKE) clean --no-print-directory
	@cd benchmark && $(MAKE) clean --no-print-directory
	@$(RM) -rf $(OUT_DIR)*
