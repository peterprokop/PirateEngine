APP_NAME = pirateGame
BUILD_DIR = ./bin

LIB_NAME:= engine
LIB_DEFINES:= -dynamiclib -fvisibility=hidden
LIB_INCLUDES:= -I./engine/src -I/Users/pprokop/VulkanSDK/1.3.290.0/macOS/include
LIB_LINKERS:= -L/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib -Wl,-rpath,/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib -lvulkan.1
LIB_C_FILES:= ./engine/src/*.c

APP_DEFINES:=
APP_INCLUDES:= -I./client/src -I./engine/src
APP_LINKERS:= -L$(BUILD_DIR) -lengine 
APP_C_FILES:= ./client/src/*.c

all: buildLibrary buildClient run

buildLibrary:
	clang $(LIB_C_FILES) $(LIB_DEFINES) -o $(BUILD_DIR)/lib$(LIB_NAME).dylib $(LIB_LINKERS) $(LIB_INCLUDES)

buildClient:
	clang $(APP_C_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_LINKERS) $(APP_INCLUDES)

run:
	$(BUILD_DIR)/$(APP_NAME)


# export VULKAN_SDK=/Users/pprokop/VulkanSDK/1.3.290.0
# export VK_ICD_FILENAMES=/Users/pprokop/VulkanSDK/1.3.290.0/MoltenVK/dylib/macOS/MoltenVK_icd.json
# export DYLD_LIBRARY_PATH=.:/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib