#include "window.hpp"

success_t window_c::createWindow(window_c::win_info_t* wininfo) {
	// Check for window initialisation already done to prevent multiple initialisations
	if (window_c::window) {
		io::printDebug("Window already initialised!");
		return FAILURE;
	}
	
	io::printDebug("Window Initializing...");

	// Initialise GLFW and report if failed
	if (!glfwInit()) {
		io::printError("GLFW Initialisation Failed!");
		return FAILURE;
	}

	// Set window resizability
	glfwWindowHint(GLFW_RESIZABLE, wininfo->isResizable);

	io::printDebug("Resizable = {}", wininfo->isResizable);

	// Create Window from the given struct
	if (wininfo->isFullscreen) {
		
		// Get Primary monitor
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		// Get main video mode for monitor
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		window_c::window = glfwCreateWindow(wininfo->dimensions.x, wininfo->dimensions.y, wininfo->title.c_str(), monitor, nullptr);
	}
	else {
		window_c::window = glfwCreateWindow(wininfo->dimensions.x, wininfo->dimensions.y, wininfo->title.c_str(), nullptr, nullptr);
	}
	
	io::printDebug("Fullscreen = {}", wininfo->isFullscreen);

	// Check for window intialisation failure
	if (!window_c::window) {
		io::printError("Window could not be created!");
		glfwDestroyWindow(window_c::window);
		return FAILURE;
	}

	io::printDebug("Window Initialized");

	return SUCCESS;
}

bool window_c::shouldClose() const {
	return glfwWindowShouldClose(window_c::window);
}

void window_c::swapBuffers() const {
	glfwSwapBuffers(window_c::window);
}

void window_c::pollEvents() const {
	glfwPollEvents();
}

window_c::~window_c() {
	if (!window_c::window) return;
	glfwDestroyWindow(window_c::window);
	glfwTerminate();
	io::printDebug("Window Destroyed!");
}