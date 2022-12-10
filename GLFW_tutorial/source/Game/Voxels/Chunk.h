#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include <stack>
#include "Scene/Camera.h"
#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "Scene/Convex.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Light/LightMap.h"

///SortableShell---------------------------------------------
/// <summary>
/// 
/// </summary>
template <typename object, typename key>
class SortableShell {
public:
	SortableShell(object* obj_, key value_) :
		obj(obj_), value(value_)
	{}
	SortableShell() {}

	key value;
	object* obj;

	inline bool operator > (const SortableShell& str) const
	{
		return (value > str.value);
	}

	inline bool operator < (const SortableShell& str) const
	{
		return (value < str.value);
	}

private:
};

///iGeometry---------------------------------------------
/// <summary>
/// 
/// </summary>
template<typename T>
class iGeometry {
public:

	iGeometry():size_vertex_to_draw(0){
		VBO.begin();
		VAO.begin();
		T::attrib(VAO);
		VAO.end();
		VBO.end();
	}

	inline void saveInBuffer() {
		VAO.begin();
		VBO.begin();
		VBO.data(vertices);
		EBO.begin();
		EBO.data(indices);
		T::attrib(VAO);
		VAO.end();
		EBO.end();
		size_vertex_to_draw = indices.size();
	}

	inline void draw() {
		VAO.begin();
		glDrawElements(GlRender::TRIANGLES, size_vertex_to_draw, GL_UNSIGNED_INT,0);
	}

	inline void pushIndices(size_t begin) {
		indices.push_back(begin);
		indices.push_back(begin + 1);
		indices.push_back(begin + 2);
		indices.push_back(begin);
		indices.push_back(begin + 2);
		indices.push_back(begin + 3);
	}

	inline void push_back(const T& vertex) {
		vertices.push_back(vertex);
	}

	ArrayBufferObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;

	size_t size_vertex_to_draw;
	std::vector<unsigned int> indices;
	std::vector<T> vertices;
private:
};


///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class gChunk{
public:

	///Closes---------------------------------------------

	class Closes {

	public:

		Closes() {clear();}

		inline void clear() {
			for (size_t i = 0; i < Side2D::NuN; i++) {
				chunks[i] = nullptr;
			}
		}

		inline void setModified(size_t index) {
			if (chunks[index]) chunks[index]->modified = 1;
		}

		inline void setModified() {
			for (size_t i = 0; i < Side2D::NuN; i++) setModified(i);
		}

		inline void setModified(size_t x, size_t y) {

			if (x == CHUNK_W - 1) {
				setModified(Side2D::right);
				if (y == CHUNK_D - 1)setModified(Side2D::right_top);
				else if (y == 0) setModified(Side2D::right_bottom);
			}
			else if (x == 0) {
				setModified(Side2D::left);
				if (y == CHUNK_D - 1)setModified(Side2D::left_top);
				else if (y == 0) setModified(Side2D::left_bottom);
			}

			if (y == CHUNK_D - 1)setModified(Side2D::top);
			else if (y == 0) setModified(Side2D::bottom);	
			
			}

			inline gChunk* get(size_t index) {return chunks[index];}

			gChunk* chunks[Side2D::NuN];

	private:
			
	};

	gChunk(): global_(0)
	{
		voxels.resize(CHUNK_SIZE);
	}

	inline void setPosition(const glm::uvec3&global) {
		local_ = global;
		global_ = global * CHUNK_VOLUME;
	}

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
	inline Voxel& getFromLocalCoord(size_t x, size_t y, size_t z) {return voxels[getIndex(x, y, z)];}

	inline const Voxel& getFromLocalCoord(size_t x, size_t y, size_t z)const {return voxels[getIndex(x, y, z)];}

	inline const glm::uvec3& getGlobalPos()const { return global_; }

	inline const glm::uvec3& getLocalPos()const { return local_; }

	/// <summary>
	/// »змен€ет заданный в глобальных кординатах coord воксель, устанавлива€ себе и 6 ближайшим чанкам modified в true
	/// </summary>
	bool setVoxel(const Voxel& voxel, const glm::uvec3& coord);

	/// <summary>
	/// Lightning-------------------------------------------------------------------------------------
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

	std::vector<Voxel>& getVoxels()				{ return voxels;}
	const std::vector<Voxel>& getVoxels()const	{ return voxels;}

	inline bool isModified()const { return modified; }

	inline void setNull() {
		isInitLightMap = isGenerated = 0;
		modified = 1;
		for (size_t i = 0; i < CHUNK_SIZE; i++) voxels[i] = Voxel(vox::air);
	}

	bool isGenerated = 0;
	bool isInitLightMap = 0;

	
	Closes closes;
	LightMap lightMap;

