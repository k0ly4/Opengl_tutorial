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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="coord"> - Global voxel coord</param>
	/// <returns></returns>
	const Voxel* getVoxel(const glm::ivec3& coord);
	inline const Voxel* getVoxel(int x, int y, int z) { return  getVoxel(glm::ivec3(x, y, z)); }

	/// <summary>
	/// coord - global chunk coordinates
	/// </summary>
	Chunk* get(const glm::ivec3& coord);

	inline Chunk* getByVoxel(const glm::ivec3& coord) {
		return get(toLocal(coord));
	}
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
	//Получить заданный канал цвета по глобальным кординатам вокселя
	unsigned char getChannelLight(const glm::ivec3& coord, int channel);
	inline unsigned char getChannelLight(int x, int y, int z, int channel) { return getChannelLight(glm::ivec3(x, y, z), channel); }

	const glm::uvec3& size()const {
		return volume_;
	}

private:
	//Проверяет допустимость локальных кординат
	inline bool isIn(const glm::ivec3& local) {
		if (local.x < 0 || local.z < 0 || local.y < 0 || local.x >= volume_.x || local.y >= volume_.y || local.z >= volume_.z) return 0;
		return 1;
	}

	inline glm::ivec3 toLocal(const glm::ivec3& global) {
		auto local = global / CHUNK_VOLUME;
		if (global.x < 0) local.x--;
		if (global.y < 0) local.y--;
		if (global.z < 0) local.z--;
		return local;
	}
	std::vector<Chunk> chunks_;
	glm::uvec3 volume_;
};
#endif