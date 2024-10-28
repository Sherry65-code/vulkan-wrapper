#include "vulkan_wrapper.hpp"

// Initialise window class
window_c window;

bool vkr_c::validateExtensions(const std::vector<const char*>& active_instance_extensions, const std::vector<VkExtensionProperties>& available_instance_extensions) {
	for (size_t i = 0; i < active_instance_extensions.size(); i++) {
		bool found = false;
		for (size_t j = 0; j < available_instance_extensions.size(); j++) {
			if (strcmp(active_instance_extensions[i], available_instance_extensions[j].extensionName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) return false;
	}
	return true;
}

bool vkr_c::validateLayers(const std::vector<const char*>& active_instance_layers, const std::vector<VkLayerProperties>& available_instance_layers) {
	for (size_t i = 0; i < active_instance_layers.size(); i++) {
		bool found = false;
		for (size_t j = 0; j < available_instance_layers.size(); j++) {
			if (strcmp(active_instance_layers[i], available_instance_layers[j].layerName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) return false;
	}
	return true;
}

void vkr_c::createGLFWErrorCallback() {
	glfwSetErrorCallback([](int error, const char* description) {
		io::printError("GLFW Error {} : {}", error, description);
		});
}

std::vector<const char*> vkr_c::getRequiredInstanceExtensions() {
	uint32_t required_extensions_count;
	
	// Get Required Instance Extensions with their count
	const char** required_extensions = glfwGetRequiredInstanceExtensions(&required_extensions_count);
	
	// Error check
	if (!required_extensions) io::printErrorAndExit("Failed to Get Required Extensions!");

	// Copy pointer to vector
	std::vector<const char*> vec(required_extensions, required_extensions + required_extensions_count);
	return vec;
}

vkr_c::vkr_c(vkr_c::vulkan_init_t* _vulkan_init_info) {
	// Dereference to vulkan init struct
	vulkan_init_info = *_vulkan_init_info;

	// Vulkan basic init
	success_t init_window_s = vkr_c::initWindow();
	
		// GLFW Error Callback Setup
		vkr_c::createGLFWErrorCallback();

	success_t init_instance_s = vkr_c::initInstance();
}

success_t vkr_c::initWindow() {
	return window.createWindow(&vulkan_init_info.win_init_info);
}

success_t vkr_c::initInstance() {
	// Global success checker for vulkan functions
	VkResult success;

	io::printDebug("Initializing Vulkan Instance...");

	// Intializing Volk
	if (volkInitialize() != VK_SUCCESS) {
		io::printErrorAndExit("Failed to initialize Volk!");
	}

	// Get Vulkan Extensions Available
	uint32_t instance_extension_count;
	do {
		success = vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr);
	} while (success == VK_INCOMPLETE);

	if (success != VK_SUCCESS) io::printError("Failed to Get Extensions!");
	std::vector<VkExtensionProperties> available_instance_extensions(instance_extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, available_instance_extensions.data());

	// Get Required Instance Extensions
	std::vector<const char*> active_instance_extensions = getRequiredInstanceExtensions();
	
	// Enable Debug Layers if needed
#ifndef NDEBUG
	// Check for debug report extension support
	bool has_debug_report = false;
	for (const auto& ext : available_instance_extensions) {
		if (strcmp(ext.extensionName, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0) {
			has_debug_report = true;
			break;
		}
	}
	if (has_debug_report)
		active_instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	else
		io::printWarning("{} is not available! Disabling Debug Reporting!", VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	// Enable Validation Layers
	// --TO-BE-IMPLEMENTED--
#endif 
	
	// Extensions avaliability Check
	if (!validateExtensions(active_instance_extensions, available_instance_extensions))
		io::printErrorAndExit("Required Instance Extensions are missing!");

	// Get Layers
	uint32_t instance_layer_count;
	vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);

	std::vector<VkLayerProperties> supported_validation_layers(instance_layer_count);
	vkEnumerateInstanceLayerProperties(&instance_layer_count, supported_validation_layers.data());

	std::vector<const char*> requested_validation_layers;

#ifndef NDEBUG
	// If debug mode is on then turn validation layers on
	requested_validation_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

	// Check if required layers are available
	if (!validateLayers(requested_validation_layers, supported_validation_layers))
		io::printWarning("Failed to Enable Validation Layers!");
	
	// Vk Application Info Pass
	VkApplicationInfo applicationInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
	applicationInfo.pApplicationName = vulkan_init_info.win_init_info.title.c_str();
	applicationInfo.pEngineName = vulkan_init_info.vulkan_application_info.engineName.c_str();
	applicationInfo.apiVersion = vulkan_init_info.vulkan_application_info.vulkanVersion;

	// Setting up struct for Instance Creation
	VkInstanceCreateInfo instanceInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	instanceInfo.pApplicationInfo = &applicationInfo;
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(active_instance_extensions.size());
	instanceInfo.ppEnabledExtensionNames = active_instance_extensions.data();
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(requested_validation_layers.size());
	instanceInfo.ppEnabledLayerNames = requested_validation_layers.data();

	// Create VkInstance and check if it succeeded
	success = vkCreateInstance(&instanceInfo, nullptr, &gInstance);

	if (success != VK_SUCCESS) {
		io::printError("Failed to create Vulkan Instance!");
		return FAILURE;
	}

	// Load Volk sepecific instance functions
	volkLoadInstance(gInstance);

	io::printDebug("Vulkan Instance Created Successfully!");

	return SUCCESS;
}

vkr_c::~vkr_c() {
	// Cleanup Functions for vulkan

	io::printDebug("Vulkan Object Destructor Initiated!");

	// Destroy Vulkan Instance
	vkDestroyInstance(gInstance, nullptr);

	io::printDebug("Vulkan Object Destructor Exiting!");
}