protected:

	static inline bool isFree(Voxel voxel, byte drawGroup) {
		return !VoxelPack::isRender(voxel) || (VoxelPack::get(voxel).drawGroup != drawGroup);
	}

	inline bool isFree(int x, int y, int z, byte drawGroup) {
		///Local test
		if (isChunkBelong(x, y, z)) return isFree(getFromLocalCoord(x, y, z),drawGroup);
		//Global
		if ((size_t)y >= CHUNK_H) return 1;//66 66 [1]-67 66
		const gChunk* chunk = closes.get( Side2D::toSideI( x , z, CHUNK_W - 1));
		if (chunk == nullptr) return 0;
		return isFree( chunk->getFromLocalCoord( ::clip(x , CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D) ), drawGroup);
	}
	//render
	std::vector<Voxel>voxels;
	mutable bool modified = 1;
	glm::uvec3 local_, global_;

};
class Chunk;

///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public gChunk,public Drawable {
public:

	Chunk():gChunk(){
		mesh.VBO.setMode(GBO::DYNAMIC);
		mesh.EBO.setMode(GBO::DYNAMIC);
		shaderHint = glShader::voxel;
	}
	//render
	void draw(const View* view, const Shader& shader);
	void draw(const Shader& shader);
	iGeometry<VoxelVertex> mesh;

private:

	unsigned char LIGHT(int x, int y, int z, int channel) {
		
		if (isChunkBelong(x, y, z)) return lightMap.get(x, y, z, channel);
		if ((size_t)y >= CHUNK_H) return 0;
		gChunk* chunk = closes.get(Side2D::toSideI(x, z, CHUNK_W - 1));
		return chunk != 0 ? chunk->lightMap.get(::clip(x, CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D), channel) : 0;
	}
	inline unsigned char LIGHT(const glm::ivec3& coord, int channel) {LIGHT(coord.x, coord.y, coord.z, channel);}

	glm::vec4 getFastLight(int x, int y, int z);
	glm::vec4 getSoftLight(int x, int y, int z);

	struct Vec4M {
		union
		{	
			struct { glm::vec4 v; };
			struct { float m[4]; };
		};
		Vec4M(const glm::vec4& v_) :v(v_) {}
	};

	struct LightFace {
		union
		{
			glm::vec4 l0;
			struct { float l0m[4]; };
		};
		union
		{
			glm::vec4 l1;
			struct { float l1m[4]; };
		};
		union
		{
			glm::vec4 l2;
			struct { float l2m[4]; };
		};
		union
		{
			glm::vec4 l3;
			struct { float l3m[4]; };
		};
	};
	
	void getLightTop(int x, int y, int z,		LightFace& face);
	void getLightBottom(int x, int y, int z,	LightFace& face);
	void getLightRight(int x, int y, int z,		LightFace& face);
	void getLightLeft(int x, int y, int z,		LightFace& face);
	void getLightForward(int x, int y, int z,	LightFace& face);
	void getLightBack(int x, int y, int z,		LightFace& face);

	friend class ChunkMeshQueue;
	bool needUpBuffer = 0;

	void buildMesh();
	void fastUpMesh();
};
#endif

