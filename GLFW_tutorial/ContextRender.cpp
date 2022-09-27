#include "ContextRender.h"


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

	void GlRender::Blend(bool enable) {
		if (isBlend != enable) {
			isBlend = enable;
			if (enable)glEnable(GL_BLEND);
			else glDisable(GL_BLEND);
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

void GlRender::bind(GeneralRender& fbo) {
	if (last_fbo != fbo.id_) {
		last_fbo = fbo.id_;
		glBindFramebuffer(GL_FRAMEBUFFER, last_fbo);
		Viewport::set(fbo.size_);
	}
}


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