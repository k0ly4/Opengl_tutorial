
#ifndef CONTEXT_WINDOW_H
#define CONTEXT_WINDOW_H

#include "GLFW.h"
#include "Math.h"
#include "Event.h"
#include "RenderEntity.h"
#include <list>

class ContextWindow {
	
public:

	static inline bool isFocus() {
		return focus;
	}
private:

	struct callback {
		static void framebuffer_size(GLFWwindow* window, int width, int height);
		static void window_position(GLFWwindow* window, int xpos, int ypos);
		static void window_refresh(GLFWwindow* window);
		static void window_focus(GLFWwindow* window, int focused);
		static void setup(GLFWwindow* window);
	};


	static GLFWwindow* context;
	static glm::uvec2 size;
	static glm::ivec2 position;
	static bool focus;
	
	~ContextWindow() {}
	ContextWindow() {}

	friend class RenderWindow;
	friend class Cursor;
	friend class Mouse;
	friend class Keyboard;
	friend class GlRender;
	
	static EventStack events;
};

#endif