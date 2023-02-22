#include "Scene3D.h"
#include "UI/UiScene.h"
#include "Game/Voxels/ChunkMesh.h"

Scene3D::Scene3D(UiScene* ui_) :
	ui(ui_),
	player(ui_->texCowBoy)
{
}

void Scene3D::init(RenderWindow& window) {
	player.setCamera(camera);

	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::setFlipOnLoad(1);
	filin.load("asset\\image\\favor.jpg");
	world.init(camera);
	
	world.chunks.frustum.setCamInternals(camera.projData());
	world.chunks.frustum.setCamDef(camera.basis());
	sChunkMesh::set(std::make_shared<ChunkMeshBuilderDefault>());
}

void Scene3D::inForward(RenderTarget& target) {

	forwardSetupContext();

	CullFace::Enable(false);
	target.draw(world.chunks);
	player.input.cursor.draw(target);
	world.weather.draw(target,&camera,player.hitbox->position);
	//world.chunks.frustum.draw(&target);
	/*Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	CullFace::Enable(true);*/
	//target.draw(light.getPoints());
	//DrawObject
	//Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	//DrawObject
	//Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
}

void Scene3D::inGBuffer(RenderTarget& target) {
	Blend::Enable(false);
	Depth::Enable(true);
	CullFace::Mode(CullFace::Back);

	CullFace::Enable(false);

	CullFace::Enable(true);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i]);
	}
}