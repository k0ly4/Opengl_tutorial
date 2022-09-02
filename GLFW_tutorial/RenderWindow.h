
#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "FrameBuffer.h"

struct Key
{
	enum Type :int
	{
		release, press, repeat
	};
	int action;
	int mods;
};

class Keyboard {
	static Key keys[GLFW_KEY_LAST];
	friend class RenderWindow;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (keys[key].action != action || action == GLFW_REPEAT) {

			Window::events.push_back(Event(key, Event::KEY_RELEASED + action, mods));

			keys[key].action = action;
			keys[key].mods = mods;
		}
	}
	static void char_callback(GLFWwindow* window, unsigned int codepoint) {
		Window::events.push_back(Event(codepoint, Event::TEXT_ENTERED));
	}
public:
	static bool isAction(int button, int action) {
		return  keys[button].action == action;
	}
	static const Key& getKey(int button) {
		return keys[button];
	}
};
Key Keyboard::keys[GLFW_KEY_LAST];

class Cursor {
	Cursor() {}
	~Cursor() {}
	friend class RenderWindow;
	static GLFWcursor* cursor;
	static bool Inside_Window;
	static int mode;

	static void enter_callback(GLFWwindow* window, int entered) {
		Inside_Window = entered;
		Window::events.push_back(Event(Event::CURSOR_LEFT + entered));
	}

public:
	//GLFW_CURSOR_NORMAL
	//GLFW_CURSOR_DISABLED
	//GLFW_CURSOR_HIDDEN
	static void setMode(int value) {
		if (mode != value) {
			mode = value;
			glfwSetInputMode(Window::context, GLFW_CURSOR, value);
		}
	}
	//Задать иконку для курсора
	static void setCursorImage(const GLFWimage* icon, glm::ivec2 hotSpot = glm::ivec2(0)) {
		if (cursor)glfwDestroyCursor(cursor);
		cursor = glfwCreateCursor(icon, hotSpot.x, hotSpot.y);
		glfwSetCursor(Window::context, cursor);
	}
	//Задать стандартную иконку для курсора:
	//GLFW_ARROW_CURSOR 
	//GLFW_IBEAM_CURSOR 
	//GLFW_CROSSHAIR_CURSOR 
	//GLFW_HAND_CURSOR 
	//GLFW_HRESIZE_CURSOR 
	//GLFW_VRESIZE_CURSOR
	static void setCursorImage(int standart_theme_shape) {
		if (cursor) glfwDestroyCursor(cursor);
		cursor = glfwCreateStandardCursor(standart_theme_shape);
		glfwSetCursor(Window::context, cursor);
	}

	static const bool isInsideWindow() {
		return Inside_Window;
	}
};
bool Cursor::Inside_Window = 0;
GLFWcursor* Cursor::cursor = 0;
int Cursor::mode = GLFW_CURSOR_NORMAL;

class Mouse {
	Mouse() {}
	~Mouse() {}
	friend class RenderWindow;
	static glm::vec2 position;
	static Key button_state[GLFW_MOUSE_BUTTON_LAST];
	static bool rawMode;

	static void pos_callback(GLFWwindow* window, double xpos, double ypos) {
		Window::events.push_back(Event::MOUSE_MOVE);
		position.x = xpos;
		position.y = ypos;
	}
	static void button_callback(GLFWwindow* window, int button, int action, int mods) {
		Window::events.push_back(Event(button, Event::MOUSE_BUTTON_RELEASED + action));
		button_state[button].action = action;
	}
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Window::events.push_back(Event(yoffset, Event::MOUSE_SCROLL));
	}
public:
	static const Key& getButton(int button) {
		return button_state[button];
	}
	static const glm::vec2& getPosition() {
		return position;
	}
	static bool isAction(int button, int action) {
		return  button_state[button].action == action;
	}
	//Задать необработанное движение мыши: при отключенном курсоре
	static void setRawMotion(bool enable) {
		if (rawMode != enable) {
			rawMode = enable;
			glfwSetInputMode(Window::context, GLFW_RAW_MOUSE_MOTION, enable);
		}
	}
};
glm::vec2 Mouse::position;
Key Mouse::button_state[GLFW_MOUSE_BUTTON_LAST];
bool Mouse::rawMode = 0;

