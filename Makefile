APP_NAME = pirateGame
BUILD_DIR = ./bin

LIB_NAME:= engine
LIB_DEFINES:= -dynamiclib -fvisibility=hidden
LIB_INCLUDES:= -I./engine/src -I/Users/pprokop/VulkanSDK/1.3.290.0/macOS/include
LIB_LINKERS:= -L/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib -Wl,-rpath,/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib -lvulkan.1
LIB_C_FILES:= ./engine/src/*.c ./engine/src/*.cpp

APP_DEFINES:= 
APP_INCLUDES:= -I./client/src -I./engine/src -I/Users/pprokop/VulkanSDK/1.3.290.0/macOS/include -I/opt/homebrew/Cellar/glfw/3.4/include
APP_LINKERS:= -L/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib -Wl,-rpath,/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib -lvulkan.1 \
	-L/opt/homebrew/Cellar/glfw/3.4/lib -lglfw
APP_C_FILES:= ./client/src/*.cpp

CFLAGS = -std=c++20 -O2
LDFLAGS = -lglfw -lvulkan -lpthread

all: buildLibrary buildClient run

buildLibrary:
	clang++ $(CFLAGS) $(LIB_C_FILES) $(LIB_DEFINES) -o $(BUILD_DIR)/lib$(LIB_NAME).dylib $(LIB_LINKERS) $(LIB_INCLUDES)

buildClient:
	clang++ $(CFLAGS) $(APP_C_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_LINKERS) $(APP_INCLUDES)

run:
	export VK_ICD_FILENAMES=/Users/pprokop/VulkanSDK/1.3.290.0/macOS/share/vulkan/icd.d/MoltenVK_icd.json
	$(BUILD_DIR)/$(APP_NAME)


# export VULKAN_SDK=/Users/pprokop/VulkanSDK/1.3.290.0
# export DYLD_LIBRARY_PATH=.:/Users/pprokop/VulkanSDK/1.3.290.0/macOS/lib