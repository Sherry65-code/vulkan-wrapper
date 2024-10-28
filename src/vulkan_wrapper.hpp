#include "window.hpp"
#include "success.hpp"
#include "volk.h"
#include <vector>

class vkr_c {
public:
	// Data Structures
	typedef struct vulkan_application_info_t {
		std::string engineName;
		uint32_t vulkanVersion;
	} vulkan_application_info_t;

	typedef struct vulkan_init_t {
		window_c::win_info_t win_init_info;
		vkr_c::vulkan_application_info_t vulkan_application_info;
	} vulkan_init_t;

	// Constructor and Destructors
	vkr_c(vulkan_init_t* _vulkan_init_info);
	~vkr_c();

private:
	// Vulkan Init Info Struct Pointer
	vulkan_init_t vulkan_init_info;

	// Vulkan Global Objects
	VkInstance gInstance;

	// Side Functions
		// Finds Required Instance Extensions
		std::vector<const char*> getRequiredInstanceExtensions();
		// Checks for extensions availiability
		bool validateExtensions(const std::vector<const char*>& active_instance_extensions, const std::vector<VkExtensionProperties>& available_instance_extensions);
		// Checks for Layers availiability
		bool validateLayers(const std::vector<const char*>& active_instance_layers, const std::vector<VkLayerProperties>& available_instance_layers);
		// Create GLFW Error Callback
		void createGLFWErrorCallback();

	// Window Init Func
	success_t initWindow();
	// Instance Init - Step 01
	success_t initInstance();
};