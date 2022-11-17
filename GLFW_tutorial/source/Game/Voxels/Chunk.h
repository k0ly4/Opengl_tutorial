#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include <stack>
#include "Scene/Camera.h"
#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "Scene/Convex.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Light/LightMap.h"

///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public Drawable{
public:

	///Closes---------------------------------------------

	class Closes {

		public:
			inline void clear() {
				for (size_t i = 0; i < 6; i++) {
					chunks[i] = nullptr;
				}
			}

			inline void setModified(size_t index) {
				if (chunks[index]) chunks[index]->modified = 1;
			}

			inline void setModified() {
				for (size_t i = 0; i < 6; i++) {
					setModified(i);
				}
			}

			inline void setModified(const glm::uvec3& local) {

				if (local.x == CHUNK_W - 1) setModified(right);
				else if (local.x == 0) setModified(left);

				if (local.z == CHUNK_D - 1) setModified(front);
				else if (local.z == 0) setModified(back);
			}

			inline Chunk* get(size_t index) {
				return chunks[index];
			}

			Chunk* chunks[6];

	private:
			
	};

	Chunk(): global_(0) 
	{

		closes.clear();
		buffer.setDataDraw(DataDraw(DataDraw::DrawArrays, GlRender::TRIANGLES, 0));
		shaderHint = glShader::voxel;
		voxels.resize(CHUNK_SIZE);
	}

	inline void setPosition(const glm::uvec3&global) {
		local_ = global;
		global_ = global * CHUNK_VOLUME;
	}

	//init
	bool load(const glm::uvec3& global);

	bool save()const;

	//void setCloses(Chunk* chunks, size_t size);

	inline void setModified(){ modified = 1; }
	
	//Local
	inline bool isUnVisible(size_t x, size_t y, size_t z)const {
		return !(!isChunkBelong(x, y, z) || VoxelPack::isRender(getFromLocalCoord(x, y, z)));
	}
	
	//Global
	inline const Voxel* getFromGlobalCoord(const glm::uvec3& coord) const{
		glm::uvec3 local = coord - glm::uvec3(global_);
		return isChunkBelong(local) ? &voxels[getIndex(local)] : 0;
	}

	inline Voxel* getFromGlobalCoord(const glm::uvec3& coord) {
		glm::uvec3 local = coord - glm::uvec3(global_);
		return isChunkBelong(local) ? &voxels[getIndex(local)] : 0;
	}
	//Local
	inline Voxel& getFromLocalCoord(size_t x, size_t y, size_t z) {
		return voxels[getIndex(x, y, z)];
	}

	inline const Voxel& getFromLocalCoord(size_t x, size_t y, size_t z)const {
		return voxels[getIndex(x, y, z)];
	}

	inline const glm::uvec3& getGlobalPos()const { return global_; }

	inline const glm::uvec3& getLocalPos()const { return local_; }

	/// <summary>
	/// »змен€ет заданный в глобальных кординатах coord воксель, устанавлива€ себе и 6 ближайшим чанкам modified в true
	/// </summary>
	bool setVoxel(const Voxel& voxel, const glm::uvec3& coord);

	/// <summary>
	/// Lightning----------------
	/// </summary>
	

	inline unsigned char getLightLocal(const glm::uvec3& local, int channel_) {
		return isChunkBelong(local) ? lightMap.get(local, channel_) : 0;
	}
	inline unsigned char getLightLocal(size_t x,size_t y,size_t z, int channel_) {
		return getLightLocal(glm::uvec3(x, y, z), channel_);
	}

	inline unsigned char getLightGlobal(const glm::uvec3& coord, int channel_) {
		return lightMap.get(
			coord - global_,
			channel_);
	}

	inline void setLightGlobal(const LightUint8& light, int channel_) {
		modified = 1;
		lightMap.set(
			light.pos - global_,
			channel_,
			light.light);
	}

	//render
	void draw(const View* view, const Shader& shader);

	std::vector<Voxel>& getVoxels()				{ return voxels;}
	const std::vector<Voxel>& getVoxels()const	{ return voxels;}

	inline void setNull() {
		isInitLightMap = isGenerated = 0;
		modified = 1;
		for (size_t i = 0; i < CHUNK_SIZE; i++) voxels[i].id = ResourceVoxelPack::id_air;
	}

	bool isGenerated = 0;
	bool isInitLightMap = 0;

	LightMap lightMap;
	Closes closes;

private:

	inline std::string getFilePath()const {
		return std::string("saves\\"+std::to_string(local_.x) + '_' + std::to_string(local_.y) + '_' + std::to_string(local_.z) + ".chunk");
	}

	inline bool isFree(Voxel voxel, byte drawGroup) {
		return !VoxelPack::isRender(voxel) || (VoxelPack::get(voxel).drawGroup != drawGroup);
	}

	inline bool isFree(int x, int y, int z, byte drawGroup) {
		///Local test
		if (isChunkBelong(x, y, z)) return isFree(getFromLocalCoord(x, y, z),drawGroup);
		//Global
		const Chunk* chunk = closes.get(getSide(x, y, z, CHUNK_VOLUME));
		if (chunk == nullptr) return 0;
		return isFree( chunk->getFromLocalCoord( ::clip(x , CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D) ), drawGroup);
	}

	unsigned char LIGHT(int x, int y, int z,int channel) {
		if (isChunkBelong(x, y, z)) return lightMap.get(x,y,z, channel);
		Chunk* chunk = closes.get(getSide(x, y, z, CHUNK_VOLUME));
		return chunk != 0 ? chunk->lightMap.get(::clip(x, CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D), channel) : 0;
	}

	glm::vec4 getFastLight(int x,int y,int z);
	glm::vec4 getSoftLight(int x,int y,int z);

	void upMesh();
	void fastUpMesh();

	//render
	std::vector<Voxel>voxels;

	
	mutable bool modified = 1;

	glm::uvec3 local_, global_;

	TypeConvex<VoxelVertex> buffer;
	std::vector<unsigned int> indices;
};


#endif

