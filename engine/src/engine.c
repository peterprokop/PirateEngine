#import "engine.h"

#import <stdio.h>
#import <stdlib.h>
#import <vulkan/vulkan.h>

void initEngine() {
	printf("initEngine\n");

	VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello W";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Pirate Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
	
	VkInstance instance;
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	printf("createInfo.enabledExtensionCount %i\n", createInfo.enabledExtensionCount);
	
	createInfo.enabledExtensionCount = 5;

	const char** extensions = malloc(sizeof(char *) * 5);
	extensions[0] = VK_KHR_SURFACE_EXTENSION_NAME;
	extensions[1] = "VK_EXT_metal_surface";	
	extensions[2] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
	extensions[3] = "VK_KHR_portability_subset";
	extensions[4] = "VK_KHR_get_physical_device_properties2";

	createInfo.ppEnabledExtensionNames = extensions;

	printf("createInfo.flags %i\n", createInfo.flags);
	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	printf("createInfo.flags %i\n", createInfo.flags);

	// createInfo.ppEnabledExtensionNames = (char*[]){
	// 	VK_KHR_SURFACE_EXTENSION_NAME,
	// 	"VK_EXT_metal_surface",
	// 	VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
	// };

	printf("createInfo.enabledExtensionCount %i\n", createInfo.enabledExtensionCount);
	

	VkResult result = vkCreateInstance(&createInfo, NULL, &instance);

	if (result == VK_SUCCESS) {
		printf("Init success\n");
	} else {
		printf("Init failure: %i\n", result);
	}
}