#include <codecvt>
class Clipboard {
	Clipboard() {}
	~Clipboard() {}
public:
	static std::wstring to_wstring(std::string& narrow) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(narrow);
	}
	static bool get(std::string& str) {
		const char* buffer = glfwGetClipboardString(NULL);
		if (buffer)
		{
			str = buffer;
			return 1;
		}
		return 0;
	}
	static void push(const std::string& str) {
		glfwSetClipboardString(NULL, str.c_str());
	}
};

class RenderWindow :public RenderTarget {
	GLFWwindow* window = 0;
	void initCallbackFunc() {
		//window_event
		Window::context = window;
		glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);
		glfwSetWindowPosCallback(window, Window::window_pos_callback);
		glfwSetWindowRefreshCallback(window, Window::window_refresh_callback);
		glfwSetWindowFocusCallback(window, Window::window_focus_callback);
		//Keyboard
		glfwSetKeyCallback(window, Keyboard::key_callback);
		glfwSetCharCallback(window, Keyboard::char_callback);
		//Mouse
		glfwSetCursorPosCallback(window, Mouse::pos_callback);
		glfwSetMouseButtonCallback(window, Mouse::button_callback);
		glfwSetScrollCallback(window, Mouse::scroll_callback);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Mouse::position = glm::vec2(xpos, ypos);

		glfwSetCursorEnterCallback(window, Cursor::enter_callback);
	}
public:
	//Интервал между обновлением экранного буфера
	inline void setFrameInterval(int interval) {
		glfwSwapInterval(interval);
	}

	bool create(glm::uvec2 Size, const std::string& title) {
		if (window) glfwDestroyWindow(window);
		window = GL_LIB.getContext();
		if (window == NULL)
		{
			std::cout << "(!)Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return 0;
		}
		setSize(Size);
		Window::size = Size;
		setTitle(title);
		initCallbackFunc();
		return 1;
	}
	inline void setMonitor(GLFWmonitor* monitor, glm::vec2 pos = glm::vec2(0.f)) {
		const  GLFWvidmode* mode = glfwGetVideoMode(monitor);

		/*glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);*/

		glfwSetWindowMonitor(window, monitor, pos.x, pos.y, mode->width, mode->height, mode->refreshRate);
	}

	//Заголовок 
	inline void setTitle(const std::string& title) {
		glfwSetWindowTitle(window, title.c_str());
	}
	//Иконка: По умолчанию: 0, Null
	inline void setIcon(size_t size, const GLFWimage* icon) {
		glfwSetWindowIcon(window, size, icon);
	}
	//Скрыть оконное пространство
	inline void hide(bool enable) {
		if (enable)glfwHideWindow(window);
		else glfwShowWindow(window);
	}
	//Прозрачность окна
	bool setOpacity(float opacity) {
		glfwSetWindowOpacity(window, opacity);
		return opacity == glfwGetWindowOpacity(window);
	}
	//Позиция окна
	inline void setPosition(glm::ivec2 position) {
		glfwSetWindowPos(window, position.x, position.y);
	}

	inline const glm::uvec2& getSize() {
		return Window::size;
	}
	inline const glm::ivec2& getPosition() {
		return Window::position;
	}
	//Задать размеры окна
	inline void setSize(glm::uvec2 Size) {
		glfwSetWindowSize(window, Size.x, Size.y);
	}
	//Лимит окна при изменении размера
	//fixedRatio - Сохранять соотношения сторон окна при изменении размера
	inline void setLimitSizeable(bool fixedRatio, glm::ivec2 min, glm::ivec2 max) {
		if (fixedRatio) glfwSetWindowAspectRatio(window, Window::size.x, Window::size.y);
		else glfwSetWindowAspectRatio(window, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSetWindowSizeLimits(window, min.x, min.y, max.x, max.y);
	}
	//Получение событий из буфера
	bool pollEvent(Event& event) {
		if (Window::events.size()) {
			event = Window::events.front();
			Window::events.pop_front();
			return 1;
		}
		return 0;
	}
	//проверка флага закрытия окна
	inline bool isOpen() {
		return !glfwWindowShouldClose(window);
	}
	//закрыть окно
	inline void close() {
		glfwSetWindowShouldClose(window, true);
	}
	//отобразить и обновить список событий
	inline void display() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}window;

#endif