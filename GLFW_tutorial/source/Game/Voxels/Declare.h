#ifndef DECLARE_VOXELS_H
#define DECLARE_VOXELS_H

#include "Math/GLM.h"
typedef unsigned short twobyte;
//Chunk--------------------------------------------------------
#define CHUNK_D				16
#define CHUNK_W				16
#define CHUNK_H				128

#define CHUNK_VOL			CHUNK_W * CHUNK_H * CHUNK_D
#define CHUNK_VEC			glm::uvec3(CHUNK_W,CHUNK_H,CHUNK_D)
#define CHUNK_IVEC			glm::ivec3(CHUNK_W,CHUNK_H,CHUNK_D)
//Region--------------------------------------------------------
#define REGION_SIZE			20
#define REGION_RADIUS		REGION_SIZE / 2
#define REGION_VEC			glm::uvec2(REGION_SIZE,REGION_SIZE)
#define REGION_IVEC			glm::ivec2(REGION_SIZE,REGION_SIZE)
#define REGION_VOL			REGION_SIZE*REGION_SIZE
//SuperRegion---------------------------------------------------
#define SUPREG_SIZE 3
#define SUPREG_SIZE_CHUNK	REGION_SIZE*SUPREG_SIZE
#define SUPREG_SIZE_VOX		SUPREG_SIZE_CHUNK * CHUNK_D
#define SUPREG_VOL_CH		SUPREG_SIZE * REGION_SIZE
#define SUPREG_VOL			SUPREG_SIZE * SUPREG_SIZE
//Func-----------------------------------------------------------

inline glm::uvec2 toChunkPosByVoxel(const glm::uvec3& posVx) noexcept { return { posVx.x / CHUNK_W,posVx.z / CHUNK_D }; }

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

inline size_t getIndex(size_t x, size_t y, size_t z, const glm::uvec3& size) {
	return ((y * size.z + z) * size.x + x);
}
inline size_t getIndex(const glm::uvec3& coord, const glm::uvec3& size) {
	return ((coord.y * size.z + coord.z) * size.x + coord.x);
}
inline size_t toInt(size_t x, size_t z, size_t size)	noexcept				{ return (z * size + x); }

inline size_t toInt(const glm::uvec2& coord, size_t size)	noexcept			{ return (coord.y * size + coord.x); }

inline size_t toInt(const glm::uvec2& coord, const glm::uvec2& size)noexcept	{ return (coord.y * size.x + coord.x); }

inline size_t toInt(const glm::uvec3& coord, const glm::uvec3& size) noexcept	{ return ((coord.y * size.z + coord.z) * size.x + coord.x);}

inline glm::uvec2 toCoord2(size_t index, size_t row)noexcept {
	return { index % row, index / row };
}
///Не использовать!!!!
inline glm::uvec2 toCoord2(size_t index, glm::uvec2 size) {	return glm::uvec2(index % size.x, index / size.y);	}
inline glm::uvec2 toCoord2(size_t index) { return glm::uvec2(index % CHUNK_W, index / CHUNK_D); }
inline glm::uvec3 toCoord3(size_t index) {
	size_t xz_size = CHUNK_W * CHUNK_D;
	glm::uvec2 xz = toCoord2(index % xz_size);
	return glm::uvec3(xz.x, index / xz_size, xz.y);
}

//Global

inline int clip(int coord, int max) {
	return coord >= max ? (coord - max) : coord < 0 ? (max + coord) : coord;
}
inline glm::ivec3 clip(const glm::ivec3& coord, const glm::ivec3& max) {
	return glm::ivec3(clip(coord.x, max.x), clip(coord.y, max.y), clip(coord.z, max.z));
}


#endif