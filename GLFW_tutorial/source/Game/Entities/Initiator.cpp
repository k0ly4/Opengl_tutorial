#include "Initiator.h"
#include "Game/Entities/Player.h"
#include "Game/Entities/WeatherHandle.h"


void Initiator::init(Player& player) {
	using namespace luke;
	LUA_TRY
		script.openB((directory + names[i_player]).c_str());
		LuaRef config = script.get("player_config");
		//physics paramaters
		LuaRef phys = config["physics"];
		player.input.maxSpeed =		phys["maxSpeed"].cast<float>();
		player.input.jumpImpulse =	phys["jumpImpulse"].cast<float>();
		phys["setHitbox"](player.hitbox.get());
		//general
		player.input.cursor.maxDistanceCursor = config["maxDistanceCursor"].cast<float>();
		

	LUA_CATCH
}
void Initiator::init(WeatherHandle& weather) {
	using namespace luke;
	LUA_TRY
		script.openB((directory + names[i_weather]).c_str());
	LuaRef config = script.get("weather_config");
	//physics paramaters
	LuaRef lua_fog = config["fog"];

	weather.fogShader.density =		lua_fog["density"];
	weather.fogShader.gradient =	lua_fog["gradient"];
	weather.timeFactor = config["timeFactor"];

	LUA_CATCH
}
Initiator sInitiator::initiator;