#ifndef CHUNK_HANDLE_H
#define CHUNK_HANDLE_H
#include "Game/Entities/WeatherHandle.h"
#include "Game/Entities/Observer.h"
#include "Region.h"
#include "Game/Voxels/SuperRegion.h"
#include "Game/Voxels/ChunkMeshQueue.h"
#include "Math/Frustum.h"
///ChunkHandle---------------------------------------------
/// <summary>
///  
/// </summary>
class ChunkSectorRender :public Drawable, public uniListener<Chunk>/*, public uniListener<Chunk>*/{
	
public:

	ChunkSectorRender(SupReg* region) :
		size_(sSetup::distance_render),
		begCh_(0),
		viewCh_(glm::uvec3(sSetup::getBeginPos()) / CHUNK_VEC),
		region_(region) 
	{ 
		shaderHint = glShader::voxel; 
		chunks_.setSize(size_);
	}
	
	inline void create(size_t size, const glm::ivec3& posView) { 
		size_ = size;
		chunks_.setSize(size_);
		viewPos_ = posView;
		viewCh_ = glm::uvec2(viewPos_.x / CHUNK_W, viewPos_.z / CHUNK_D);
		extractFromRegion();
	}
	//определяет сторону сектора в центре которого наблюдатель
	void setSize(size_t size);
	void upView(const Camera& camera);
	inline Chunk* getByVoxel(size_t x, size_t y, size_t z) {	return get(toLocal(x, y, z)); }
	inline Chunk* getByVoxel(const glm::uvec3& coord) {			return getByVoxel(coord.x,coord.y,coord.z); }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="coord"> - Global voxel coord</param>
	/// <returns></returns>
	inline const Voxel* getVoxel(const glm::uvec3& coord) {
		Chunk* chunk = getByVoxel(coord);
		return chunk ? chunk->getGlobal(coord) : 0;
	}
	inline const Voxel* getVoxel(int x, int y, int z) { return  getVoxel(glm::uvec3(x, y, z)); }
	inline bool isObstacle(const glm::uvec3& coord) {
		const Voxel* voxel = getVoxel(coord);
		return (voxel && VoxPack::isSolid(*voxel));
	}
	inline bool isObstacle(size_t x,size_t y,size_t z) { return isObstacle(glm::uvec3(x, y, z)); }
	/// <summary>
	/// coord - global chunk coordinates
	/// </summary>
	

	inline void draw(const View* view, const Shader& shader) {
		if (ch_sort.size() == 0)return;
		//if (ch_sort[0].ch->flag.isModified()&& ch_sort[0].ch->flag.isInitLight()) ch_sort[0].ch->buildMesh();
		//ch_sort[0].ch->buildSortMesh(viewPos_);
		shader.use();
		view->useVP(shader);
		VoxPack::get()->use(shader);
		shader.uniform("color_factor", weather->getSunFactor());
		shader.uniform("ambient", 0.015f);
		
		for (int i = ch_sort.size() - 1; i > -1; i--) {
			if(ch_sort[i].ch->flag.isDraw) ch_sort[i].ch->drawOpaqueMesh(shader);
		}
		for (int i = ch_sort.size() - 1; i > -1; i--) {
			if (ch_sort[i].ch->flag.isDraw)	ch_sort[i].ch->drawSortMesh(shader);
		}
	}

	void setVoxel(const Voxel&, const glm::ivec3& coord);
	
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
	inline unsigned char getChannelLight(const glm::uvec3& coord, int channel) {
		Chunk* chunk = getByVoxel(coord);
		return chunk? chunk->lightMap.get(glm::uvec3(coord.x % CHUNK_W, coord.y, coord.z % CHUNK_D), channel):0;
	}
	inline unsigned char getChannelLight(int x, int y, int z, int channel) { return getChannelLight(glm::ivec3(x, y, z), channel); }

	inline size_t size()const {						return size_; }
	inline const glm::uvec2& getBegin()const {		return begCh_; }
	inline ChunkPtrs& chunks() {					return chunks_;}
	WeatherHandle* weather;
	FrustumG frustum;
private:

	void upChunks_sort();
	friend class ChunkMeshQueue;
	inline Chunk* get(const glm::uvec3& local) { return isIn(local) ? chunks_(local.x,local.z) : 0; }
	void extractFromRegion();

	//Проверяет допустимость локальных кординат
	inline bool isIn(const glm::uvec3& local) { return (local.x < size_ && local.z < size_ && local.y == 0);}

	inline glm::uvec3 toLocal(size_t x,size_t y,size_t z) {
		return glm::uvec3(x / CHUNK_W- begCh_.x, y / CHUNK_H, z / CHUNK_D - begCh_.y);
	}
	ChunkPtrs chunks_;
	SortableChunks ch_sort;

	size_t size_;
	glm::uvec2 begCh_;


	glm::uvec2 viewCh_;
	glm::ivec3 viewPos_;
	
	SupReg* region_;
};
#endif