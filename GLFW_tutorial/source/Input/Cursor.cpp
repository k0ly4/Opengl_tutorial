#include "Cursor.h"

	void Cursor::callback::Entry(GLFWwindow* window, int entered) {
		is_inside = entered;
		static const Event::Type event_type[2] = { Event::CursorLeft,Event::CursorEntered };
		ContextWindow::events.push(event_type[entered]);
	}

	void Cursor::callback::setup(GLFWwindow* window){
		glfwSetCursorEnterCallback(window, Entry);
	}

	void Cursor::setMode(ModeDisplay value) {
		if (mode != value) {
			mode = value;
			glfwSetInputMode(ContextWindow::context, GLFW_CURSOR, value);
		}
	}

	void Cursor::setCursorImage(const GLFWimage* icon, glm::ivec2 hotSpot) {
		if (cursor)glfwDestroyCursor(cursor);
		cursor = glfwCreateCursor(icon, hotSpot.x, hotSpot.y);
		glfwSetCursor(ContextWindow::context, cursor);
	}

	void Cursor::setCursorImage(StandartImage standart_theme_shape) {
		if (cursor) glfwDestroyCursor(cursor);
		cursor = glfwCreateStandardCursor(standart_theme_shape);
		glfwSetCursor(ContextWindow::context, cursor);
	}

bool Cursor::is_inside= 0;
GLFWcursor* Cursor::cursor = 0;
Cursor::ModeDisplay Cursor::mode = Cursor::Normal;