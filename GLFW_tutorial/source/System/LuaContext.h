#ifndef LUA_CONTEXT_H
#define LUA_CONTEXT_H
#include "LuaFIle.h"
#include "Math\Math.h"
namespace luke {
	class sLua
	{
	};
	
	
	inline glm::uvec3 toUvec3(LuaRef& ref) {
		if (ref.isTable()) {
			return glm::uvec3(
				ref[1].cast<size_t>(),
				ref[2].cast<size_t>(),
				ref[3].cast<size_t>()
			);
		}
		return glm::uvec3(ref.cast<size_t>());
	}
	inline ColorU toColorU(const LuaRef& ref) {
		if (ref.isNil()) {
			return ColorU(255);
		}
		if (ref.isTable()) {
			if (ref.length() == 4) {
				return ColorU(
					ref[1].cast<byte>(),
					ref[2].cast<byte>(),
					ref[3].cast<byte>(),
					ref[4].cast<byte>()
				);
			}
		if (ref.length() ==3) {
				return ColorU(
					ref[1].cast<byte>(),
					ref[2].cast<byte>(),
					ref[3].cast<byte>()
				);
			}
		}
		return ColorU(ref.cast<byte>());
	}
}

#endif

