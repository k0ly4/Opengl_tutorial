
#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "Graphic/Framebuffer.h"
#include "Input/Input.h"

class RenderWindow:public RenderTarget {

public:
	//Интервал между обновлениями экранного буфера
	inline void setFrameInterval(int interval) {
		glfwSwapInterval(interval);
	}

	bool create(glm::uvec2 Size, const std::string& title);

	void setMonitor(GLFWmonitor* monitor, glm::vec2 pos = glm::vec2(0.f));

	//Заголовок 
	inline void setTitle(const std::string& title) {
		glfwSetWindowTitle(window, title.c_str());
	}

	//Иконка: По умолчанию: 0, Null
	inline void setIcon(const GLFWimage& icon) {
		glfwSetWindowIcon(window, 1, &icon);
	}

	//Скрыть оконное пространство
	void hide(bool enable);

	//Прозрачность окна
	bool setOpacity(float opacity);

	//Позиция окна
	inline void setPosition(glm::ivec2 position) {
		glfwSetWindowPos(window, position.x, position.y);
	}

	inline const glm::uvec2& getSize() {
		return ContextWindow::size;
	}
	inline const glm::ivec2& getPosition() {
		return ContextWindow::position;
	}
	//Задать размеры окна
	inline void setSize(glm::uvec2 Size) {
		glfwSetWindowSize(window, Size.x, Size.y);
	}

	//Лимит окна при изменении размера
	//fixedRatio - Сохранять соотношения сторон окна при изменении размера
	void setLimitSizeable(bool fixedRatio, glm::ivec2 min, glm::ivec2 max);

	//Получение событий из буфера
	bool pollEvent(Event& event);

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

private:

	GLFWwindow* window = 0;
	void initCallbackFunc();
};

#endif