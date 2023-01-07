#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include <vector>
#include "Math/Math.h"
#include "System/Arrays.h"
///LightMap_-----------------------------------------
/// <summary>
/// 
/// </summary>
class LightMap
{
	public:

		LightMap() { map_.setNull();}

		inline byte get(int x, int y, int z, int channel)const	noexcept { return (map_(x,y,z) >> (channel << 2)) & 0xF;}
		inline byte get(const glm::uvec3& coord, int channel)const noexcept { return (map_(coord) >> (channel << 2)) & 0xF; }
		inline byte get(size_t i, int channel)const noexcept { return (map_[i] >> (channel << 2)) & 0xF; }
		inline ColorU get(int x, int y, int z)const noexcept {
			size_t i = map_.ind(x, y, z);
			return {getR(i),getG(i) ,getB(i) ,getS(i) };
		}
		inline byte getR(int x, int y, int z)const noexcept	{ return map_(x, y, z) & 0xF; }
		inline byte getG(int x, int y, int z)const noexcept	{ return (map_(x, y, z) >> 4) & 0xF;}
		inline byte getB(int x, int y, int z)const noexcept	{return (map_(x, y, z) >> 8) & 0xF;}
		inline byte getS(int x, int y, int z)const noexcept	{return (map_(x, y, z) >> 12) & 0xF;}
		
		inline byte getR(size_t i)const noexcept { return  map_[i] & 0xF; }
		inline byte getG(size_t i)const noexcept { return (map_[i] >> 4) & 0xF; }
		inline byte getB(size_t i)const noexcept { return (map_[i] >> 8) & 0xF; }
		inline byte getS(size_t i)const noexcept { return (map_[i] >> 12) & 0xF; }

		inline void setR(int x, int y, int z, int value)noexcept {
			twobyte& red = map_(x,y,z);
			red = (red & 0xFFF0) | value;
		}
		inline void setG(int x, int y, int z, int value)noexcept {
			twobyte& green = map_(x, y, z);
			green = (green & 0xFF0F) | (value << 4);
		}
		inline void setB(int x, int y, int z, int value)noexcept {
			twobyte& blue = map_(x, y, z);
			blue = (blue & 0xF0FF) | (value << 8);
		}
		inline void setS(int x, int y, int z, int value)noexcept {
			twobyte& sun = map_(x, y, z);
			sun = (sun & 0x0FFF) | (value << 12);
		}
		inline void set(int x, int y, int z, int channel, int value)noexcept {
			twobyte& ch = map_(x, y, z);
			ch = (ch & (0xFFFF & (~(0xF << (channel * 4))))) | (value << (channel << 2));
		}
		inline void set(const glm::uvec3& coord, int channel, int value)noexcept {
			twobyte& ch = map_(coord);
			ch = (ch & (0xFFFF & (~(0xF << (channel * 4))))) | (value << (channel << 2));
		}

	private:

		Array3d<CHUNK_W, CHUNK_H, CHUNK_D, twobyte> map_;
};

///LightUint8-------------------------------------------------------
/// <summary>
/// 
/// </summary>
struct LightUint8 {

	glm::uvec3 pos;
	unsigned char light;
	
	LightUint8() {}
	LightUint8(size_t x_, size_t y_, size_t z_, byte light_) :
		pos(x_, y_, z_),
		light(light_)
	{}
	LightUint8(const glm::uvec3& pos_, byte light_) :
		pos(pos_),
		light(light_)
	{}
};


///LightUint8-------------------------------------------------------
/// <summary>
/// 
/// </summary>
struct LightUint8RGB {

	glm::uvec3 pos;
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

