#include "Chunk.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>


//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

bool Chunk::load(size_t size, const glm::ivec3& global) {

	size_ = size;
	local_ = global;
	global_ = global * (int)size;

	if (FileManager::read(getFilePath(), voxels)) {
		modified = 1;
		for (size_t i = 0; i < 6; i++) {
			if (closes[i] == 0)continue;
			closes[i]->modified = 1;
		}
		return 1;

	}
	return 0;
}

bool Chunk::save()const {
	if (FileManager::write(getFilePath(), voxels)) {
		return 1;
	}
	return 0;
}

void Chunk::generate(size_t size,const glm::ivec3& global) {

		size_ = size;
		local_ = global;
		global_ = global* (int)size;

		voxels.resize(size * size * size);
	
		for (size_t y = 0; y < size; y++) {
			for (size_t z = 0; z < size; z++) {
				for (size_t x = 0; x < size; x++)
				{
					glm::ivec3 real_coord = glm::ivec3(x,y,z) + global_;
					int id = (glm::perlin(glm::vec3(real_coord) * 0.0125f) > 0.1f)-1;//real_y <= (height) * 60 + 30;
					if (real_coord.y <= 2)
						id = 1;
					getLocal(x, y, z).id = id;
				}
			}
		}

		modified = 1;

}

void Chunk::setCloses(const std::vector<Chunk>& chunks) {

	for (size_t i = 0; i < 6; i++) {
		closes[i] = nullptr;
	}

	for (size_t i = 0; i < chunks.size(); i++) {
		glm::ivec3 dist = (chunks[i].global_ / (int)size_ - global_ / (int)size_) ;
		if (((abs(dist.x) + abs(dist.y) + abs(dist.z)) != 1))
			continue;
		closes[getSide(dist)] = &chunks[i];
	}
}

void Chunk::modifiedCloses(const glm::uvec3& local) {

	if (local.x == size_ - 1) setModifiedCloses(right);
	else if (local.x == 0) setModifiedCloses(left);

	if (local.y == size_ - 1) setModifiedCloses(top);
	else if (local.y == 0) setModifiedCloses(bottom);

	if (local.z == size_ - 1) setModifiedCloses(front);
	else if (local.z == 0) setModifiedCloses(back);
}

bool Chunk::setVoxel(const Voxel& voxel, const glm::uvec3& coord) {
	Voxel* voxel_ = getGlobal(coord);
	if (voxel_ == 0) return 0;
	if (*voxel_ == voxel) return 1;

	*voxel_ = voxel;
	modified = 1;
	modifiedCloses(coord - glm::uvec3(global_));
	return 1;
}

inline void push_back(std::vector<unsigned>& indices,size_t begin) {
	indices.push_back(begin);
	indices.push_back(begin + 1);
	indices.push_back(begin + 2);
	indices.push_back(begin);
	indices.push_back(begin + 2);
	indices.push_back(begin + 3);
}

inline void push_back(TypeConvex<VoxelVertex>& convex, const glm::vec3& pos, const glm::vec2& uv,const glm::vec4& light) {
	convex.push_back(VoxelVertex(pos, uv, light));
}

