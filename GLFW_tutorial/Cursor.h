#ifndef CURSOR_H
#define CURSOR_H

#include "ContextWindow.h"

class Cursor {


public:

	enum StandartImage:int
	{
		Arrow = GLFW_ARROW_CURSOR,
		Ibeam = GLFW_IBEAM_CURSOR,
		Crosshair = GLFW_CROSSHAIR_CURSOR,
		Hand = GLFW_HAND_CURSOR,
		HResize = GLFW_HRESIZE_CURSOR,
		VResize = GLFW_VRESIZE_CURSOR,
	};

	enum ModeDisplay :int 
	{
		Normal = GLFW_CURSOR_NORMAL,
		Disabled = GLFW_CURSOR_DISABLED,
		Hidden = GLFW_CURSOR_HIDDEN,
	};

	//Показ, скрытие или отключение курсора
	static void setMode(ModeDisplay mode_);
	static ModeDisplay getMode() {
		return mode;
	}
	//Установить иконку
	static void setCursorImage(const GLFWimage* icon, glm::ivec2 hotSpot = glm::ivec2(0));

	//Задать стандартную иконку:
	static void setCursorImage(StandartImage standart_theme_shape);

	//Находится ли внутри окна
	static const bool isInsideWindow() {
		return is_inside;
	}
	
private:	
	friend RenderWindow;

		Cursor() {}
	   ~Cursor() {}

	   class callback {

	   public:
		   static void setup(GLFWwindow* window);

	   private:
		   static void Entry(GLFWwindow* window, int entered);
	   }; 

	   static GLFWcursor* cursor;
	   static bool is_inside;
	   static ModeDisplay mode;
};

#endif