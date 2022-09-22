
#ifndef CONTEXT_RENDER_H
#define CONTEXT_RENDER_H

#include "Math.h"
#include "GLFW.h"
#include "ContextWindow.h"

class GlRender {

public:
	enum Primitive :GLenum
	{
		POINTS = 0, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLES_STRIP, TRIANGLES_FAN, QUADS, SIZE
	};

	static void setClearColor(const Color& color);
	static inline void setClearColor(float color) {
		setClearColor(Color(color));
	}
	static inline void setClearColor(float r, float g, float b, float a) {
		setClearColor(Color(r, g, b, a));
	}

	static void DepthTest(bool enable);
	static void Blend(bool enable);

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

	static void PolygonMode(GLenum mode);
	//Format:
	//GL_DEPTH_COMPONENT
	static inline void GetPixels(const IntRect& rect, GLenum format, float* pixel) {
		glReadPixels(rect.x, rect.y, rect.w, rect.h, format, GL_FLOAT, pixel);
	}

	static void unbind();
	static void bind(GeneralRender& fbo);

private:

	GlRender() {}
	~GlRender() {}
	static Color cur_color_clear;
	static GLenum polygonMode;
	static bool isDepthTest;
	static bool isBlend;
	static unsigned int last_fbo;

};
#endif
