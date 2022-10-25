#ifndef CHUNK_H
#define CHUNK_H

#include <stack>
#include "Scene/Camera.h"
#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "Scene/Convex.h"
#include "VoxelAtlas.h"

inline size_t getIndex(size_t x, size_t y, size_t z, size_t size) {
	return ((y * size + z) * size + x);
}

inline size_t getIndex(const glm::uvec3& coord, size_t size) {
	return ((coord.y * size + coord.z) * size + coord.x);
}

inline size_t getIndex(size_t x, size_t y, size_t z, const glm::uvec3& size) {
	return ((y * size.z + z) * size.x + x);
}

inline size_t getIndex(const glm::uvec3& coord, const glm::uvec3& size) {
	return ((coord.y * size.z + coord.z) * size.x + coord.x);
}

inline int normalize(int coord, int max) {
	return coord >= max ? (coord - max) : coord < 0 ? (max + coord) : coord;
}



///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public Drawable{
public:

	Chunk() :
		size_(0),
		global_(0) {
		for (size_t i = 0; i < 6; i++) {
			closes[i] = nullptr;
		}
		buffer.setDataDraw(DataDraw(DataDraw::DrawArrays, GlRender::TRIANGLES, 0));
		shaderHint = glShader::voxel;
	}
	//init
	bool load(size_t size, const glm::ivec3& global);
	bool save()const;
	void generate(size_t size, const glm::ivec3& global);
	void setCloses(const std::vector<Chunk>& chunks);
	void setAtlas(const VoxelAtlas& atlas) {
		atlas_ = &atlas;
	}
	//render
	void draw(const View* view, const Shader& shader);

	//Local
	inline bool isUnVisible(size_t x, size_t y, size_t z)const {
		return !(!isIn(x, y, z) || isRender(getLocal(x, y, z)));
	}
	inline bool isIn(size_t x, size_t y, size_t z)const {
		return (x < size_ && y < size_ && z < size_);
	}
	inline bool isIn(const glm::uvec3& coord)const {
		return (coord.x < size_&& coord.y < size_&& coord.z < size_);
	}
	//Global
	inline const Voxel* getGlobal(const glm::uvec3& coord) const{
		auto local = coord - glm::uvec3(global_);
		return isIn(local) ? &voxels[getIndex(local, size_)] : 0;
	}

	inline Voxel* getGlobal(const glm::uvec3& coord) {
		glm::uvec3 local = coord - glm::uvec3(global_);
		return isIn(local.x, local.y, local.z) ? &voxels[getIndex(local, size_)] : 0;
	}

	/// <summary>
	/// »змен€ет заданный в глобальных кординатах coord воксель, устанавлива€ себе и 6 ближайшим чанкам modified в true
	/// </summary>
	bool setVoxel(const Voxel& voxel, const glm::uvec3& coord);

private:

	inline std::string getFilePath()const {
		return std::string("saves\\"+std::to_string(local_.x) + '_' + std::to_string(local_.y) + '_' + std::to_string(local_.z) + ".chunk");
	}

	void setModifiedCloses(size_t index) {
		if (closes[index] == nullptr)return;
		closes[index]->modified = 1;
	}
	void modifiedCloses(const glm::uvec3& local);
	//Local
	inline Voxel& getLocal(size_t x, size_t y, size_t z) {
		return voxels[getIndex(x, y, z, size_)];
	}
	inline const Voxel& getLocal(size_t x, size_t y, size_t z)const {
		return voxels[getIndex(x, y, z, size_)];
	}

	//Global

	inline int normalize(int coord) {
		return coord >= (int)size_ ? 1 : coord < 0 ? -1 : 0;
	}

	inline bool isFree(int x, int y, int z) {
		///Local test
		if (isIn(x, y, z)) return !isRender(getLocal(x, y, z));
		//return 1;
		//Global
		const Chunk* chunk = closes[getSide(normalize(x),normalize(y), normalize(z))];
		if (chunk == nullptr) return 0;
		return !(isRender( chunk->getLocal( ::normalize(x , size_), ::normalize(y, size_), ::normalize(z, size_)) ) );
	}

	void upMesh();
	//render
	const VoxelAtlas* atlas_ = 0;
	TypeConvex<VoxelVertex> buffer;
	std::vector<Voxel>voxels;
	std::vector<unsigned int> indices;

	const Chunk *closes[6];
	mutable bool modified = 1;

	size_t size_;
	glm::ivec3 global_;
	glm::ivec3 local_;
};


#endif

