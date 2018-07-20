TARGET_ramps_v14_BUILD_DIR := $(BUILD_DIR)/ramps_v14
TARGET_ramps_v14_HEX := $(BIN_DIR)/arduino_firmware.hex

TARGET_ramps_v14_OBJ := $(patsubst $(FBARDUINO_FIRMWARE_SRC_DIR)/%,$(TARGET_ramps_v14_BUILD_DIR)/%,$(CXX_OBJ))

$(TARGET_ramps_v14_HEX): $(TARGET_ramps_v14_BUILD_DIR) $(TARGET_ramps_v14_BUILD_DIR)/arduino_firmware.eep $(TARGET_ramps_v14_BUILD_DIR)/arduino_firmware.elf
	$(OBJ_COPY) -O ihex -R .eeprom  $(TARGET_ramps_v14_BUILD_DIR)/arduino_firmware.elf $@

$(TARGET_ramps_v14_BUILD_DIR)/arduino_firmware.eep: $(TARGET_ramps_v14_BUILD_DIR)/arduino_firmware.elf
	$(OBJ_COPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $< $@

$(TARGET_ramps_v14_BUILD_DIR)/arduino_firmware.elf: $(TARGET_ramps_v14_OBJ)
	$(CC) -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections,--relax -mmcu=atmega2560 -o $@ $(TARGET_ramps_v14_OBJ) $(DEPS_OBJ) $(DEP_CORE_LDFLAGS)

$(TARGET_ramps_v14_BUILD_DIR)/%.o: $(FBARDUINO_FIRMWARE_SRC_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -DFARMBOT_BOARD_ID=0 $(DEPS_CFLAGS) $< -o $@

$(TARGET_ramps_v14_BUILD_DIR):
	$(MKDIR_P) $(TARGET_ramps_v14_BUILD_DIR)

target_ramps_v14: $(TARGET_ramps_v14_HEX)

target_ramps_v14_clean:
	$(RM) -r $(TARGET_ramps_v14_OBJ)
	$(RM) $(TARGET_ramps_v14_HEX)
