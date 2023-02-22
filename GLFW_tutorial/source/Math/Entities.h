#ifndef MATH_ENTITIES_H
#define MATH_ENTITIES_H

#include "Math/GLM.h"

template<typename T>
struct Range {
	T min, max;
	Range(T min_, T max_) :min(min_), max(max_) {}
	Range() :Range(0, 0) {}
	inline bool is(T value) { return (value <= max) && (value >= min); }
};

namespace vox {
	enum ShiftBit :twobyte {
		//концентрация
		shift_density = 0x0u,
	};
	enum MaskBit :twobyte {
		//концентрация
		mask_density = 0x7u,
	};
}

///extendedID----------------------------------------------
/// <summary>
/// 
/// </summary>
struct extendedID {

	twobyte id;
	union {
		twobyte m;
		struct {
			byte m1;
			byte m2;
		};
	};
	inline twobyte getBits(twobyte shift, twobyte mask = 0x7u) noexcept { return (m >> shift) & mask; }
	inline byte setBits(twobyte value, twobyte shift, twobyte mask = 0x7u) noexcept {
		m = m
			/*maskMult*/ & (~(mask << shift))
			/*maskAdd*/ | ((value & mask) << shift);
	}
	extendedID() {}
	extendedID(twobyte id_, byte m1_, byte m2_) :
		id(id_),
		m1(m1_),
		m2(m2_) {}
};
///Voxel----------------------------------------------
/// <summary>
/// 
/// </summary>
struct Voxel {



	union
	{

		int id_;
		extendedID e;

	};
	//inline byte density()noexcept { return e.bitsM(vox::b_density); }
	//Возвращает биты содержащие концентрацию<7
	inline byte density()noexcept { return e.m1; }
	//Заносит биты содержащие концентрацию<7
	inline void setDensity(byte value)noexcept { e.setBits(value, vox::shift_density, vox::mask_density); }
	//Voxel(int id_) :id(id_) {}
	Voxel(twobyte id, byte m1 = 0, byte m2 = 0) :e(id, m1, m2)
	{}
	Voxel() :e(0, 0, 0) {}
};

static bool operator==(const Voxel& left, const Voxel& right) { return (right.id_ == left.id_); }
static bool operator!=(const Voxel& left, const Voxel& right) { return (right.id_ != left.id_); }
#endif