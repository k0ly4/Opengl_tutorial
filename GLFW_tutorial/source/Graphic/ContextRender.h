
#ifndef CONTEXT_RENDER_H
#define CONTEXT_RENDER_H

#include "Math/Math.h"
#include "Game/Lib/GLFW.h"
#include "Graphic/TextureEntity.h"
#include "Graphic/RenderEntity.h"

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
	enum FuncEnum:GLenum
	{
		Zero =						GL_ZERO,						//Коэффициент равен 0 (0, 0, 0, 0)
		One =						GL_ONE,							//Коэффициент равен 1 (1, 1, 1, 1)

		SrcColor =					GL_SRC_COLOR, 					//Коэффициент равен вектору источника цвета
		OneMinusSrcColor =			GL_ONE_MINUS_SRC_COLOR,			//Коэффициент равен 1 минус вектор источника цвета

		DstColor =					GL_DST_COLOR, 					//Коэффициент равен вектору цвета приемника
		OneMinusDstColor =			GL_ONE_MINUS_DST_COLOR,			//Коэффициент равен 1 минус вектор цвета приемника

		SrcAlpha =					GL_SRC_ALPHA,					//Коэффициент равен альфа - компоненте вектора цвета источника
		OneMinusSrcAlpha =			GL_ONE_MINUS_SRC_ALPHA,			//Коэффициент равен 1 − альфа вектора цвета источника 

		DstAlpha =					GL_DST_ALPHA,					//Коэффициент равен альфа - компоненте вектора цвета приемника 
		OneMinusDstAlpha =			GL_ONE_MINUS_DST_ALPHA,			//Коэффициент равен 1 − альфа вектора цвета приемника 

		ConstantColor =				GL_CONSTANT_COLOR,				//Коэффициент равен вектору постоянного цвета 
		OneMInusConstantColor =		GL_ONE_MINUS_CONSTANT_COLOR,	//Коэффициент равен 1 − вектор постоянного цвета 
		ConstantAlpha =				GL_CONSTANT_ALPHA,				//Коэффициент равен альфа - компоненте вектора постоянного цвета 
		OneMinusConstantAlpha =		GL_ONE_MINUS_CONSTANT_ALPHA,	//Коэффициент равен 1 − альфа вектора постоянного цвета 

		None
	};

	enum EquationEnum :GLenum
	{
		Add =				GL_FUNC_ADD,				// result = Источник + Приемник.
		Subtract =			GL_FUNC_SUBTRACT,			// result = Источник − Приемник.
		ReverseSubract =	GL_FUNC_REVERSE_SUBTRACT,	// result = Приемник − Источник.
		Min =				GL_MIN,						// result = min(Приемник, Источник).
		Max =				GL_MAX,						// result = max(Приемник, Источник).
	
	};
	static void Func(GLenum sourse, GLenum destination);
	static void Equation(GLenum mode);
	static void Enable(bool new_state);

private:

	Blend() {}
	~Blend() {}
	static GLenum modeEquation;
	static GLenum srcFunc, dstFunc;
	static bool isEnable;
};

/// GlRender------------------------------------------------------------
/// <summary>
/// Depth
/// </summary>
class Depth {
public:
	enum FuncEnum:GLenum
	{
		Always =	GL_ALWAYS, 		//Тест на глубину всегда проходит.
		Never =		GL_NEVER, 		//Тест на глубину никогда не проходит.
		Less =		GL_LESS, 		//Проходит, если значение глубины фрагмента меньше сохраненного значения глубины.
		Equal =		GL_EQUAL, 		//Проходит, если значение глубины фрагмента равно сохраненному значению глубины.
		Lequal =	GL_LEQUAL, 		//Проходит, если значение глубины фрагмента меньше или равно сохраненному значению глубины.
		Greater =	GL_GREATER, 	//Проходит, если значение глубины фрагмента больше сохраненного значения глубины.
		NotEqual =	GL_NOTEQUAL,	//	Проходит, если значение глубины фрагмента не равно сохраненному значению глубины.
		Gequal =	GL_GEQUAL, 		//Проходит, если значение глубины фрагмента больше или равно сохраненному значению глубины.
	};
	static void Enable(bool new_state);
	static void Mask(bool new_state);
	static void Func(GLenum func);
private:
	
