#ifndef CHUNK_HANDLE_H
#define CHUNK_HANDLE_H

#include "Chunk.h"

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
class ChunkHandle :public Drawable {

public:

	ChunkHandle() :volume_(0, 0, 0) {
		shaderHint = glShader::voxel;
	}
	void create(const glm::uvec3& volume);
	void save()const;
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
	
	void draw(const View* view, const Shader& shader) {
		for (size_t i = 0; i < chunks_.size(); i++) {
			chunks_[i].draw(view, shader);
		}
	}

	void setVoxel(const Voxel&, const glm::ivec3& coord);
	bool setVoxel(const Voxel&, const glm::vec3& start_ray, const glm::vec3& direction_ray, float maxDistance, bool adMode);
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

	inline const Voxel* rayCast(
		const Basis& basis,
		float maxDistance,
		glm::vec3& end,
		glm::vec3& norm,
		glm::ivec3& iend) {
		return rayCast(basis.position, basis.front, maxDistance, end, norm, iend);
	}
private:
	const size_t CHUNK_SIZE = 16;

	std::vector<Chunk> chunks_;
	glm::uvec3 volume_;
};
#endif