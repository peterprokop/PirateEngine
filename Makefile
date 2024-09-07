APP_NAME = pirateGame

BUILD_DIR = ./bin
BUILD_TEXTURES_DIR = ./bin/textures
BUILD_MODELS_DIR = ./bin/models

VK_SDK_BASE:=/Users/pprokop/VulkanSDK/1.3.290.0
GLFW_BASE:=/opt/homebrew/Cellar/glfw/3.4
GLM_BASE:=/opt/homebrew/Cellar/glm/1.0.1

APP_INCLUDES:=\
	-I./client/src\
	-I$(VK_SDK_BASE)/macOS/include\
	-I$(GLFW_BASE)/include\
	-I$(GLM_BASE)/include
APP_LINKERS:=\
	-L$(VK_SDK_BASE)/macOS/lib -Wl,-rpath,$(VK_SDK_BASE)/macOS/lib -lvulkan.1 \
	-L$(GLFW_BASE)/lib -lglfw\
	-L$(GLM_BASE)/lib -lglm
APP_C_FILES:= ./client/src/*.cpp

CFLAGS = -std=c++20 -O2
CMACROS = -D __PE_SHADER_DIR="\"$(abspath $(BUILD_DIR))/shaders\""\
-D __PE_TEXTURES_DIR="\"$(abspath $(BUILD_TEXTURES_DIR))\""\
-D __PE_MODELS_DIR="\"$(abspath $(BUILD_MODELS_DIR))\""

.PHONY: all copyAssets buildShaders buildClient run

all: copyAssets buildShaders buildClient run buildAndRun

copyAssets:
	mkdir -p $(BUILD_TEXTURES_DIR)
	mkdir -p $(BUILD_MODELS_DIR) 
	cp ./client/textures/* $(BUILD_TEXTURES_DIR)
	cp ./client/models/* $(BUILD_MODELS_DIR)

buildShaders:
	mkdir -p $(BUILD_DIR)/shaders
	$(VK_SDK_BASE)/macOS/bin/glslc client/shaders/shader.vert -o $(BUILD_DIR)/shaders/vert.spv
	$(VK_SDK_BASE)/macOS/bin/glslc client/shaders/shader.frag -o $(BUILD_DIR)/shaders/frag.spv

buildClient:
	mkdir -p $(BUILD_DIR)
	clang++ $(CFLAGS) $(CMACROS) $(APP_C_FILES)\
		-o $(BUILD_DIR)/$(APP_NAME) $(APP_LINKERS) $(APP_INCLUDES)

run:
	export VK_ICD_FILENAMES=$(VK_SDK_BASE)/macOS/share/vulkan/icd.d/MoltenVK_icd.json
	echo "### Running pirate engine ###"
	$(BUILD_DIR)/$(APP_NAME)

buildAndRun: buildClient run


# export VULKAN_SDK=$(VK_SDK_BASE)/
# export DYLD_LIBRARY_PATH=.:$(VK_SDK_BASE)/macOS/lib