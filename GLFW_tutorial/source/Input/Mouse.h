#ifndef MOUSE_H
#define MOUSE_H

#include"Graphic/ContextWindow.h"

class Mouse {
	
public:
	enum Button:int
	{
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Button_1 = GLFW_MOUSE_BUTTON_1,
		Button_2 = GLFW_MOUSE_BUTTON_2,
		Button_3 = GLFW_MOUSE_BUTTON_3,
		Button_4 = GLFW_MOUSE_BUTTON_4,
		Button_5 = GLFW_MOUSE_BUTTON_5,
		Button_6 = GLFW_MOUSE_BUTTON_6,
		Button_7 = GLFW_MOUSE_BUTTON_7,
		Button_8 = GLFW_MOUSE_BUTTON_8,
		Button_size,
	};

	static const Key& getButton(Button button) {
		return button_state[button];
	}

	static const glm::vec2& getPosition() {
		return position;
	}

	static bool isAction(Button button, int action) {
		return  button_state[button].action == action;
	}

	//Задать необработанное движение мыши: при отключенном курсоре
	static void setRawMotion(bool enable);

private:
	Mouse() {}
	~Mouse() {}

	friend class RenderWindow;

	static glm::vec2 position;
	static Key button_state[Button_size];
	static bool rawMode;

	class callback {

		public:
			static void setup(GLFWwindow* window);
		
		private:

			static void Move(GLFWwindow* window, double xpos, double ypos);
			static void Click(GLFWwindow* window, int button, int action, int mods);
			static void Scroll(GLFWwindow* window, double xoffset, double yoffset);

	};
	

};

#endif
