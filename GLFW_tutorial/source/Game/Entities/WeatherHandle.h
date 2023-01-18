#ifndef WEATHER_HANDLE_H
#define WEATHER_HANDLE_H
#include "Game/Lib/GLFW.h"
#include "Math/Math.h"
class WeatherHandle
{
	glm::vec3 f = {0.3f,0.5f,1.f};
public:
	WeatherHandle() {
		sunFactor = 0.f;
		tDaySeconds = 10;
		time = 0;
	}
	inline Color colorSky() { return {sunFactor* f.r,sunFactor * f.g,sunFactor * f.b };}
	inline void update(float delta) {
		sunFactor =(sinf((float)time * 0.01f)+1.f)*0.5f;
		time += delta;
	}
	inline float getSunFactor() {return sunFactor;}

private:

	float time;
	float sunFactor;
	size_t tDaySeconds;
};
#endif

