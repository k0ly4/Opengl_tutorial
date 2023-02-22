
#ifndef CONTEXT_WINDOW_H
#define CONTEXT_WINDOW_H

#include <list>
#include <queue>
#include "Game/Lib/GLFW.h"
#include "Math/Math.h"
#include "Input/Event.h"
#include "Graphic/RenderEntity.h"

class ContextWindow {
	
public:

	static inline bool isFocus() { return focus;}

private:

	~ContextWindow() {}
	ContextWindow() {}

	struct callback {
		static void framebuffer_size(GLFWwindow* window, int width, int height);
		static void window_position(GLFWwindow* window, int xpos, int ypos);
		static void window_refresh(GLFWwindow* window);
		static void window_focus(GLFWwindow* window, int focused);
		static void setup(GLFWwindow* window);
	};


	static GLFWwindow*	context;
	static glm::uvec2	size;
	static glm::ivec2	pos;
	static bool			focus;

	static std::queue<Event> events;

	friend class RenderWindow;
	friend class Cursor;
	friend class Mouse;
	friend class Keyboard;
	friend class sRender;
};

#endif