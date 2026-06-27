BOARD         ?= frdm_mcxn947/mcxn947/cpu0
BUILD_DIR     ?= build
APP_DIR       ?= app
SERIAL_PORT   ?= /dev/tty.usbmodemDBSLGK210ZLLE3
BAUD          ?= 115200

# Runner: linkserver (default) or jlink (if you reflashed the debug probe)
RUNNER        ?= linkserver

# Zephyr environment — paths relative to this repo inside the workspace
ZEPHYR_BASE   ?= $(CURDIR)/../zephyr
ZEPHYR_VENV   ?= $(CURDIR)/../.venv

# Ensure west comes from the Zephyr venv
WEST          := $(ZEPHYR_VENV)/bin/west
PIP           := $(ZEPHYR_VENV)/bin/pip
PYTHON        := $(ZEPHYR_VENV)/bin/python3

export ZEPHYR_BASE

.PHONY: build flash debug clean console config venv deps help

## Create the Python virtual environment and install west
venv:
	python3 -m venv $(ZEPHYR_VENV)
	$(PIP) install --upgrade pip
	$(PIP) install west

## Install Zephyr Python dependencies (run after west update)
deps:
	$(PIP) install -r $(ZEPHYR_BASE)/scripts/requirements.txt

## Build the application
build:
	$(WEST) build -b $(BOARD) $(APP_DIR) -d $(BUILD_DIR)

## Build and flash
flash: build
	$(WEST) flash -d $(BUILD_DIR) -r $(RUNNER)

## Build, flash, and start GDB
debug: build
	$(WEST) debug -d $(BUILD_DIR) -r $(RUNNER)

## Open serial console
console:
	screen $(SERIAL_PORT) $(BAUD)

## Interactive Kconfig editor
config:
	$(WEST) build -d $(BUILD_DIR) -t menuconfig

## Remove build directory
clean:
	rm -rf $(BUILD_DIR)

## Show available targets
help:
	@echo "Zephyr FRDM-MCXN947 Makefile"
	@echo ""
	@echo "Setup:"
	@echo "  venv       - Create Python venv and install west"
	@echo "  deps       - Install Zephyr Python dependencies"
	@echo ""
	@echo "Build & Run:"
	@echo "  build      - Build the application"
	@echo "  flash      - Build and flash"
	@echo "  debug      - Build, flash, and start GDB"
	@echo "  console    - Open serial console (screen)"
	@echo "  config     - Interactive Kconfig editor"
	@echo "  clean      - Remove build directory"
	@echo ""
	@echo "Variables:"
	@echo "  BOARD       = $(BOARD)"
	@echo "  RUNNER      = $(RUNNER)"
	@echo "  SERIAL_PORT = $(SERIAL_PORT)"
	@echo "  BAUD        = $(BAUD)"
	@echo "  ZEPHYR_BASE = $(ZEPHYR_BASE)"
