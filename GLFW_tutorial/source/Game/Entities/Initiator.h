#ifndef INITIATOR_H
#define INITIATOR_H
#include "System/LuaContext.h"

class Player;
class WeatherHandle;
class Initiator
{
public:
	enum EnumPath :size_t
	{
		i_generator,i_physics,i_player,i_weather,i_size
	};
	std::string directory = "scripts\\";
	const char* names[4] = {
		"i_generator.lua",
		"i_physics.lua",
		"i_player.lua",
		"i_weather.lua",
	};

	void init(Player& entity);
	void init(WeatherHandle& entity);

private:
	luke::LuaInterface script;
};

class sInitiator {
public:
	static inline void init(Player& entity) { initiator.init(entity); }
	static inline void init(WeatherHandle& entity) { initiator.init(entity); }

	static Initiator initiator;
private:

	sInitiator() {}
	~sInitiator() {}
};
#endif
