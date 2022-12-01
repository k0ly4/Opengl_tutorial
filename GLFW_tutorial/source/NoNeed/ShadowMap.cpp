#include"ShadowMap.h"

void ShadowMap::render(const View3D& view_player, RenderScene& render) {

	/*float near_plane = 1.0f, far_plane = 30.f;
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	view.setView(lightView);
	view.setProjection(lightProjection);*/

	// ѕрив€зываем и очищаем текущий каскад
	GlRender::bind(fbo);
	Depth::Enable(true);
	Depth::Mask(true);
	glClear(GL_DEPTH_BUFFER_BIT);
	const Shader& shader = glShader::get(glShader::depth);
	shader.use();
	
	shader.uniform("lightSpaceMatrix", view.getVP());
	render.inShadowMap(fbo, glShader::depth);

}