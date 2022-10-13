#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include"DepthFrameBuffer.h"

class ShadowMap{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
public:

	ShadowMap()
	{
		fbo.create(SHADOW_WIDTH, SHADOW_HEIGHT);

		float near_plane = 1.0f, far_plane = 30.f;
		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		view.setView(lightView);
		view.setProjection(lightProjection);
		fbo.setView(view);
	}

	const glm::ivec2& getSize()const {
		return glm::ivec2(SHADOW_WIDTH, SHADOW_HEIGHT);
	}

	void uniform(const Shader& shader, const std::string& name, size_t unit) {
		fbo.getTexture().use(unit);
		shader.uniform("gWVP", view.getVP());
	}
	void setView(const View3D& camera) {
		view.setView(camera.getView().get());
		//view.setProjection(camera.getProjection().get()); 

	}

	void render(const View3D& view_player, RenderScene& render);

	Texture2D getTexture() {
		return fbo.getTexture();
	}
private:
	ViewMatrix view;
	RenderTextureDepth fbo;
};

#endif
