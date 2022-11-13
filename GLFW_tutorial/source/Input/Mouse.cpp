#include "Mouse.h"

	 void Mouse::callback::Move(GLFWwindow* window, double xpos, double ypos) {
		position.x = xpos;
		position.y = ypos;

		ContextWindow::events.push(Event::MouseMoveEvent(position));
	}

	 void Mouse::callback::Click(GLFWwindow* window, int button, int action, int mods) {
		button_state[button].action = action;
		static const Event::Type event_type[2] = { Event::MouseButtonReleased, Event::MouseButtonPressed };
		ContextWindow::events.push(Event(event_type[action], Event::MouseButtonEvent(button, position)));
	}

	 void Mouse::callback::Scroll(GLFWwindow* window, double xoffset, double yoffset) {
		 ContextWindow::events.push(Event::MouseScrollEvent((int)yoffset));
	}
	 void Mouse::callback::setup(GLFWwindow* window) {
		 glfwSetCursorPosCallback(window, Move);
		 glfwSetMouseButtonCallback(window, Click);
		 glfwSetScrollCallback(window, Scroll);
	 }
	void Mouse::setRawMotion(bool enable) {
		if (rawMode != enable) {
			rawMode = enable;
			glfwSetInputMode(ContextWindow::context, GLFW_RAW_MOUSE_MOTION, enable);
		}
	}

glm::vec2 Mouse::position;
Key Mouse::button_state[Button_size];
bool Mouse::rawMode = 0;