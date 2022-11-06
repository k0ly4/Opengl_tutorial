#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include <vector>
#include "Math/Math.h"
#define CHUNK_D 16
#define CHUNK_W 16
#define CHUNK_H 128

#define CHUNK_SIZE CHUNK_W * CHUNK_H * CHUNK_D
#define CHUNK_VOLUME glm::ivec3(CHUNK_W,CHUNK_H,CHUNK_D)

inline size_t getIndex(size_t x, size_t y, size_t z, size_t size) {
	return ((y * size + z) * size + x);
}
inline size_t getIndex(const glm::uvec3& coord, size_t size) {
	return ((coord.y * size + coord.z) * size + coord.x);
}

inline size_t toInt(size_t x, size_t y, size_t z, size_t size) {
	return ((y * size + z) * size + x);
}
inline size_t toInt(const glm::uvec3& coord, size_t size) {
	return ((coord.y * size + coord.z) * size + coord.x);
}
//3
inline size_t getIndex(size_t x, size_t y, size_t z) {
	return ((y * CHUNK_D + z) * CHUNK_W + x);
}
inline size_t getIndex(const glm::uvec3& coord) {
	return ((coord.y * CHUNK_D + coord.z) * CHUNK_W + coord.x);
}
inline size_t getIndex(size_t x, size_t y, size_t z, const glm::uvec3& size) {
	return ((y * size.z + z) * size.x + x);
}
inline size_t getIndex(const glm::uvec3& coord, const glm::uvec3& size) {
	return ((coord.y * size.z + coord.z) * size.x + coord.x);
}
//2
inline size_t toInt(size_t x, size_t z) {
	return (z * CHUNK_W + x);
}

inline size_t toInt(size_t x, size_t z, size_t size) {
	return (z * size + x);
}
inline size_t toInt(const glm::uvec2& coord, size_t size) {
	return (coord.y * size + coord.x);
}
inline size_t toInt(const glm::uvec2& coord, const glm::uvec2& size) {
	return (coord.y * size.x + coord.x);
}
//Global

inline int clip(int coord, int max) {
	return coord >= max ? (coord - max) : coord < 0 ? (max + coord) : coord;
}
inline glm::ivec3 clip(const glm::ivec3& coord, const glm::ivec3& max) {
	return glm::ivec3(clip(coord.x,max.x), clip(coord.y, max.y), clip(coord.z, max.z));
}

inline bool isChunkBelong(size_t x, size_t y, size_t z) {
	return (x < CHUNK_W && y < CHUNK_H && z < CHUNK_D);
}
inline bool isChunkBelong(const glm::uvec3& coord) {
	return (coord.x < CHUNK_W && coord.y < CHUNK_H && coord.z < CHUNK_D);
}



///LightMap_-----------------------------------------
/// <summary>
/// 
/// </summary>
class LightMap
{
	public:

		LightMap() {
			map_.resize(CHUNK_SIZE);
			for (size_t i = 0; i < CHUNK_SIZE; i++)
				map_[i] = 0;
		}

		inline unsigned char get(int x, int y, int z, int channel) {
			return (map_[getIndex(x,y,z)] >> (channel << 2)) & 0xF;
		}

		inline unsigned char get(const glm::uvec3& coord, int channel) {
			return (map_[getIndex(coord)] >> (channel << 2)) & 0xF;
		}
		inline unsigned char getR(int x, int y, int z) {
			return map_[getIndex(x, y, z)] & 0xF;
		}
		inline unsigned char getG(int x, int y, int z) {
			return (map_[getIndex(x, y, z)] >> 4) & 0xF;
		}
		inline unsigned char getB(int x, int y, int z) {
			return (map_[getIndex(x, y, z)] >> 8) & 0xF;
		}
		inline unsigned char getS(int x, int y, int z) {
			return (map_[getIndex(x, y, z)] >> 12) & 0xF;
		}

		inline void setR(int x, int y, int z, int value) {
			const int index = getIndex(x, y, z);
			map_[index] = (map_[index] & 0xFFF0) | value;
		}
		inline void setG(int x, int y, int z, int value) {
			const int index = getIndex(x, y, z);
			map_[index] = (map_[index] & 0xFF0F) | (value << 4);
		}
		inline void setB(int x, int y, int z, int value) {
			const int index = getIndex(x, y, z);
			map_[index] = (map_[index] & 0xF0FF) | (value << 8);
		}
		inline void setS(int x, int y, int z, int value) {
			const int index = getIndex(x, y, z);
			map_[index] = (map_[index] & 0x0FFF) | (value << 12);
		}
		inline void set(int x, int y, int z, int channel, int value) {
			const int index = getIndex(x, y, z);
			map_[index] = (map_[index] & (0xFFFF & (~(0xF << (channel * 4))))) | (value << (channel << 2));
		}
		inline void set(const glm::uvec3& coord, int channel, int value) {
			const int index = getIndex(coord);
			map_[index] = (map_[index] & (0xFFFF & (~(0xF << (channel * 4))))) | (value << (channel << 2));
		}

	private:

		std::vector<unsigned short> map_;
};

///LightUint8-------------------------------------------------------
/// <summary>
/// 
/// </summary>
struct LightUint8 {

	glm::ivec3 pos;
	unsigned char light;
	
	LightUint8() {}
	LightUint8(int x_, int y_, int z_, byte light_) :
		pos(x_, y_, z_),
		light(light_)
	{}
	LightUint8(const glm::ivec3& pos_, byte light_) :
		pos(pos_),
		light(light_)
	{}
};


///LightUint8-------------------------------------------------------
/// <summary>
/// 
/// </summary>
struct LightUint8RGB {

	glm::ivec3 pos;
	Uint8RGB light;

	LightUint8RGB() {}
	LightUint8RGB(int x_, int y_, int z_, unsigned char r,unsigned char b, unsigned char g) :
		pos(x_, y_, z_),
		light(r,g,b)
		
	{}
	LightUint8RGB(const glm::ivec3& pos_, const Uint8RGB& rgb) :
		pos(pos_),
		light(rgb)
	{}
};
#endif

