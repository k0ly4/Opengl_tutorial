#ifndef SYS_ENTITIES_H
#define SYS_ENTITIES_H
#include <iostream>
#include <bitset>
/// <summary>
/// 
/// </summary>
template<typename T> 
class Bits {
	
	inline operator T()noexcept { return v_; }
	inline T get(T shift, T mask)const noexcept  { return (v_ >> shift) & mask; }
	inline T get2(T shift = 0)const noexcept { return (v_ >> shift) & 0x3; }
	inline T get4(T shift = 0)const noexcept { return (v_ >> shift) & 0xF; }
	inline T get8(T shift = 0)const noexcept { return (v_ >> shift) & 0xFF; }

	inline void set(T value, T shift, T mask) noexcept {
		v_ = v_
			/*maskMult*/ & (~(mask << shift))
			/*maskAdd*/ | ((value & mask) << shift);
	}
	inline void set2(T value, T shift = 0) noexcept {
		v_ = v_& (~(0x3 << shift))| ((value & 0x3) << shift);
	}
	inline void set4(T value, T shift = 0) noexcept {
		v_ = v_& (~(0xF << shift)) | ((value & 0xF) << shift);
	}
	inline void set8(T value, T shift = 0) noexcept {
		v_ = v_& (~(0xFF << shift)) | ((value & 0xFF) << shift);
	}
	inline void print() {
		
		std::cout<<std::bitset<sizeof(v_)* CHAR_BIT>(v_) << "\n";
	}
	Bits() {}
	Bits(const T& value):
		v_(value){}

private:
	T v_;
};

#endif