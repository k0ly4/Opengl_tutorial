#ifndef PLAYER_H
#define PLAYER_H

#include "Scene/Camera.h"
#include "Game/World.h"
#include "Scene/Primitive.h"
class Player
{

public:
	Player(const Texture2D& texture) :cube(glShader::texture)
	{
		cube.getGraphic()->material.setBaseColor(Color::BLACK);
		cube.setScale(glm::vec3(1.1f));
		cube.genMesh(0.5f);
		GlRender::Line::Width(5.f);
		cube.getGraphic()->VAO.data_draw.data.mode_primitive = GlRender::LINE_STRIP;

	}
	void getMesh(std::vector<Vertex>& vertices);

	const glm::vec3& getPosition()const {
		return camera_->getPosition();
	}

	inline Basis getBasis()const {
		return Basis(camera_->getBasis().position, camera_->getBasis().right, camera_->getBasis().up, camera_->getBasis().front);
	}

	void setCamera(Camera& camera) {
		camera_ = &camera;
	}

	void upVoxelCursor(ChunkHandle& chunks);
	void setCurVoxel(const Voxel& voxel) {
		curVoxel = voxel;
	}
	void setVoxel(World& world,bool isModAdd);
	void draw(RenderTarget& target) {
		target.draw(cube);
	}
private:

	Voxel curVoxel = Voxel(2);
	Cube cube;
	float maxDistanceCursor = 10.f;
	glm::vec3 normCursor;
	glm::ivec3 posCursor;
	Camera* camera_ =0;

};

#endif