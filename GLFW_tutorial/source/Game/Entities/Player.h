#ifndef PLAYER_H
#define PLAYER_H

#include "Scene/Camera.h"
#include "Game/Voxels/World.h"
#include "Scene/Primitive.h"
#include "Physics/Hitbox.h"

//Player------------------------------------------------------
class InputPlayer {

public:

	inline void jump() { 
		hitbox->velocity += GAME::WORLD_UP* jumpImpulse;
		hitbox->grounded = 0; 
	}
	inline void cameraUpdate(const glm::vec2& pos) { 
		camera->mouse_move(pos); 
		camera->setPosition(hitbox->position); 
	}

	void moveUpdate(float time);
	void setVoxel(World& world, bool isModAdd);
	inline void setCurVoxel(const Voxel& voxel) { curVoxel = voxel; }

	float jumpImpulse = 20.f;
	float maxSpeed = 10.f;
	std::shared_ptr<Hitbox> hitbox;
	Camera* camera;
	
	glm::vec3 normCursor;
	glm::ivec3 posCursor;
	Voxel curVoxel = vox::earth;

private:

};

//Cursor3D------------------------------------------------------
class Cursor3D {
public:

	Cursor3D();

	void update(ChunkSectorRender& chunks, InputPlayer& input,const Basis& basis);
	void draw(RenderTarget& target);

	float maxDistanceCursor = 10.f;
	iGeometry <ConvexVertex> mesh;

private:
	glm::vec3 posCursor;
};

//Player------------------------------------------------------
class Player
{

public:

	Player(const Texture2D& texture) : input(){
		setHitbox(std::make_shared<Hitbox>());
		hitbox->create(glm::vec3(0.f), glm::vec3(0.4f, 0.9f, 0.4f), 60.f);
	}

	inline void setHitbox(std::shared_ptr<Hitbox> hitbox_) {
		hitbox = hitbox_;
		input.hitbox = hitbox;
	}

	inline void setCamera(Camera& camera) {
		camera_ = &camera;
		input.camera = &camera;
	}

	const glm::vec3& getPosition()const {return camera_->getPosition(); }

	inline Basis getBasis()const { return Basis(camera_->getBasis().position, camera_->getBasis().right, camera_->getBasis().up, camera_->getBasis().front);}
	inline void cursorUpdate(ChunkSectorRender& chunks) { cursor.update(chunks, input, getBasis()); }
	
	std::shared_ptr<Hitbox> hitbox;
	InputPlayer input;
	Cursor3D cursor;

private:

	Camera* camera_ =0;
};

#endif