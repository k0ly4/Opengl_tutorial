#include "RenderWindow.h"

	void RenderWindow::initCallbackFunc() {
		//window_event
		ContextWindow::context = window;
		ContextWindow::callback::setup(window);
		//Keyboard
		Keyboard::callback::setup(window);
		//Mouse
		Mouse::callback::setup(window);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Mouse::position = glm::vec2(xpos, ypos);

		Cursor::callback::setup(window);
	}

	bool RenderWindow::create(glm::uvec2 Size, const std::string& title) {
		if (window) glfwDestroyWindow(window);
		window = GLFW::getContext();
		if (window == NULL)
		{
			std::cout << "(!)Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return 0;
		}
		setSize(Size);
		ContextWindow::size = Size;
		setTitle(title);
		initCallbackFunc();
		return 1;
	}

	void RenderWindow::setMonitor(GLFWmonitor* monitor, glm::ivec2 pos) {
		const  GLFWvidmode* mode = glfwGetVideoMode(monitor);

		/*glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);*/

		glfwSetWindowMonitor(window, monitor, pos.x, pos.y, mode->width, mode->height, mode->refreshRate);
	}

	inline void RenderWindow::hide(bool enable) {
		if (enable)glfwHideWindow(window);
		else glfwShowWindow(window);
	}
	
	bool RenderWindow::setOpacity(float opacity) {
		glfwSetWindowOpacity(window, opacity);
		return opacity == glfwGetWindowOpacity(window);
	}

	void RenderWindow::setLimitSizeable(bool fixedRatio, glm::ivec2 min, glm::ivec2 max) {
		if (fixedRatio) glfwSetWindowAspectRatio(window, ContextWindow::size.x, ContextWindow::size.y);
		else glfwSetWindowAspectRatio(window, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSetWindowSizeLimits(window, min.x, min.y, max.x, max.y);
	}
	
	bool RenderWindow::pollEvent(Event& event) {
		glfwPollEvents();
		if (ContextWindow::events.empty()) return 0;
		event = ContextWindow::events.front();
		ContextWindow::events.pop();
		return 1;
	}