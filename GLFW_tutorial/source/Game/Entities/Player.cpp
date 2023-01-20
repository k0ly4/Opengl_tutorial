#include "Player.h"
#include "Input/Keyboard.h"


void InputPlayer::moveUpdate(float time) {
	hitbox->velocity.x = hitbox->velocity.z =0.f;
	const glm::vec3& vecRight = camera->basis().right;
	if (Keyboard::getKey(Keyboard::W).action) {
		hitbox->velocity += maxSpeed * glm::cross(GAME::WORLD_UP, vecRight);
	}
	else if (Keyboard::getKey(Keyboard::S).action) {
		hitbox->velocity += -maxSpeed * glm::cross(GAME::WORLD_UP,vecRight);
	}
	if (Keyboard::getKey(Keyboard::D).action) {
		hitbox->velocity += maxSpeed * vecRight;
	}
	else if (Keyboard::getKey(Keyboard::A).action) {
		hitbox->velocity += -maxSpeed * vecRight;
	}
}

void InputPlayer::setVoxel(World& world, bool isModAdd) {
	if (cursor.pos == glm::ivec3(0))return;

	if (isModAdd) {
		glm::ivec3 pos(cursor.pos + glm::ivec3(cursor.norm));
		if (VoxPack::isLiquid(inventory->get().vox)) {
			inventory->get().vox.e.m1 = VoxPack::maxConcLiquid;
		}
		world.light.addToQueue(pos, inventory->get().vox);
		world.chunks.setVoxel(inventory->get().vox, pos);
	}
	else {
		world.light.addToQueue(cursor.pos, vox::air);
		world.chunks.setVoxel(vox::air, cursor.pos);
	}
}

inline void push_back(iGeometry<ConvexVertex>&mesh,	const glm::vec3& pos) {
	mesh.push_back(ConvexVertex(pos, glm::vec4(0.f,0.f,0.f,1.f)));
}
inline void push_line(std::vector<unsigned>& indices,size_t i1,size_t i2) {
	indices.push_back(i1);
	indices.push_back(i2);
}

Cursor3D::Cursor3D():
	pos(0),
	norm(0.f)
{
	
	std::vector<unsigned>& ind = mesh.indices;
	push_back(mesh, glm::vec3(0.f, 0.f, 0.f));//0
	push_back(mesh, glm::vec3(1.f, 0.f, 0.f));
	push_back(mesh, glm::vec3(0.f, 1.f, 0.f));//2
	push_back(mesh, glm::vec3(0.f, 0.f, 1.f));//3
	push_back(mesh, glm::vec3(1.f, 1.f, 0.f));//4
	push_back(mesh, glm::vec3(0.f, 1.f, 1.f));
	push_back(mesh, glm::vec3(1.f, 0.f, 1.f));//6
	push_back(mesh, glm::vec3(1.f, 1.f, 1.f));//7

	for (size_t i = 0; i < mesh.vertices.size(); i++) {
		mesh.vertices[i].pos = mesh.vertices[i].pos* 1.001f - glm::vec3(0.0005f);
	}

	push_line(ind, 0,1);
	push_line(ind, 0,2);
	push_line(ind, 0,3);

	push_line(ind, 1, 4);
	push_line(ind, 2, 5);
	push_line(ind, 3, 5);

	push_line(ind, 6, 1);
	push_line(ind, 6, 3);
	push_line(ind, 2, 4);

	push_line(ind, 7, 6);
	push_line(ind, 7, 5);
	push_line(ind, 7, 4);

	mesh.saveInBuffer();
}
void Cursor3D::rayCast(ChunkSectorRender& chunks, const Basis& basis) {
	glm::vec3 end;
	if (chunks.rayCast(basis, maxDistanceCursor, end, norm, pos) == 0) pos= glm::ivec3(0);
}

void Cursor3D::draw(RenderTarget& target){
	const Shader& shader = glShader::get(glShader::color_layout);
	shader.use();
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(pos)));
	target.getView()->use(shader);
	mesh.draw(Render::LINES);
}
