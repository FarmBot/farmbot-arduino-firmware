rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

BUILD_DIR ?= $(shell pwd)/_build
BIN_DIR ?= $(shell pwd)/bin
FBARDUINO_FIRMWARE_SRC_DIR ?= src
FBARDUINO_FIRMWARE_BUILD_DIR ?= $(BUILD_DIR)/sketch
FBARDUINO_FIRMWARE_LIB_BUILD_DIR ?= $(BUILD_DIR)/libraries

ARDUINO_INSTALL_DIR ?= $(HOME)/arduino-1.8.5

# Files to be tracked for make to know to rebuild.
CXX_SRC := $(wildcard $(FBARDUINO_FIRMWARE_SRC_DIR)/*.cpp)
SRC := $(CXX_SRC)
SRC_DEPS := $(SRC) $(wildcard $(FBARDUINO_FIRMWARE_SRC_DIR)/*.h)

# Object files and Dependency files That will eventually be built.
CXX_OBJ := $(CXX_SRC:.cpp=.o)

## Commands needed to compile and whatnot.
CXX := $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/bin/avr-g++
CC := $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/bin/avr-gcc
AR := $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/bin/avr-gcc-ar
OBJ_COPY := $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/bin/avr-objcopy
MKDIR_P := mkdir -p

CXX_FLAGS := -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10600 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR
CFLAGS := -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections,--relax -mmcu=atmega2560

.DEFAULT_GOAL := all

## Dependencies
include lib/core.Makefile
include lib/SPI.Makefile
include lib/Servo.Makefile
include lib/EEPROM.Makefile

# Targets
include lib/targets/ramps_v14.Makefile
include lib/targets/farmduino_v10.Makefile
include lib/targets/farmduino_k14.Makefile

.PHONY: all clean \
	dep_core dep_core_clean \
	dep_Servo dep_Servo_clean \
	dep_SPI dep_SPI_clean \
	dep_EEPROM dep_EEPROM_clean \
	target_ramps_v14 target_ramps_v14_clean \
	target_farmduino_v10 target_farmduino_v10_clean \
	target_farmduino_k14 target_farmduino_k14_clean

DEPS := $(DEP_CORE) $(DEP_SPI) $(DEP_Servo) $(DEP_EEPROM)
DEPS_OBJ := $(DEP_SPI_OBJ) $(DEP_Servo_OBJ) $(DEP_EEPROM_OBJ)
DEPS_CFLAGS := $(DEP_CORE_CFLAGS) $(DEP_SPI_CFLAGS) $(DEP_Servo_CFLAGS) $(DEP_EEPROM_CFLAGS)

all: $(BIN_DIR) $(DEPS) target_ramps_v14 target_farmduino_v10 target_farmduino_k14

clean: target_ramps_v14_clean target_farmduino_v10_clean target_farmduino_k14_clean

strings_test: all
	$(OBJ_COPY) -I ihex $(TARGET_ramps_v14_HEX)     -O binary $(TARGET_ramps_v14_HEX).bin
	$(OBJ_COPY) -I ihex $(TARGET_farmduino_v10_HEX) -O binary $(TARGET_farmduino_v10_HEX).bin
	$(OBJ_COPY) -I ihex $(TARGET_farmduino_k14_HEX) -O binary $(TARGET_farmduino_k14_HEX).bin
	@strings $(TARGET_ramps_v14_HEX).bin | grep -q "6.4.0.R" 
	@strings $(TARGET_farmduino_v10_HEX).bin | grep -q "6.4.0.F" 
	@strings $(TARGET_farmduino_k14_HEX).bin | grep -q "6.4.0.G" 

force_clean:
	$(RM) -r $(BUILD_DIR) $(BIN_DIR)

$(BIN_DIR):
	$(MKDIR_P) $(BIN_DIR)
