#ifndef PLAYER_H
#define PLAYER_H

#include "Scene/Camera.h"
#include "Game/Voxels/World.h"
#include "Physics/Hitbox.h"
#include "UI/Shape.h"
//Goods------------------------------------------------------
struct Goods
{
	Voxel vox;
	byte size;
	Goods(byte size_, Voxel vox_) :
		size(size_),
		vox(vox_) {}
	Goods() :Goods(0, 0) {}
};
#define INVENT_COL 9
#define INVENT_ROW 4
#define INVENT_SIZE INVENT_COL*INVENT_ROW
#define STACK 64
//Inventory------------------------------------------------------
class Inventory {
public:
	void clear() {
		for (size_t i = 0; i < INVENT_SIZE; i++) el[i].size = 0;
	}
	Inventory() {
		clear();
		init();
	}
	inline void init() {
		
		border.transform.setOrigin(begPos);
		frame.transform.setOrigin(begPos);

		border.setFillColor(Color::ColorU(100, 100, 100, 200));
		border.setSize(64.f, 64.f);
		border.setOutlineColor(Color::ColorU(100, 100, 100));
		border.setOutlineThickness(4.f);

		frame.setSize(64.f, 64.f);
		frame.setFillColor({ 0.f, 0.f, 0.f, 0.f });
		frame.setOutlineColor(Color::BLACK);
		frame.setOutlineThickness(5.f);

	}
	inline void set(const Goods& thing) { el[curEl] = thing; }
	inline Goods& get() { return el[curEl]; }

	inline void draw(RenderTarget& target) {
		drawBorders(target);
		frame.transform.setPosition(64.f * curEl, 0.f);
		drawGoods(target);
		target.draw(frame);
	}

	size_t curEl = 0;
	Goods el[INVENT_SIZE];

private:

	inline void drawBorders(RenderTarget& target) {
		for (size_t i = 0; i < INVENT_COL; i++) {
			border.transform.setPosition(i * 64.f, 0.f);
			target.draw(border);
		}
	}

	inline void drawGoods(RenderTarget& target) {
		for (size_t i = 0; i < INVENT_COL; i++) {
			if (el[i].size == 0)continue;
			Sprite& sprite = VoxPack::getSprite(el[i].vox);
			sprite.transform.setOrigin(begPos);
			sprite.transform.setPosition(i * 64.f, 0.f);
			target.draw(sprite);
		}
	}
	glm::vec2 begPos = { 300.f,730.f };
	RectangleShape  border, frame;
};
//Cursor3D------------------------------------------------------
class Cursor3D {
public:

	Cursor3D();
	void rayCast(ChunkSectorRender& chunks, const Basis& basis);
	void draw(RenderTarget& target);

	float maxDistanceCursor = 10.f;
	iGeometry <ConvexVertex> mesh;
	glm::ivec3 pos;
	glm::vec3 norm;
private:
};

//InputPlayer------------------------------------------------------
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
	inline void setCurVoxel(const Voxel& voxel) {inventory->set({ STACK,voxel }); }

	float jumpImpulse = 20.f;
	float maxSpeed = 10.f;
	std::shared_ptr<Hitbox> hitbox;
	Camera* camera;
	Inventory* inventory;
	Cursor3D cursor;
private:

};


//Player------------------------------------------------------
class Player
{

public:

	Player(const Texture2D& texture) : input(){
		setHitbox(std::make_shared<Hitbox>());
		hitbox->create(glm::vec3(0.f), glm::vec3(0.4f, 0.9f, 0.4f), 60.f);
		inventory.init();
		input.inventory = &inventory;
		input.setCurVoxel(vox::grass);
	}

	inline void setHitbox(std::shared_ptr<Hitbox> hitbox_) {
		hitbox = hitbox_;
		input.hitbox = hitbox;
	}

	inline void setCamera(Camera& camera) {
		camera_ = &camera;
		input.camera = &camera;
	}

	const glm::vec3& getPosition()const {return camera_->position(); }

	inline Basis getBasis()const { 
		return Basis(camera_->basis().position, camera_->basis().right, camera_->basis().up, camera_->basis().front);}
	inline void cursorUpdate(ChunkSectorRender& chunks) { input.cursor.rayCast(chunks, getBasis()); }

	std::shared_ptr<Hitbox> hitbox;
	InputPlayer input;
	Inventory inventory;

private:

	Camera* camera_ =0;
};

#endif