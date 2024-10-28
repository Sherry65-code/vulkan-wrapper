#pragma once
#include <GLFW/glfw3.h>
#include "success.hpp"
#include <string>
#include "io.hpp"

class window_c {
public:
	typedef GLFWwindow* win_t;
	typedef union dimensions { uint32_t x, y; uint32_t dimensions[2]; uint32_t width, height; } dimensions;
	typedef struct win_info_t {
		std::string title;
		dimensions dimensions;
		bool isResizable;
		bool isFullscreen;
	} win_info_t;
private:
	win_t window = nullptr;

public:
	success_t createWindow(win_info_t* wininfo);
	bool shouldClose() const;
	void swapBuffers() const;
	void pollEvents() const;
	~window_c();
};