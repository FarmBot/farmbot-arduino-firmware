TARGET_farmduino_k14_BUILD_DIR := $(BUILD_DIR)/farmduino_k14
TARGET_farmduino_k14_HEX := $(BIN_DIR)/farmduino_k14.hex

TARGET_farmduino_k14_OBJ := $(patsubst $(FBARDUINO_FIRMWARE_SRC_DIR)/%,$(TARGET_farmduino_k14_BUILD_DIR)/%,$(CXX_OBJ))

$(TARGET_farmduino_k14_HEX): $(TARGET_farmduino_k14_BUILD_DIR) $(TARGET_farmduino_k14_BUILD_DIR)/farmduino_k14.eep $(TARGET_farmduino_k14_BUILD_DIR)/farmduino_k14.elf
	$(OBJ_COPY) -O ihex -R .eeprom  $(TARGET_farmduino_k14_BUILD_DIR)/farmduino_k14.elf $@

$(TARGET_farmduino_k14_BUILD_DIR)/farmduino_k14.eep: $(TARGET_farmduino_k14_BUILD_DIR)/farmduino_k14.elf
	$(OBJ_COPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $< $@

$(TARGET_farmduino_k14_BUILD_DIR)/farmduino_k14.elf: $(TARGET_farmduino_k14_OBJ)
	$(CC) -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections,--relax -mmcu=atmega2560 -o $@ $(TARGET_farmduino_k14_OBJ) $(DEPS_OBJ) $(DEP_CORE_LDFLAGS)

$(TARGET_farmduino_k14_BUILD_DIR)/%.o: $(FBARDUINO_FIRMWARE_SRC_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -DFARMBOT_BOARD_ID=2 $(DEPS_CFLAGS) $< -o $@

$(TARGET_farmduino_k14_BUILD_DIR):
	$(MKDIR_P) $(TARGET_farmduino_k14_BUILD_DIR)

target_farmduino_k14: $(TARGET_farmduino_k14_HEX)

target_farmduino_k14_clean:
	$(RM) -r $(TARGET_farmduino_k14_BUILD_DIR)
	$(RM) $(TARGET_farmduino_k14_HEX)
