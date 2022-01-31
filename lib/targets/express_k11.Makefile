TARGET_express_k11_BUILD_DIR := $(BUILD_DIR)/express_k11
TARGET_express_k11_HEX := $(BIN_DIR)/express_k11.hex

TARGET_express_k11_OBJ := $(patsubst $(FBARDUINO_FIRMWARE_SRC_DIR)/%,$(TARGET_express_k11_BUILD_DIR)/%,$(CXX_OBJ))

$(TARGET_express_k11_HEX): $(TARGET_express_k11_BUILD_DIR) $(TARGET_express_k11_BUILD_DIR)/express_k11.eep $(TARGET_express_k11_BUILD_DIR)/express_k11.elf
	$(OBJ_COPY) -O ihex -R .eeprom  $(TARGET_express_k11_BUILD_DIR)/express_k11.elf $@

$(TARGET_express_k11_BUILD_DIR)/express_k11.eep: $(TARGET_express_k11_BUILD_DIR)/express_k11.elf
	$(OBJ_COPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $< $@

$(TARGET_express_k11_BUILD_DIR)/express_k11.elf: $(TARGET_express_k11_OBJ)
	$(CC) -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections,--relax -mmcu=atmega2560 -o $@ $(TARGET_express_k11_OBJ) $(DEPS_OBJ) $(DEP_CORE_LDFLAGS)

$(TARGET_express_k11_BUILD_DIR)/%.o: $(FBARDUINO_FIRMWARE_SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXX_FLAGS) -DFARMBOT_BOARD_ID=6 $(DEPS_CFLAGS) $< -o $@

$(TARGET_express_k11_BUILD_DIR):
	$(MKDIR_P) $(TARGET_express_k11_BUILD_DIR)

target_express_k11: $(TARGET_express_k11_HEX)

target_express_k11_clean:
	$(RM) -r $(TARGET_express_k11_BUILD_DIR)
	$(RM) $(TARGET_express_k11_HEX)
