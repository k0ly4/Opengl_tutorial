#include "Keyboard.h"

	 void Keyboard::callback::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
		
		keys[key].action = action;
		keys[key].mods = mods;
		
		static const Event::Type type_event[3] = {Event::KeyReleased,Event::KeyPressed,Event::KeyRepeated};
		ContextWindow::events.push(Event(type_event[action],Event::KeyEvent(key,mods)));
		ContextWindow::events.last().key = Event::KeyEvent(key, mods);
	}

	void Keyboard::callback::OnChar(GLFWwindow* window, unsigned int codepoint) {
		ContextWindow::events.push(Event::TextEvent(codepoint));
	}
	void Keyboard::callback::setup(GLFWwindow* window) {
		glfwSetKeyCallback(window, OnKey);
		glfwSetCharCallback(window, OnChar);
	}
Key Keyboard::keys[GLFW_KEY_LAST];