void Chunk::upMesh() {	
	bool ambientOcclusion = 1;
	float aoFactor = 0.15f;
	buffer.clear();
	indices.clear();
	for (size_t y = 0; y < size_; y++) {
		for (size_t z = 0; z < size_; z++) {
			for (size_t x = 0; x < size_; x++) {
				const Voxel& voxel = getLocal(x, y, z);

				if (isRender(voxel) == 0) {
					continue;
				}
				float uvsize = atlas_->getVoxelSize();
				// AO values
				float l;
				float a, b, c, d, e, f, g, h;
				a = b = c = d = e = f = g = h = 0.0f;
				//top
				if (isFree(x, y + 1, z)) {

					l = 1.0f;
					if (ambientOcclusion) {
						a = isFree(x + 1, y + 1, z) * aoFactor;
						b = isFree(x, y + 1, z + 1) * aoFactor;
						c = isFree(x - 1, y + 1, z) * aoFactor;
						d = isFree(x, y + 1, z - 1) * aoFactor;

						e = isFree(x - 1, y + 1, z - 1) * aoFactor;
						f = isFree(x - 1, y + 1, z + 1) * aoFactor;
						g = isFree(x + 1, y + 1, z + 1) * aoFactor;
						h = isFree(x + 1, y + 1, z - 1) * aoFactor;
					}

					const glm::vec2& uv = atlas_->get(voxel, ::top);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(l* (c + d + e)));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(l* (c + b + f)));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x,			uv.y + uvsize), glm::vec4(l* (a + b + g)));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x,			uv.y),			glm::vec4(l *(a + d + h)));
				}

				if (isFree(x, y - 1, z)) {
					l = 0.75f;
					if (ambientOcclusion) {
						a = isFree(x + 1, y - 1, z) * aoFactor;
						b = isFree(x, y - 1, z + 1) * aoFactor;
						c = isFree(x - 1, y - 1, z) * aoFactor;
						d = isFree(x, y - 1, z - 1) * aoFactor;

						e = isFree(x - 1, y - 1, z - 1) * aoFactor;
						f = isFree(x - 1, y - 1, z + 1) * aoFactor;
						g = isFree(x + 1, y - 1, z + 1) * aoFactor;
						h = isFree(x + 1, y - 1, z - 1) * aoFactor;
					}

					const glm::vec2& uv = atlas_->get(voxel, ::bottom);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(l* (c + d + e)));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(l* (a + d + h)));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),  glm::vec4(l* (a + b + g)));
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(l *(c + b + f)));
				}

				if (isFree(x + 1, y, z)) {
					l = 0.95f;
					if (ambientOcclusion) {
						a = isFree(x + 1, y + 1, z) * aoFactor;
						b = isFree(x + 1, y, z + 1) * aoFactor;
						c = isFree(x + 1, y - 1, z) * aoFactor;
						d = isFree(x + 1, y, z - 1) * aoFactor;

						e = isFree(x + 1, y - 1, z - 1) * aoFactor;
						f = isFree(x + 1, y - 1, z + 1) * aoFactor;
						g = isFree(x + 1, y + 1, z + 1) * aoFactor;
						h = isFree(x + 1, y + 1, z - 1) * aoFactor;
					}

					const glm::vec2& uv = atlas_->get(voxel, ::right);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(l* (c + d + e)));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(l* (a + d + h)));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(l* (a + b + g)));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(l *(c + b + f)));
				}

				if (isFree(x - 1, y, z)) {
					l = 0.85f;
					
					if (ambientOcclusion) {
						a =  isFree(x - 1, y + 1, z) * aoFactor;
						b =  isFree(x - 1, y, z + 1) * aoFactor;
						c =  isFree(x - 1, y - 1, z) * aoFactor;
						d =  isFree(x - 1, y, z - 1) * aoFactor;

						e =  isFree(x - 1, y - 1, z - 1) * aoFactor;
						f =  isFree(x - 1, y - 1, z + 1) * aoFactor;
						g =  isFree(x - 1, y + 1, z + 1) * aoFactor;
						h =  isFree(x - 1, y + 1, z - 1) * aoFactor;
					}

					const glm::vec2& uv = atlas_->get(voxel, ::left);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(l* (c + d + e)));
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(l* (b + c + f)));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(l* (a + b + g)));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(l *(d + a + h)));
				}

				if (isFree(x, y, z + 1)) {
					l = 0.9f;
					if (ambientOcclusion) {
						a = isFree(x, y + 1, z + 1) * aoFactor;
						b = isFree(x + 1, y, z + 1) * aoFactor;
						c = isFree(x, y - 1, z + 1) * aoFactor;
						d = isFree(x - 1, y, z + 1) * aoFactor;

						e = isFree(x - 1, y - 1, z + 1) * aoFactor;
						f = isFree(x + 1, y - 1, z + 1) * aoFactor;
						g = isFree(x + 1, y + 1, z + 1) * aoFactor;
						h = isFree(x - 1, y + 1, z + 1) * aoFactor;
					}
					const glm::vec2& uv = atlas_->get(voxel, ::front);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(l* (c + d + e)));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(l* (b + c + f)));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(l* (a + b + g)));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(l *(a + d + h)));
				}
				if (isFree(x, y, z - 1)) {
					l = 0.8f;
					if (ambientOcclusion) {
						a = isFree(x, y + 1, z - 1) * aoFactor;
						b = isFree(x + 1, y, z - 1) * aoFactor;
						c = isFree(x, y - 1, z - 1) * aoFactor;
						d = isFree(x - 1, y, z - 1) * aoFactor;

						e = isFree(x - 1, y - 1, z - 1) * aoFactor;
						f = isFree(x + 1, y - 1, z - 1) * aoFactor;
						g = isFree(x + 1, y + 1, z - 1) * aoFactor;
						h = isFree(x - 1, y + 1, z - 1) * aoFactor;
					}
					const glm::vec2& uv = atlas_->get(voxel, ::back);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3( x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(l* (c + d + e)));
					push_back(buffer,glm::vec3( x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), glm::vec4(l* (a + d + h)));
					push_back(buffer,glm::vec3( x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(l* (a + b + g)));
					push_back(buffer,glm::vec3( x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(l *(b + c + f)));
				}							 					 		
			}
		}
	}
	modified = 0;
}

void Chunk:: draw(const View* view, const Shader& shader) {
	if (modified) upMesh();
	shader.use();
	view->useVP(shader);
	atlas_->use(shader);
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_)+glm::vec3(0.5f)  ));
	buffer.getVAO().begin();
	glDrawElements(GlRender::TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}

