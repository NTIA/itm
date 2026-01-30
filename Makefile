# Makefile for ITM (Irregular Terrain Model) Project
# Compiles libitm.so/libitm.dylib (shared library) and ITMDrvr (command-line driver)

# Detect OS
UNAME_S := $(shell uname -s)

# Compiler and flags
CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra -O2 -std=c++11
CXXFLAGS_DEBUG = -fPIC -Wall -Wextra -g -std=c++11
LDFLAGS = -shared -lm
LDFLAGS_STATIC = -lm

# OS-specific settings
ifeq ($(UNAME_S),Linux)
  CXXFLAGS += -D_LINUX
  CXXFLAGS_DEBUG += -D_LINUX
  LIBITM_EXT = so
else ifeq ($(UNAME_S),Darwin)
  CXXFLAGS += -D_DARWIN
  CXXFLAGS_DEBUG += -D_DARWIN
  LIBITM_EXT = dylib
else
  # Default fallback to Linux
  CXXFLAGS += -D_LINUX
  CXXFLAGS_DEBUG += -D_LINUX
  LIBITM_EXT = so
endif

# Directories
INCLUDE_DIR = include
SRC_DIR = src
WIN32_DRVR_DIR = win32/ITMDrvr
BUILD_DIR = build
BIN_DIR = bin

# Source files for libitm library
LIBITM_SOURCES = \
	$(SRC_DIR)/ComputeDeltaH.cpp \
	$(SRC_DIR)/DiffractionLoss.cpp \
	$(SRC_DIR)/FindHorizons.cpp \
	$(SRC_DIR)/FreeSpaceLoss.cpp \
	$(SRC_DIR)/FresnelIntegral.cpp \
	$(SRC_DIR)/H0Function.cpp \
	$(SRC_DIR)/InitializeArea.cpp \
	$(SRC_DIR)/InitializePointToPoint.cpp \
	$(SRC_DIR)/InverseComplementaryCumulativeDistributionFunction.cpp \
	$(SRC_DIR)/itm_area.cpp \
	$(SRC_DIR)/itm_p2p.cpp \
	$(SRC_DIR)/KnifeEdgeDiffraction.cpp \
	$(SRC_DIR)/LinearLeastSquaresFit.cpp \
	$(SRC_DIR)/LineOfSightLoss.cpp \
	$(SRC_DIR)/LongleyRice.cpp \
	$(SRC_DIR)/QuickPfl.cpp \
	$(SRC_DIR)/SigmaHFunction.cpp \
	$(SRC_DIR)/SmoothEarthDiffraction.cpp \
	$(SRC_DIR)/TerrainRoughness.cpp \
	$(SRC_DIR)/TroposcatterLoss.cpp \
	$(SRC_DIR)/ValidateInputs.cpp \
	$(SRC_DIR)/Variability.cpp

# Source files for ITMDrvr executable
ITMDRVR_SOURCES = \
	$(WIN32_DRVR_DIR)/AreaMode.cpp \
	$(WIN32_DRVR_DIR)/Common.cpp \
	$(WIN32_DRVR_DIR)/ITMDrvr.cpp \
	$(WIN32_DRVR_DIR)/PointToPointMode.cpp \
	$(WIN32_DRVR_DIR)/Reporting.cpp

# Object files
LIBITM_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/libitm/%.o,$(LIBITM_SOURCES))
ITMDRVR_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/itmdrvr/%.o,$(ITMDRVR_SOURCES))

# Targets
LIBITM_TARGET = $(BIN_DIR)/libitm.$(LIBITM_EXT)
ITMDRVR_TARGET = $(BIN_DIR)/ITMDrvr

# Default target
all: $(LIBITM_TARGET)
	@echo ""
	@echo "✓ Build complete!"
	@echo ""
	@echo "Built targets:"
	@echo "  - $(LIBITM_TARGET)"

all-with-driver: $(LIBITM_TARGET) $(ITMDRVR_TARGET)

# Create output directories
$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

# Build libitm.so
$(LIBITM_TARGET): $(LIBITM_OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "✓ Built $(LIBITM_TARGET)"

# Build ITMDrvr
$(ITMDRVR_TARGET): $(ITMDRVR_OBJECTS) $(LIBITM_TARGET) | $(BIN_DIR)
	$(CXX) -o $@ $(ITMDRVR_OBJECTS) -L$(BIN_DIR) -litm $(LDFLAGS_STATIC)
	@echo "✓ Built $(ITMDRVR_TARGET)"

# Compile libitm source files
$(BUILD_DIR)/libitm/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	@echo "  Compiled $<"

# Compile ITMDrvr source files
$(BUILD_DIR)/itmdrvr/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(WIN32_DRVR_DIR) -c $< -o $@
	@echo "  Compiled $<"

# Debug build with symbols
debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: clean all
	@echo "✓ Debug build complete"

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "✓ Cleaned build artifacts"

# Install targets (optional)
install: all
	@mkdir -p /usr/local/lib /usr/local/include
	@cp $(LIBITM_TARGET) /usr/local/lib/
	@cp $(INCLUDE_DIR)/itm.h /usr/local/include/
	@cp $(INCLUDE_DIR)/Enums.h /usr/local/include/
	@cp $(INCLUDE_DIR)/Errors.h /usr/local/include/
	@cp $(INCLUDE_DIR)/Warnings.h /usr/local/include/
	@ldconfig
	@echo "✓ Installed libitm.so to /usr/local/lib"
	@echo "✓ Installed header files to /usr/local/include"

# Uninstall targets (optional)
uninstall:
	@rm -f /usr/local/lib/libitm.so
	@rm -f /usr/local/include/itm.h /usr/local/include/Enums.h /usr/local/include/Errors.h /usr/local/include/Warnings.h
	@ldconfig
	@echo "✓ Uninstalled ITM library and headers"

# Help target
help:
	@echo "ITM Project Makefile - Available targets:"
	@echo ""
	@echo "  make              - Build libitm.so (default)"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make install      - Install libitm.so (requires sudo)"
	@echo "  make uninstall    - Remove installed libraries (requires sudo)"
	@echo "  make help         - Display this help message"
	@echo ""
	@echo "NOTES:"
	@echo "  - ITMDrvr (command-line driver) requires Windows-specific code."
	@echo "  - To build ITMDrvr on Windows, use the Visual Studio solution:"
	@echo "    win32/itm.sln"
	@echo "  - The libitm.so library can be used with custom applications"
	@echo "    or wrapped with language bindings (Python, C#, etc.)"
	@echo ""

.PHONY: all debug clean install uninstall help
