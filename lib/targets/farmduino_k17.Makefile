TARGET_farmduino_k17_BUILD_DIR := $(BUILD_DIR)/farmduino_k17
TARGET_farmduino_k17_HEX := $(BIN_DIR)/farmduino_k17.hex

TARGET_farmduino_k17_OBJ := $(patsubst $(FBARDUINO_FIRMWARE_SRC_DIR)/%,$(TARGET_farmduino_k17_BUILD_DIR)/%,$(CXX_OBJ))

$(TARGET_farmduino_k17_HEX): $(TARGET_farmduino_k17_BUILD_DIR) $(TARGET_farmduino_k17_BUILD_DIR)/farmduino_k17.eep $(TARGET_farmduino_k17_BUILD_DIR)/farmduino_k17.elf
	$(OBJ_COPY) -O ihex -R .eeprom  $(TARGET_farmduino_k17_BUILD_DIR)/farmduino_k17.elf $@

$(TARGET_farmduino_k17_BUILD_DIR)/farmduino_k17.eep: $(TARGET_farmduino_k17_BUILD_DIR)/farmduino_k17.elf
	$(OBJ_COPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $< $@

$(TARGET_farmduino_k17_BUILD_DIR)/farmduino_k17.elf: $(TARGET_farmduino_k17_OBJ)
	$(CC) -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections,--relax -mmcu=atmega2560 -o $@ $(TARGET_farmduino_k17_OBJ) $(DEPS_OBJ) $(DEP_CORE_LDFLAGS)

$(TARGET_farmduino_k17_BUILD_DIR)/%.o: $(FBARDUINO_FIRMWARE_SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXX_FLAGS) -DFARMBOT_BOARD_ID=7 $(DEPS_CFLAGS) $< -o $@

$(TARGET_farmduino_k17_BUILD_DIR):
	$(MKDIR_P) $(TARGET_farmduino_k17_BUILD_DIR)

target_farmduino_k17: $(TARGET_farmduino_k17_HEX)

target_farmduino_k17_clean:
	$(RM) -r $(TARGET_farmduino_k17_BUILD_DIR)
	$(RM) $(TARGET_farmduino_k17_HEX)
