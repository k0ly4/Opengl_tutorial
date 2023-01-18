#include "ContextRender.h"

/// Blend------------------------------------------------------------
/// <summary>
/// GlRender
/// 
void Blend::Func(GLenum source, GLenum destination) {
	if (srcFunc == source && dstFunc == destination) return;
	srcFunc = source;
	dstFunc = destination;
	glBlendFunc(srcFunc, dstFunc);
}

void Blend::Equation(GLenum mode) {
	if (modeEquation == mode) return;
	modeEquation = mode;
	glBlendEquation(modeEquation);
}

void Blend::Enable(bool enable) {
	if (isEnable == enable) return;
	isEnable = enable;
	if (isEnable) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

bool Blend::isEnable = false;

GLenum Blend::srcFunc = Blend::None;
GLenum Blend::dstFunc = Blend::None;

GLenum Blend::modeEquation = Blend::Add;
/// Depth------------------------------------------------------------
 /// <summary>
 /// Depth
 /// </summary>
void Depth::Enable(bool enable) {
	if (isEnable == enable) return;
	isEnable = enable;
	if (enable)glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
	
}

void Depth::Mask(bool enable) {
	if (isEnableTest == enable) return;
	isEnableTest = enable;
	glDepthMask(enable);
}

void Depth::Func(GLenum func) {
	if (func_ == func)return;
	func_ = func;
	glDepthFunc(func_);
}

GLenum Depth::func_ = Depth::Less;
bool Depth::isEnable = 0;
bool Depth::isEnableTest =1;

/// Stencil------------------------------------------------------------
 /// <summary>
 /// Depth
 /// </summary>
void Stencil::Enable(bool enable) {
	if (isEnable == enable) return;
	isEnable = enable;
	if (enable)glEnable(GL_STENCIL_TEST);
	else glDisable(GL_STENCIL_TEST);
}

void Stencil::Mask(GLuint mask) {
	if (mask_ == mask) return;
	mask_ = mask;
	glStencilMask(mask_);
}

void Stencil::Func(GLenum func,GLint ref,GLuint mask) {
	glStencilFunc(func, ref, mask);
}

void Stencil::Op(GLenum sfail, GLenum dpfail, GLenum dppass) {
	if (action_.sfail == sfail && action_.dpfail == dpfail && action_.dppass == dppass)
		return;
	action_ = ActionPass(sfail, dpfail, dppass);
	action_.implementContext();
}

bool Stencil::isEnable = 0;
GLuint Stencil::mask_ = Stencil::One;
Stencil::ActionPass Stencil::action_;

/// GlRender------------------------------------------------------------
 /// <summary>
 /// GlRender
 /// </summary>

 void Render::setClearColor(const Color& color) {
	if (cur_color_clear != color) {
		cur_color_clear = color;
		glClearColor(color.r, color.g, color.b, color.a);
	}
}

void Render::PolygonMode(GLenum mode) {
	if (polygonMode != mode) {
		polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	}
}

void Render::unbind() {
	if (last_fbo != 0) {
		last_fbo = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);
		Viewport::set(ContextWindow::size);
	}
}

void Render::bind(const iFrame& fbo,bool renderMode) {
	if (last_fbo != fbo.id()) {
		last_fbo = fbo.id();
		glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);
		if(renderMode) Viewport::set(fbo.size());
	}
}


bool CullFace::enable = 0;
GLenum  CullFace::mode = CullFace::Back;
GLenum CullFace::frontFace = CullFace::ccw;

GLfloat			Render::Line::width = 0.f;
bool			Render::Line::smooth = 0;

GLfloat			Render::Point::size = 0.f;
bool			Render::Point::smooth = 0;

IntRect			Render::Viewport::cur;

unsigned int	Render::last_fbo = 0;
Color			Render::cur_color_clear(0.f);
GLenum			Render::polygonMode = GL_FILL;