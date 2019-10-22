DEP_CORE_BUILD_DIR := $(BUILD_DIR)/core
DEP_CORE := $(DEP_CORE_BUILD_DIR)/core.a

DEP_CORE_CFLAGS := \
	-I$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino \
	-I$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/variants/mega \

DEP_CORE_LDFLAGS := \
	 $(DEP_CORE_BUILD_DIR)/core.a -L$(DEP_CORE_BUILD_DIR) -lm

DEP_CORE_ASM_SRC := $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/wiring_pulse.S

DEP_CORE_C_SRC := \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/WInterrupts.c \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/hooks.c \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/wiring.c \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/wiring_analog.c \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/wiring_digital.c \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/wiring_pulse.c \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/wiring_shift.c

DEP_CORE_CXX_SRC := \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/CDC.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/HardwareSerial0.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/HardwareSerial1.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/HardwareSerial2.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/HardwareSerial3.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/IPAddress.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/PluggableUSB.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/Print.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/Stream.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/Tone.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/USBCore.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/WMath.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/WString.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/abi.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/main.cpp \
	$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/new.cpp

DEP_CORE_OBJ := \
	$(DEP_CORE_BUILD_DIR)/wiring_pulse.S.o \
	$(DEP_CORE_BUILD_DIR)/WInterrupts.c.o \
	$(DEP_CORE_BUILD_DIR)/hooks.c.o \
	$(DEP_CORE_BUILD_DIR)/wiring.c.o \
	$(DEP_CORE_BUILD_DIR)/wiring_analog.c.o \
	$(DEP_CORE_BUILD_DIR)/wiring_digital.c.o \
	$(DEP_CORE_BUILD_DIR)/wiring_pulse.c.o \
	$(DEP_CORE_BUILD_DIR)/wiring_shift.c.o \
	$(DEP_CORE_BUILD_DIR)/CDC.cpp.o \
	$(DEP_CORE_BUILD_DIR)/HardwareSerial.cpp.o \
	$(DEP_CORE_BUILD_DIR)/HardwareSerial0.cpp.o \
	$(DEP_CORE_BUILD_DIR)/HardwareSerial1.cpp.o \
	$(DEP_CORE_BUILD_DIR)/HardwareSerial2.cpp.o \
	$(DEP_CORE_BUILD_DIR)/HardwareSerial3.cpp.o \
	$(DEP_CORE_BUILD_DIR)/IPAddress.cpp.o \
	$(DEP_CORE_BUILD_DIR)/PluggableUSB.cpp.o \
	$(DEP_CORE_BUILD_DIR)/Print.cpp.o \
	$(DEP_CORE_BUILD_DIR)/Stream.cpp.o \
	$(DEP_CORE_BUILD_DIR)/Tone.cpp.o \
	$(DEP_CORE_BUILD_DIR)/USBCore.cpp.o \
	$(DEP_CORE_BUILD_DIR)/WMath.cpp.o \
	$(DEP_CORE_BUILD_DIR)/WString.cpp.o \
	$(DEP_CORE_BUILD_DIR)/abi.cpp.o \
	$(DEP_CORE_BUILD_DIR)/main.cpp.o \
	$(DEP_CORE_BUILD_DIR)/new.cpp.o

DEP_CORE_D := $(DEP_CORE_OBJ:.cpp=.d)

DEP_CORE_CFLAGS_P := -c -g -Os -w -std=gnu11 -ffunction-sections \
-fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 \
-DF_CPU=16000000L -DARDUINO=10600 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR \
$(DEP_CORE_CFLAGS)

DEP_CORE_CXX_FLAGS_P := -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections \
-fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 \
-DF_CPU=16000000L -DARDUINO=10600 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR \
$(DEP_CORE_CFLAGS)

$(DEP_CORE): $(DEP_CORE_BUILD_DIR) $(DEP_CORE_OBJ)
	$(AR) rcs $(DEP_CORE) $(DEP_CORE_OBJ)

$(DEP_CORE_BUILD_DIR)/%.S.o: $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/%.S
	$(CC) -c -g -x assembler-with-cpp -flto -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10600 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AV \
	-I$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino -I$(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/variants/mega \
	$< -o $@

$(DEP_CORE_BUILD_DIR)/%.c.o: $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/%.c
	$(CC) $(DEP_CORE_CFLAGS_P) $< -o $@

$(DEP_CORE_BUILD_DIR)/%.cpp.o: $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/cores/arduino/%.cpp
	$(CXX) $(DEP_CORE_CXX_FLAGS_P) $< -o $@

$(DEP_CORE_BUILD_DIR):
	$(MKDIR_P) $(DEP_CORE_BUILD_DIR)

dep_core_clean:
	$(RM) $(DEP_CORE_OBJ)
	$(RM) $(DEP_CORE_D)
	$(RM) $(DEP_CORE)
