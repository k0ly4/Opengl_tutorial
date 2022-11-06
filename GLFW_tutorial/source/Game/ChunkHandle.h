#ifndef CHUNK_HANDLE_H
#define CHUNK_HANDLE_H

#include "Chunk.h"

///Region---------------------------------------------
/// <summary>
///  
/// </summary>
class Region {

public:

	Region() {
		if(load(buffer,center)==0) generate(buffer,center);
		for (size_t i = 0; i < buffer.size(); i++) 
			buffer[i].setCloses(buffer);
	}

	void fill(std::vector<Chunk*>& target, size_t size, const glm::ivec2& center);
	inline bool save()const {
		return save(getPath(), buffer);
	}
	static bool save(const std::string& path, const std::vector<Chunk>& chunks);

private:

	static inline std::string getPath(const glm::ivec2& center) {
		return DIRECTORY + std::to_string(center.x) + "@" + std::to_string(center.y) + EXTENSION;
	}

	inline std::string getPath()const {
		return getPath(center);
	}

	static void generate(std::vector<Chunk>& empty,const glm::ivec2& center);
	static bool load(std::vector<Chunk>& empty, const glm::ivec2& center);
	void generate(Chunk& chunk);
	bool load(const Chunk);

	static const std::string DIRECTORY;
	static const std::string EXTENSION;

	static const size_t region_size = 6;
	
	glm::ivec2 center = glm::ivec2(0,0);
	std::vector<Chunk> buffer;
};

///ChunkHandle---------------------------------------------
/// <summary>
///  
/// </summary>
class ChunkHandle :public Drawable {

public:

	ChunkHandle() :size_(0) {
		shaderHint = glShader::voxel;
	}

	void create(size_t radius);

	void save()const;

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
			chunks_[i]->draw(view, shader);
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

	inline size_t size()const {
		return size_;
	}

private:
	//Проверяет допустимость локальных кординат
	inline bool isIn(const glm::ivec3& local) {
		if (local.x < 0 || local.z < 0 || local.x >= size_ || local.z >= size_ || local.y!=0) return 0;
		return 1;
	}

	inline glm::ivec3 toLocal(const glm::ivec3& global) {
		auto local = global / CHUNK_VOLUME;
		if (global.x < 0) local.x--;
		if (global.y < 0) local.y--;
		if (global.z < 0) local.z--;
		return local;
	}

	Region region;

	std::vector<Chunk*> chunks_;
	size_t size_;
	glm::ivec2 center;
};
#endif