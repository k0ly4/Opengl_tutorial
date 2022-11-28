#ifndef CHUNK_HANDLE_H
#define CHUNK_HANDLE_H

#include "Game/Entities/Observer.h"
#include "Region.h"
#include "Game/Voxels/SuperRegion.h"
///ChunkHandle---------------------------------------------
/// <summary>
///  
/// </summary>
class ChunkSectorRender :public Drawable, public uniListener<Chunk>{

public:

	ChunkSectorRender() :size_(0) {
		shaderHint = glShader::voxel;
	}

	void create(size_t radius);

	void update(const glm::ivec3& positionCamera);

	void save()const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="coord"> - Global voxel coord</param>
	/// <returns></returns>
	const Voxel* getVoxel(const glm::uvec3& coord);
	inline const Voxel* getVoxel(int x, int y, int z) { return  getVoxel(glm::uvec3(x, y, z)); }
	inline bool isObstacle(const glm::uvec3& coord) {
		const Voxel* voxel = getVoxel(coord);
		if (voxel && VoxelPack::isObstacle(*voxel))return 1;
		return 0;
	}
	inline bool isObstacle(size_t x,size_t y,size_t z) { return isObstacle(glm::uvec3(x, y, z)); }
	/// <summary>
	/// coord - global chunk coordinates
	/// </summary>
	Chunk* get(const glm::uvec3& coord);

	inline Chunk* getByVoxel(const glm::ivec3& coord) {
		return get(toLocal(coord));
	}

	void draw(const View* view, const Shader& shader) {
		for (size_t i = 0; i < chunks_.size(); i++) {
			chunks_[i]->draw(view, shader);
		}
	}

	void setVoxel(const Voxel&, const glm::ivec3& coord);
	/*bool setVoxel(const Voxel&, const glm::vec3& start_ray, const glm::vec3& direction_ray, float maxDistance, bool adMode);*/

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
	unsigned char getChannelLight(const glm::uvec3& coord, int channel);
	inline unsigned char getChannelLight(int x, int y, int z, int channel) { return getChannelLight(glm::ivec3(x, y, z), channel); }

	inline size_t size()const {
		return size_;
	}
	const glm::uvec2& getBegin()const {
		return begin_;
	}
	std::vector<Chunk*>& getChunks() {
		return chunks_;
	}

	SupReg region;

private:

	void extractFromRegion();

	//Проверяет допустимость локальных кординат
	inline bool isIn(const glm::uvec3& local) {
		return (local.x < size_ && local.z < size_ && local.y == 0);
	}

	inline glm::uvec3 toLocal(const glm::uvec3& global) {
		return ( glm::ivec3(global / CHUNK_VOLUME) - glm::ivec3(begin_.x, 0, begin_.y));
	}
	void lightFlagUp();
	

	std::vector<Chunk*> chunks_;
	size_t size_;
	glm::uvec2 begin_;
	glm::uvec2 cameraChunk_ = glm::uvec2(2);
};
#endif