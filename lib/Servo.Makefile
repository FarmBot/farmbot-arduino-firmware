DEP_Servo_BUILD_DIR := $(FBARDUINO_FIRMWARE_LIB_BUILD_DIR)/Servo
DEP_Servo := $(DEP_Servo_BUILD_DIR)/Servo.a
DEP_Servo_SRC_DIR := $(ARDUINO_INSTALL_DIR)/libraries/Servo/src

DEP_Servo_CFLAGS := \
	-I$(DEP_Servo_SRC_DIR)

DEP_Servo_CFLAGS := -I$(DEP_Servo_SRC_DIR)
DEP_Servo_LDFLAGS := $(DEP_Servo_BUILD_DIR)/Servo.a -L$(DEP_Servo_BUILD_DIR) -lm

DEP_Servo_ASM_SRC := $(call rwildcard, $(DEP_Servo_SRC_DIR), *.S)
DEP_Servo_ASM_OBJ := $(DEP_Servo_ASM_SRC:.S=.o)

DEP_Servo_C_SRC   := $(call rwildcard, $(DEP_Servo_SRC_DIR), *.c)
DEP_Servo_C_OBJ   := $(DEP_Servo_C_SRC:.c=.o)

DEP_Servo_CXX_SRC := $(call rwildcard, $(DEP_Servo_SRC_DIR), *.cpp)
DEP_Servo_CXX_OBJ := $(DEP_Servo_CXX_SRC:.cpp=.o)

DEP_Servo_ALL_OBJ := $(DEP_Servo_ASM_OBJ) $(DEP_Servo_C_SRC) $(DEP_Servo_CXX_OBJ)

DEP_Servo_SRC := $(DEP_SERVO_ASM_SRC) $(DEP_SERVO_C_SRC) $(CXX_SRC)
DEP_Servo_OBJ := $(patsubst $(DEP_Servo_SRC_DIR)/%,$(DEP_Servo_BUILD_DIR)/%,$(DEP_Servo_ALL_OBJ))

DEP_Servo_DIRS := $(sort $(dir $(DEP_Servo_OBJ)))

ARDUINO_DEP_Servo_CXX_FLAGS_P := $(DEP_CORE_CXX_FLAGS_P) $(DEP_Servo_CFLAGS)

$(DEP_Servo): $(DEP_CORE) $(DEP_Servo_BUILD_DIR) $(DEP_Servo_OBJ)
	$(AR) rcs $(DEP_Servo) $(DEP_Servo_OBJ)

$(DEP_Servo_BUILD_DIR)/%.o: $(DEP_Servo_SRC_DIR)/%.cpp
	$(CXX) $(ARDUINO_DEP_Servo_CXX_FLAGS_P) $< -o $@

$(DEP_Servo_BUILD_DIR):
	$(MKDIR_P) $(DEP_Servo_DIRS)

dep_Servo: $(DEP_Servo)

dep_Servo_clean:
	$(RM) $(DEP_Servo_OBJ)
	$(RM) $(DEP_Servo)
