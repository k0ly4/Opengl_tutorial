#include "ContextWindow.h"
	
	 void ContextWindow::callback::framebuffer_size(GLFWwindow* window, int width, int height)
	{
		//printf("SizeCallback %d %d\n", width, height);
		size = glm::ivec2(width, height);
		
		ContextWindow::events.push(Event::SizeEvent(size));
	}

	 void  ContextWindow::callback::window_position(GLFWwindow* window, int xpos, int ypos)
	{
		//printf("PosWindowCallback %d %d\n", xpos, ypos);
		position = glm::ivec2(xpos, ypos);
		ContextWindow::events.push(Event::MoveEvent(position));
	}

	 void ContextWindow::callback::window_refresh(GLFWwindow* window)
	{
		printf("Refresh\n");
	}

	 void  ContextWindow::callback::window_focus(GLFWwindow* window, int focus_)
	{
		focus = focus_;
		static const Event::Type event_type[2] = {Event::LostFocus,Event::GainedFocus};
		ContextWindow::events.push(event_type[focus]);
	}
	 void ContextWindow::callback::setup(GLFWwindow* window) {
		 glfwSetFramebufferSizeCallback(window, ContextWindow::callback::framebuffer_size);
		 glfwSetWindowPosCallback(window, ContextWindow::callback::window_position);
		 glfwSetWindowRefreshCallback(window, ContextWindow::callback::window_refresh);
		 glfwSetWindowFocusCallback(window, ContextWindow::callback::window_focus);
	 }

GLFWwindow* ContextWindow::context = 0;
glm::uvec2 ContextWindow::size = glm::uvec2(0);
glm::ivec2 ContextWindow::position = glm::uvec2(0);
EventStack ContextWindow::events;
bool ContextWindow::focus = 1;