#include "ContextRender.h"

/// Blend------------------------------------------------------------
/// <summary>
/// GlRender

void Blend::Enable(bool enable) {
	if (isEnable == enable) return;
	isEnable = enable;
	if (isEnable) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

bool Blend::isEnable = 0;

/// GlRender------------------------------------------------------------
 /// <summary>
 /// GlRender
 /// </summary>

 void GlRender::setClearColor(const Color& color) {
	if (cur_color_clear != color) {
		cur_color_clear = color;
		glClearColor(color.r, color.g, color.b, color.a);
	}
}

void GlRender::DepthTest(bool enable) {
	if (isDepthTest != enable) {
		isDepthTest = enable;
		if (enable)glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}
}

void GlRender::PolygonMode(GLenum mode) {
	if (polygonMode != mode) {
		polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	}
}

void GlRender::unbind() {
	if (last_fbo != 0) {
		last_fbo = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);
		Viewport::set(ContextWindow::size);
	}
}

void GlRender::bind(const GeneralRender& fbo,bool renderMode) {
	if (last_fbo != fbo.getId()) {
		last_fbo = fbo.getId();
		glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);
		if(renderMode)
			Viewport::set(fbo.getSize());
	}
}


bool CullFace::enable = 0;
GLenum  CullFace::mode = CullFace::Back;
GLenum CullFace::frontFace = CullFace::ccw;

GLfloat GlRender::Line::width = 0.f;
bool GlRender::Line::smooth = 0;

GLfloat GlRender::Point::size = 0.f;
bool GlRender::Point::smooth = 0;

IntRect  GlRender::Viewport::cur;

unsigned int GlRender::last_fbo = 0;
Color GlRender::cur_color_clear(0.f);
bool GlRender::isDepthTest = 0;

GLenum GlRender::polygonMode = GL_FILL;