	Depth() {}
	~Depth() {}

	static bool isEnableTest;
	static bool isEnable;
	static GLenum func_;
};
/// GlRender------------------------------------------------------------
/// <summary>
/// Depth
/// </summary>
class Stencil {
public:

	enum StandartMask :GLuint
	{
		Null = 0x00,		// каждый бит в буфере трафарета становится нулем (отключение записи)
		One = 0xFF,			// каждый бит записывается в буфер трафарета как есть
	};

	enum FuncEnum :GLenum
	{
		Always = GL_ALWAYS, 
		Never = GL_NEVER, 	
		Less = GL_LESS, 	
		Equal = GL_EQUAL, 	
		Lequal = GL_LEQUAL, 
		Greater = GL_GREATER, 
		NotEqual = GL_NOTEQUAL,
		Gequal = GL_GEQUAL, 
	};

	struct FuncPass {
		GLenum func;
		GLint ref;
		GLuint mask;
		FuncPass() :
			func(Always),
			mask(One),
			ref()
		{}
	};

	enum Action:GLenum
	{
		Keep = GL_KEEP,				//Сохраняется текущее значение трафарета.
		Zero = GL_ZERO,				//Значение трафарета устанавливается равным 0.
		Replace = GL_REPLACE,		//Значение трафарета заменяется значением ref, установленным с помощью функции glStencilFunc().
		Incr = GL_INCR,				//Значение трафарета увеличивается на 1, если оно меньше максимального значения.
		IncrWrap = GL_INCR_WRAP,	//То же, что и GL_INCR, с той лишь разницей, что, при превышении максимального значения, значение сбрасывается в 0.
		Decr = GL_DECR,				//Значение трафарета уменьшается на 1, если оно превышает минимальное значение.
		DecrWrap = GL_DECR_WRAP,	//То же, что и GL_DECR, но значение становится максимальным, если оно оказывается ниже 0.
		Invert = GL_INVERT,			//Побитовое инвертирование текущего значения буфера трафарета.
	};

	struct ActionPass {
		GLenum sfail;
		GLenum dpfail;
		GLenum dppass;

		ActionPass() :
			sfail(Keep),
			dpfail(Keep),
			dppass(Keep)
		{}

		ActionPass(GLenum sfail_, GLenum dpfail_, GLenum dppass_):
			sfail(sfail_),
			dpfail(dpfail_),
			dppass(dppass_)
		{}

		void implementContext() {
			glStencilOp(sfail, dpfail, dppass);
		}

	};

	static void Enable(bool new_state);

	static void Mask(GLuint mask);
	//func — задает функцию, используемую при тесте трафарета==
	// Функция применяется к сохраненному значению трафарета и ref - значению функции glStencilFunc().
	//ref — cодержимое буфера трафарета сравнивается с данным значением.
	//mask — указывает маску, которая сопоставляется в операции побитового 
	//И как с эталонным значением, так и с сохраненным значением трафарета перед их сравнением.
	static void Func(GLenum func, GLint ref, GLuint mask);
	//sfail — действие, которое нужно выполнить, если тест трафарета не пройден.
	//dpfail — действие, которое нужно выполнить, если тест трафарета пройден, но тест глубины — нет.
	//dppass — действие, которое нужно предпринять, если пройдены и тест трафарета, и тест глубины.
	static void Op(GLenum sfail, GLenum dpfail, GLenum dppass);

private:

	Stencil() {}
	~Stencil() {}

	static bool isEnable;
	static GLuint mask_;
	//FuncPass func_;
	static ActionPass action_;
};

/// Render------------------------------------------------------------
/// <summary>
/// Render
/// </summary>
class sRender {

public:

