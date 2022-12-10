#ifndef MASTER_GENERATION
#define MASTER_GENERATION

#include "Chunk.h"
#include "FastNoiseLite.h"


/////qRandom----------------------------------
class qRandom {

public:

	qRandom() {
		seed = (unsigned)time(0);
	}

	inline int rand() {
		seed = (8253729 * seed + 2396403);
		return seed % 32768;
	}

	inline void setSeed(size_t number) {
		seed = number + 8253729;
		rand();
	}

private:

	size_t seed;

};


/////FastNoise----------------------------------
class FastNoise {

public:
	FastNoise() {
		//NoiseType_Perlin              =1358
		//NoiseType_Cellular            =3682 =3584 =3485
		//NoiseType_OpenSimplex2        =779
		//NoiseType_OpenSimplex2S       =928
		//NoiseType_Value				=1000 1283 =993
		//NoiseType_OpenSimplex2S       =5034  =5164
		setTypeNoise(FastNoiseLite::NoiseType_OpenSimplex2);
	}

	inline float get(const glm::vec3& pos) {return noise.GetNoise(pos.x,pos.y,pos.z);}
	//[-1,1]
	inline float get(const glm::vec2& pos) {return noise.GetNoise(pos.x, pos.y);}
	//[0,1]
	inline float getNormalize(const glm::vec2& pos) { return (noise.GetNoise(pos.x, pos.y) + 1.f) * 0.5f;}
	inline float getNormalizeFactor(float x,float y,float factor) {return (noise.GetNoise(x * factor, y * factor) + 1.f) * 0.5f;}
	/// 2D noise at given position using current settings
	inline float get(const glm::uvec2& pos) { return noise.GetNoise((float)pos.x, (float)pos.y);}
	inline float get(const glm::uvec3& pos) { return noise.GetNoise((float)pos.x, (float)pos.y, (float)pos.z);}
	inline float get(float x,float y,float z){return noise.GetNoise(x, y, z);}
	
	inline void setSeed(int seed) {noise.SetSeed(seed);}
	inline void setTypeNoise(FastNoiseLite::NoiseType type) {noise.SetNoiseType(type);}
private:
	FastNoiseLite noise;
};

/////Generator----------------------------------
class Generator {
public:
	virtual void generate(Chunk& chunk) = 0;
};

/////MasterGaneration----------------------------------
class MasterGeneration
{
public:

	inline static void generateChunk(Chunk& chunk) { generator->generate(chunk); }
	inline static void setGenerator(Generator* generator_) {
		if (generator)	delete generator;
		generator = generator_;
	}

private:

	MasterGeneration() {}
	~MasterGeneration() {}

	static Generator* generator;
};

/////DefaultGenerator----------------------------------
class DefaultGenerator:public Generator
{
public:

	void generate(Chunk& chunk);

private:

	size_t calcHeight(const glm::vec2& real);
	FastNoise noise;

};

#include "System/LuaFIle.h"
/////CustomGenerator----------------------------------
class CustomGenerator :public Generator
{
public:

	struct Par {
		float scale;
		float influence;
	};

	CustomGenerator() {
		pars.clear();
	}
	CustomGenerator(const char* path) {
		pars.clear();
		create(path);
	}
	inline void create(const char* path) {

		integrateScript();
		script.open(path);
		initScript();

	}
	void generate(Chunk& chunk);

private:
	
	static inline void addChannelHeight(float scale, float influence) { pars.push_back({ scale, influence}); }
	 inline Biom::Type getBiom(size_t x,size_t z) {
		 int temperature = noise.getNormalize(glm::vec2(x, z)*2.f)*255.f;
		 
		 if (temperature < 50.f)return Biom::tundra;
		 if (temperature < 200.f)return Biom::plain;
		 return Biom::desert;
	}
	
	void integrateScript();
	void initScript();

	size_t calcHeight(const glm::vec2& real);

	float multFactorHeight;
	float subFactorHeight;

	static std::vector<Par> pars;
	luascr::LuaInterface script;
	FastNoise noise;

};

#endif
