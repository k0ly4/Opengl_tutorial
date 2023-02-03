#ifndef WEATHER_HANDLE_H
#define WEATHER_HANDLE_H
#include "Game/Lib/GLFW.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Math/Math.h"
#include "Scene/Plane.h"
#include "Game/Entities/Initiator.h"
class FogUniform {
public:
	inline void uniform(const Shader& shader) {
		shader.uniform("density", density);
		shader.uniform("gradient",gradient);
		shader.uniform("skyColor", skyColor);
	}
	float density, gradient;
	glm::vec3 skyColor;
private:

};

class WeatherHandle
{
	const glm::vec3 SKY = {0.3f,0.5f,1.f};
	class PlaneBoard {
	public:
		PlaneBoard() {
			mesh.vertices.resize(4);
			createMesh();
			color = Color::WHITE;
		}
		void draw(View3D* view) {
			const Shader& shader = glShader::get(glShader::test);
			shader.use();
			shader.uniform("projection", view->proj().mat4());
			shader.uniform("camera", glm::mat4(glm::mat3(view->view().mat4())));

			shader.uniform("model", glm::rotate(glm::mat4(1.f), angle, glm::vec3(1.f, 0.f, 0.f)));
			shader.uniform("color", color);
			texture_->use(0);

			Blend::Equation(Blend::Max);
			Depth::Mask(false);
			Depth::Func(Depth::Lequal);
			mesh.drawArrays(Render::TRIANGLES_STRIP);
			Depth::Mask(true);
			Depth::Func(Depth::Less);
			Blend::Equation(Blend::Add);
		}
		Color color;
		float angle;
		const Texture2D* texture_;
	
	private:

		void createMesh(float size = 0.2f,const glm::vec4& uv= { 0.f,0.f,1.f,1.f }) {
			mesh[0] = { glm::vec3(-size, -size,  0.0f),	glm::vec2(uv.x,  uv.y) };
			mesh[1] = { glm::vec3( size, -size,  0.0f),	glm::vec2(uv.z,  uv.y) };
			mesh[2] = { glm::vec3(-size,  size,  0.0f),	glm::vec2(uv.x,  uv.w) };
			mesh[3] = { glm::vec3( size,  size,  0.0f),	glm::vec2(uv.z,  uv.w) };
			mesh.saveInBuffer();
		}

		qGeometry<UvVertex> mesh;
	};

public:
	WeatherHandle() {
		sunFactor = 0.f;
		time = 0;
		fogShader.density = 0.1f;
		fogShader.gradient = 1.5f;
		sInitiator::init(*this);
	}
	inline Color colorSky() { return Color({ SKY *sunFactor,1.f });}
	inline void update(float delta) {
		angle = angle + (delta * timeFactor);
		if (angle > 1.f) angle -= 2.f;
		float abs_angl = abs(angle);
		sunFactor = 1.f - abs_angl;
		plane.angle = angle * PI-PI*0.5f;
		const float sunset = (40.f/180.f);
	
		if (abs_angl < sunset) {
			fSunset = 0.f;
		}
		else {
			fSunset = std::min(abs_angl - sunset,1.f);
		}
		
		plane.color = { 1.f+ fSunset,1.f- fSunset * 0.9f,1.f- fSunset * 1.1f,1.f };
		fogShader.skyColor = colorSky().vec3;
		time += delta;
	}
	inline float getSunFactor() {		return sunFactor;}
	inline glm::vec3 getSunLight() {	return {sunFactor,sunFactor * (1.f - fSunset),sunFactor * (1.f - fSunset) }; }

	inline void draw(RenderTarget& render,View3D* view, const glm::vec3& player_pos) {
		plane.texture_ = &VoxPack::get()->sun;
		
		plane.draw(view);
	}

	inline void draw(RenderTarget& render) {
		/*sprite.setTexture(VoxPack::get()->sun);
		sprite.transform.setPosition(100.f, 100.f);
		render.draw(sprite);*/
	}
	FogUniform fogShader;
	float timeFactor = 0.1f;
private:

	PlaneBoard plane;
	Sprite sprite;
	float fSunset;
	float time = 0.f;
	float angle = 0.f;
	float sunFactor = 0.f;
	qGeometry<UvVertex> mesh;

};
#endif