	enum Primitive :GLenum
	{
		POINTS = 0, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLES_STRIP, TRIANGLES_FAN, QUADS, PRIMITIVE_SIZE
	};
	enum BufferBit :GLbitfield
	{
		COLOR_BIT = GL_COLOR_BUFFER_BIT,
		DEPTH_BIT = GL_DEPTH_BUFFER_BIT,
	};
	/// Framebuffer------------------------------------------------------------
	class Framebuffer {
	public:

		static inline void gen(unsigned int& id)		{	glGenFramebuffers(1,	&id); }
		static inline void destroy(unsigned int id) {		glDeleteFramebuffers(1, &id); }
		//if not complete return 1
		inline static bool checkStatus() { return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE; }
		inline static bool checkStatus(const char* logInfo) {
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				LOG(LogError, logInfo);
				return 1;
			}
			return 0;
		}
		inline static void bindTexture2D(GLenum attachment,const TextureId& id,GLint level =0 )noexcept  {
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id.get(), level);
		}

		inline static void blit(
			unsigned int readId,const glm::uvec2& r_size,
			unsigned int writeId, const glm::uvec2& w_size,
			GLbitfield bit,GLenum filter = GL_NEAREST)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, readId);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, writeId);
			glBlitFramebuffer(0, 0, r_size.x, r_size.y, 0, 0, w_size.x, w_size.y, bit, filter);
		}

		static void unbind();
		static void bind(const iFramebuffer& fbo, bool renderMode = 1);

	private:
		static unsigned int last_fbo;
	};

	static inline void setDrawBuffers(GLsizei n,const GLenum* arr)noexcept {glDrawBuffers(n, arr); }
	static inline void setDrawBuffers(const std::vector<GLenum>&arr)noexcept { glDrawBuffers(arr.size(), arr.data()); }

	/// RenderBuffer------------------------------------------------------------
	class RenderBuffer {
	public:
		static inline void gen(unsigned int& id) {		glGenRenderbuffers(1, &id); }
		static inline void destroy(unsigned int id) {	glDeleteRenderbuffers(1, &id); }
		//if not complete return 1
		static void unbind() {
			
		}
		static void bind(const RenderbufferId& rbo) {
			glBindRenderbuffer(GL_RENDERBUFFER, rbo.get());
			
		}
		static void storage(const glm::ivec2& size_,GLenum internalFormat = GL_DEPTH_COMPONENT){
			glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size_.x, size_.y);
		}
		static void bindToFramebuffer(const RenderbufferId& rbo,GLenum attachment = GL_DEPTH_ATTACHMENT) {
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.get());
		}
	private:
		static unsigned int last_RBO;
	};
	/// Clear------------------------------------------------------------
	class Clear {
	public:

		static inline void	bit(GLbitfield bit_)noexcept { glClear(bit_); }
		static void			setColor(const Color& color);
		static inline void	setColor(float color) {							setColor(Color(color)); }
		static inline void	setColor(float r, float g, float b, float a) {	setColor(Color(r, g, b, a)); }

	private:
		static Color color_;

	};

	/// Viewport------------------------------------------------------------
	class  Viewport {
		
	public:

		static inline const IntRect& get() {return cur;}

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

	/// Point------------------------------------------------------------
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
	/// Line------------------------------------------------------------
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
	/// Draw------------------------------------------------------------
	class Draw {
		public:
			static void setPolygonMode(GLenum mode) {
				if (polygonMode != mode) {
					polygonMode = mode;
					glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
				}
			}
		private:
			static GLenum polygonMode;
	};
	
	//Format:
	//GL_DEPTH_COMPONENT
	static inline void GetPixels(const IntRect& rect, GLenum format, float* pixel) {
		glReadPixels(rect.x, rect.y, rect.w, rect.h, format, GL_FLOAT, pixel);
	}

private:

	sRender() {}
	~sRender() {}
	
}; 

#endif
