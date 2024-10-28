#include "vulkan_wrapper.hpp"

int main() {
	vkr_c::vulkan_init_t vulkan_init_info;
	vulkan_init_info.win_init_info.title = "Vulkan Demo 1";
	vulkan_init_info.win_init_info.dimensions.x = 500;
	vulkan_init_info.win_init_info.dimensions.y = 500;
	vulkan_init_info.win_init_info.isFullscreen = false;
	vulkan_init_info.win_init_info.isResizable = false;
	vulkan_init_info.vulkan_application_info.engineName = "Vulkan Engine";
	vulkan_init_info.vulkan_application_info.vulkanVersion = VK_VERSION_1_3;

	// Initialise vulkan wrapper
	vkr_c vkr(&vulkan_init_info);
}