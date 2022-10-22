#ifndef CHUNK_H
#define CHUNK_H

#include <stack>
#include "Scene/Camera.h"
#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "Scene/Convex.h"
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

enum Side :size_t
{
	top, 
	bottom, 
	right, 
	left, 
	front, 
	back
};

inline Side getSide(const glm::ivec3& dist) {
	Side side;
	if (	 dist.x == -1) side = left;
	else if (dist.x == 1) side = right;
	else if (dist.y == -1) side = bottom;
	else if (dist.y == 1) side = top;
	else if (dist.z == -1) side = back;
	else if (dist.z == 1) side = front;
	else LOG(LogError, "Exit setcloses::side not init\n");
	return side;
}

inline Side getSide(int x, int y, int z) {
	return getSide(glm::ivec3(x, y, z));
}
///Voxel----------------------------------------------
/// <summary>
/// 
/// </summary>
struct Voxel {
	int id;
	Voxel(int id_) :id(id_) {}
	Voxel() {}
};
static bool operator==(const Voxel& left, const Voxel& right) {
	return (right.id == left.id);
}
inline bool isRender(const Voxel& voxel) {
	return (voxel.id > -1);
}
inline bool isValid(const Voxel& voxel) {
	return (voxel.id != -1);
}
///VoxelType----------------------------------------------
/// <summary>
/// 
/// </summary>
struct UvVoxel {

	void setSolid(int fill) {
		for (size_t i = 0; i < 6; i++)
			id[i] = fill;
	}
	int id[6];
};

///VoxelAtlas---------------------------
/// <summary>
/// 
/// </summary>
class VoxelAtlas {

public:

	enum IdUV :int
	{
		uv_turf = 0,
		uv_side_turf,
		uv_earth,
		uv_max,
	};

	enum IdVoxel :int
	{
		id_air = -1,
		id_turf = 0,
		id_earth,
		id_max,
	};

	
	void load(const std::string& path, size_t sizeVoxel);

	inline const glm::vec2& get(int id, int side)const {
		return uv[scan[id].id[side]];
	}

	inline const glm::vec2& get(const Voxel& id, int side)const {
		return get(id.id,side);
	}

	inline float getVoxelSize()const {
		return uvSize_;
	}

	void use(const Shader& shader)const {
		shader.uniform("configMaterial", 1);
		shader.uniform("baseColor", glm::vec3(1.f));
		texture_.use(0);
	}

private:

	glm::vec2 getUV(int id) {
		float u = (id % sizeVoxel_) * uvSize_;
		float v = 1.f - ((1.f + id / sizeVoxel_) * uvSize_);
		return glm::vec2(u, v);
	}

	std::vector<glm::vec2> uv;
	std::vector<UvVoxel> scan;

	float uvSize_;
	size_t sizeVoxel_ = 32;
	Texture2D texture_;

};

///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public Drawable{
public:

	Chunk() :
		size_(0),
		global_(0) {

		buffer.setDataDraw(DataDraw(DataDraw::DrawArrays, GlRender::TRIANGLES, 0));
		shaderHint = glShader::texture;
	}
	//init
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
		return isIn(local.x, local.y, local.z) ? &voxels[getIndex(local, size_)] : 0;
	}

	inline Voxel* getGlobal(const glm::uvec3& coord) {
		auto local = coord - glm::uvec3(global_);
		return isIn(local.x, local.y, local.z) ? &voxels[getIndex(local, size_)] : 0;
	}

	/// <summary>
	/// Изменяет заданный в глобальных кординатах coord воксель, устанавливая себе и 6 ближайшим чанкам modified в true
	/// </summary>
	bool setVoxel(const Voxel& voxel, const glm::uvec3& coord);

private:
	//Local
	inline Voxel& getLocal(size_t x, size_t y, size_t z) {
		return voxels[getIndex(x, y, z, size_)];
	}
	inline const Voxel& getLocal(size_t x, size_t y, size_t z)const {
		return voxels[getIndex(x, y, z, size_)];
	}

	//Global

	inline int normalize(int coord) {
		return coord >= size_ ? 1 : coord < 0 ? -1 : 0;
	}

	inline bool global_isUnVisible(int x, int y, int z) {
		///Local test
		if (isIn(x, y, z)) return !isRender(getLocal(x, y, z));
		return 1;
		//Global
		const Chunk* chunk = closes[getSide(normalize(x),normalize(y), normalize(z))];
		if (chunk == nullptr) return 0;
		return !(isRender( chunk->getLocal( ::normalize(x , size_), ::normalize(y, size_), ::normalize(z, size_)) ) );
	}

	void upMesh();

	const VoxelAtlas* atlas_ = 0;

	ConvexUV buffer;
	std::vector<Voxel>voxels;
	const Chunk *closes[6];

	size_t size_;
	mutable bool modified = 1;
	glm::ivec3 global_;
};

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
class ChunkHandle :public Drawable {

public:

	ChunkHandle():volume_(0,0,0) {
		shaderHint = glShader::texture;
	}
	void create(const glm::uvec3& volume);

	void setAtlas(const VoxelAtlas& atlas) {
		for (size_t i = 0; i < chunks_.size(); i++) {
			chunks_[i].setAtlas(atlas);
		}
	}
	//Global voxel coord
	const Voxel* getVoxel(const glm::ivec3& coord);

	/// <summary>
	/// coord - global chunk coordinates
	/// </summary>
	Chunk* get(const glm::ivec3& coord);
	void setVoxel(const Voxel&, const glm::ivec3& coord);
	void draw(const View* view, const Shader& shader) {
		for (size_t i = 0; i < chunks_.size(); i++) {
			chunks_[i].draw(view, shader);
		}	
	}
	bool setVoxel(const Voxel&, const glm::vec3& start_ray, const glm::vec3& direction_ray, float maxDistance, bool adMode);
private:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="start_position">-начало</param>
	/// <param name="direction">-направление</param>
	/// <param name="maxDistance">-максимальное расстояние</param>
	/// <param name="end">-кордината вывода конца пересеченного луча</param>
	/// <param name="norm">-вывод нормали вокселя</param>
	/// <param name="iend">кордината вывода в трехмерном массиве конца луча</param>
	/// <returns></returns>
	const Voxel* rayCast(
		const glm::vec3& start_position,
		const glm::vec3& direction,
		float maxDistance,
		glm::vec3& end,
		glm::vec3& norm,
		glm::ivec3& iend);

	const size_t CHUNK_SIZE = 16;
	std::vector<Chunk> chunks_;
	glm::uvec3 volume_;

};
#endif

