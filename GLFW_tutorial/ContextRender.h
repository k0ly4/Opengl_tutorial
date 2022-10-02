
#ifndef CONTEXT_RENDER_H
#define CONTEXT_RENDER_H

#include "Math.h"
#include "GLFW.h"
#include "ContextWindow.h"

/// CullFace------------------------------------------------------------
/// <summary>
/// CullFace
/// </summary>
class CullFace {

public:
	enum ModeEnum :GLenum
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK,
	};

	enum FrontFaceEnum :GLenum
	{
		ccw = GL_CCW,
		cw = GL_CW,
	};

	//Обрезка задних или передних граней
	static void Mode(ModeEnum new_state) {
		if (mode != new_state) {
			mode = new_state;
			glCullFace(mode);
		}
	}
	//Включает контекст обрезки граней
	static void Enable(bool new_state) {
		if (new_state != enable) {
			enable = new_state;
			if (enable) glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
		}
	}
	//Задает определение предних граней по или против часовой стрелке
	static void FrontFace(FrontFaceEnum new_state) {
		if (frontFace != new_state) {
			frontFace = new_state;
			glFrontFace(frontFace);
		}
	}

private:

	static bool enable;
	static GLenum mode;
	static GLenum frontFace;

};

/// GlRender------------------------------------------------------------
/// <summary>
/// GlRender
/// </summary>
class Blend
{
public:
	
	static void Enable(bool new_state);

private:

	Blend();
	~Blend();

	static bool isEnable;
};



/// GlRender------------------------------------------------------------
/// <summary>
/// GlRender
/// </summary>
class GlRender {

public:

	enum Primitive :GLenum
	{
		POINTS = 0, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLES_STRIP, TRIANGLES_FAN, QUADS, SIZE
	};
	//if not complete return 1
	inline static bool checkFramebufferStatus() {
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
	}
	static void setClearColor(const Color& color);
	static inline void setClearColor(float color) {
		setClearColor(Color(color));
	}
	static inline void setClearColor(float r, float g, float b, float a) {
		setClearColor(Color(r, g, b, a));
	}

	static void DepthTest(bool enable);
	

	class  Viewport {
		
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

	private:
		static IntRect cur;
	};

	class Point {
		
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

	private:

		static GLfloat size;
		static bool smooth;

	};

	class Line {
		
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

	private:
		static GLfloat width;
		static bool smooth;
	};

	static void PolygonMode(GLenum mode);
	//Format:
	//GL_DEPTH_COMPONENT
	static inline void GetPixels(const IntRect& rect, GLenum format, float* pixel) {
		glReadPixels(rect.x, rect.y, rect.w, rect.h, format, GL_FLOAT, pixel);
	}

	static void unbind();
	static void bind(const GeneralRender& fbo, bool renderMode = 1);

private:

	GlRender() {}
	~GlRender() {}
	static Color cur_color_clear;
	static GLenum polygonMode;
	static bool isDepthTest;
	
	static unsigned int last_fbo;

}; 



#endif
