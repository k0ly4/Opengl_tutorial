
#ifndef RENDER_H
#define RENDER_H

#include "Type_Object.h"

class GeneralRender {
protected:
	glm::ivec2 size;
	friend class GlRender;
	unsigned int id;
public:
	const glm::ivec2& getSize() {
		return size;
	}
};

class RenderTarget {
protected:
	View* view = 0;
public:
	inline void setView(View& view) {
		this->view = &view;
	}
	inline void drawDepth(Drawable& object) {
		object.draw(view, glShader::get(glShader::shadow_depth));
	}
	inline void draw(Drawable& object) {
		object.draw(view, glShader::get(object.id_obj));
	}
};

struct Event {
	enum Action : int {

		KEY_RELEASED, KEY_PRESSED, KEY_REPEATED, TEXT_ENTERED,
		MOUSE_BUTTON_RELEASED, MOUSE_BUTTON_PRESSED, CURSOR_LEFT, CURSOR_ENTERED, MOUSE_SCROLL, MOUSE_MOVE,
		WINDOW_RESIZE, WINDOW_REPOS, WINDOW_LOST_FOCUS, WINDOW_GET_FOCUS
	};
	int action;
	int key;
	int mods;
	Event() {}
	Event(int k_key, int k_action) :key(k_key), action(k_action) {}
	Event(int k_key, int k_action, int k_mods) :key(k_key), action(k_action), mods(k_mods) {}
	Event(int k_key, Action k_action) :key(k_key), action(k_action) {}
	Event(int k_action) : action(k_action) {}
	//int mods;
};

class Window {
	static GLFWwindow* context;
	static glm::uvec2 size;
	static glm::ivec2 position;
	static bool focus;
	~Window() {}
	Window() {}

	friend class RenderWindow;
	friend class Cursor;
	friend class Mouse;
	friend class Keyboard;
	friend class GlRender;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		//printf("SizeCallback %d %d\n", width, height);
		size = glm::ivec2(width, height);
		Window::events.push_back(Event::WINDOW_RESIZE);
	}
	static void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
	{
		//printf("PosWindowCallback %d %d\n", xpos, ypos);
		position = glm::ivec2(xpos, ypos);
		Window::events.push_back(Event::WINDOW_REPOS);
	}
	static void window_refresh_callback(GLFWwindow* window)
	{
		printf("Refresh\n");
	}
	static void window_focus_callback(GLFWwindow* window, int focused)
	{
		focus = focused;
		Window::events.push_back(Event::WINDOW_LOST_FOCUS + focused);
	}
	static std::list<Event> events;
public:
	static bool getFocus() {
		return focus;
	}
};
GLFWwindow* Window::context =0;
glm::uvec2 Window::size = glm::uvec2(0.f);
glm::ivec2 Window::position = glm::uvec2(0.f);
std::list<Event> Window::events;
bool Window::focus = 1;

class GlRender {
	GlRender() {}
	~GlRender() {}
	static Color cur_color_clear;
	static GLenum polygonMode;
	static bool isDepthTest;
	static bool isBlend;
	static unsigned int last_fbo;
public:
	enum Primitive:GLenum
	{
		POINTS =0, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLES_STRIP, TRIANGLES_FAN, QUADS, SIZE
	};
		static inline void setClearColor(const Color& color) {
		if (cur_color_clear != color) {
			cur_color_clear = color;
			glClearColor(color.r, color.g, color.b, color.a);
		}
	}
	static inline void setClearColor(float color) {
		setClearColor(Color(color));
	}
	static inline void setClearColor(float r, float g, float b, float a) {
		setClearColor(Color(r, g, b, a));
	}

	static inline void DepthTest(bool enable) {
		if (isDepthTest != enable) {
			isDepthTest = enable;
			if (enable)glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
		}
	}
	static inline void Blend(bool enable) {
		if (isBlend != enable) {
			isBlend = enable;
			if (enable)glEnable(GL_BLEND);
			else glDisable(GL_BLEND);
		}
	}

	class  Viewport {
		static IntRect cur;
	public:
		static inline const IntRect& get() {
			return cur;
		}
		static inline void set(const IntRect& viewport) {
			if (cur != viewport) {
				cur = viewport;
				glViewport(cur.x, cur.y, cur.w, cur.h);
			}
		}
		static inline void set(glm::ivec2 pos, glm::ivec2 size) {
			set(IntRect(pos, size));
		}
		static inline void set(glm::ivec2 size) {
			set(IntRect(glm::ivec2(0), size));
		}
		static inline void set(GLint x, GLint y, GLint w, GLint h) {
			set(IntRect(x, y, w, h));
		}
	};
	class CullFace {
		static bool enable;
		static GLenum mode;
		static GLenum frontFace;
	public:
		static void Mode(GLenum new_state) {
			if (mode != new_state) {
				mode = new_state;
				glCullFace(mode);
			}
		}
		static void Enable(bool new_state) {
			if (new_state != enable) {
				enable = new_state;
				if (enable) glEnable(GL_CULL_FACE);
				else glDisable(GL_CULL_FACE);
			}
		}
		static void FrontFace(GLenum new_state) {
			if (frontFace != new_state) {
				frontFace = new_state;
				glFrontFace(frontFace);
			}
		}
	};
	class Point {
		static GLfloat size;
		static bool smooth;
	public:
		static void Smooth(bool enable) {
			if (smooth != enable) {
				smooth = enable;
				if (smooth) glEnable(GL_POINT_SMOOTH);
				else glDisable(GL_POINT_SMOOTH);
			}
		}
		static void Size(GLfloat _size) {
			if (size != _size) {
				size = _size;
				glPointSize(size);
			}
		}
	};
	class Line {
		static GLfloat width;
		static bool smooth;
	public:
		static void Smooth(bool enable) {
			if (smooth != enable) {
				smooth = enable;
				if (smooth) glEnable(GL_LINE_SMOOTH); 
				else glDisable(GL_LINE_SMOOTH);
			}
		}
		static void Width(GLfloat _width) {
			if (width != _width) {
				width = _width;
				glLineWidth(width);
			}
		}
	};
	static inline void PolygonMode(GLenum mode) {
		if (polygonMode != mode) {
			polygonMode = mode;
			glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		}
	}

	//Format:
	//GL_DEPTH_COMPONENT
	static inline void GetPixels(const IntRect& rect, GLenum format, float* pixel) {
		glReadPixels(rect.x, rect.y, rect.w, rect.h, format, GL_FLOAT, pixel);
	}

	static inline void unbind() {
		if (last_fbo != 0) {
			last_fbo = 0;
			glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);	
			Viewport::set(Window::size);			
		}
	}
	static inline void bind(GeneralRender& fbo) {
		if (last_fbo != fbo.id) {
			last_fbo = fbo.id;
			glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);			
			Viewport::set(fbo.size);
		}
	}
};
bool GlRender::CullFace::enable = 0;
GLenum  GlRender::CullFace::mode = GL_BACK;
GLenum  GlRender::CullFace::frontFace = GL_CCW;

GLfloat GlRender::Line::width = 0.f;
bool GlRender::Line::smooth = 0;

GLfloat GlRender::Point::size = 0.f;
bool GlRender::Point::smooth = 0;

IntRect  GlRender::Viewport::cur;

unsigned int GlRender::last_fbo = 0;
Color GlRender::cur_color_clear(0.f);
bool GlRender::isDepthTest = 0;
bool GlRender::isBlend = 0;
GLenum GlRender::polygonMode = GL_FILL;

#endif