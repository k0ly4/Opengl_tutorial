
#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "Graphic/Framebuffer.h"
#include "Input/Input.h"

class RenderWindow:public RenderTarget {

public:
	//�������� ����� ������������ ��������� ������
	inline void setFrameInterval(int interval) {
		glfwSwapInterval(interval);
	}

	bool create(glm::uvec2 Size, const std::string& title);

	void setMonitor(GLFWmonitor* monitor, glm::vec2 pos = glm::vec2(0.f));

	//��������� 
	inline void setTitle(const std::string& title) {
		glfwSetWindowTitle(window, title.c_str());
	}

	//������: �� ���������: 0, Null
	inline void setIcon(const GLFWimage& icon) {
		glfwSetWindowIcon(window, 1, &icon);
	}

	//������ ������� ������������
	void hide(bool enable);

	//������������ ����
	bool setOpacity(float opacity);

	//������� ����
	inline void setPosition(glm::ivec2 position) {
		glfwSetWindowPos(window, position.x, position.y);
	}

	inline const glm::uvec2& getSize() {
		return ContextWindow::size;
	}
	inline const glm::ivec2& getPosition() {
		return ContextWindow::position;
	}
	//������ ������� ����
	inline void setSize(glm::uvec2 Size) {
		glfwSetWindowSize(window, Size.x, Size.y);
	}

	//����� ���� ��� ��������� �������
	//fixedRatio - ��������� ����������� ������ ���� ��� ��������� �������
	void setLimitSizeable(bool fixedRatio, glm::ivec2 min, glm::ivec2 max);

	//��������� ������� �� ������
	bool pollEvent(Event& event);

	//�������� ����� �������� ����
	inline bool isOpen() {
		return !glfwWindowShouldClose(window);
	}

	//������� ����
	inline void close() {
		glfwSetWindowShouldClose(window, true);
	}

	//���������� � �������� ������ �������
	inline void display() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

private:

	GLFWwindow* window = 0;
	void initCallbackFunc();
};

#endif