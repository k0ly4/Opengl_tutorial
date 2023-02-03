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

	inline int rand()noexcept {
		seed = (8253729 * seed + 2396403);
		return seed % 32768;
	}

	inline void setSeed(size_t number)noexcept {
		seed = number + 8253729;
		rand();
	}

private:

	size_t seed;

};
class sQrand {
public:

	inline static int get()noexcept							{ return random.rand(); }
	inline static int get(int max)noexcept					{ return (random.rand()%max); }
	inline static int get(const Range<int>& range)noexcept	{ return ((random.rand()+ range.min) % range.max); }

private:

	sQrand() {}
	~sQrand() {}
	static qRandom random;
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

	inline float get(const glm::vec3& pos)noexcept {						return noise.GetNoise(pos.x,pos.y,pos.z);}
	//[-1,1]
	inline float get(const glm::vec2& pos)noexcept {						return noise.GetNoise(pos.x, pos.y);}
	//[0,1]
	inline float getNormalize(const glm::vec2& pos)noexcept {				return (noise.GetNoise(pos.x, pos.y) + 1.f) * 0.5f;}
	inline float getNormalizeFactor(float x,float y,float factor)noexcept {	return (noise.GetNoise(x * factor, y * factor) + 1.f) * 0.5f;}
	/// 2D noise at given position using current settings
	inline float get(const glm::uvec2& pos)noexcept {						return noise.GetNoise((float)pos.x, (float)pos.y);}
	inline float get(const glm::uvec3& pos)noexcept {						return noise.GetNoise((float)pos.x, (float)pos.y, (float)pos.z);}
	inline float get(float x,float y,float z)noexcept {						return noise.GetNoise(x, y, z);}
	
	inline void setSeed(int seed) {								noise.SetSeed(seed);}
	inline void setTypeNoise(FastNoiseLite::NoiseType type) {	noise.SetNoiseType(type);}

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
	struct BiomMap{
	public:
		Array2d< CHUNK_W, byte> map;
		float f_tmpr, f_humidiy;
		FastNoise noise;
	};
	struct HeightsMap {
		public:
			Array2d< CHUNK_W, size_t> map;
			float f_sub, f_scale;
			FastNoise noise;
	};
	struct GreenaryMap {
	public:
		Array2d< CHUNK_W, byte> map;
		inline void clear() { map.fill(0);}
		
		inline void fillCircle(size_t index) {
			if (set(index - 1)) {
				if (set(index - 2)) 
					if (set(index - CHUNK_D)) set(index - 2 * CHUNK_D);
			}
			if (set(index + 1)) {
				if (set(index + 2))
					if (set(index + CHUNK_D)) set(index + 2 * CHUNK_D);
			}
			if (set(index - CHUNK_D - 1)) set(index - CHUNK_D + 1);
			if (set(index + CHUNK_D - 1)) set(index + CHUNK_D + 1);
		}
	private: 
		inline bool set(size_t index) {
			if (map.is(index)) map[index] = 2;
			else return 0;
			return 1;
		}
	};
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
		script.openB(path);
		initScript();

	}
	void generate(Chunk& chunk);

	static inline void addChannelHeight(float scale, float influence) { pars.push_back({ scale, influence }); }
	inline byte getBiom(float x, float z) {
		byte temperature = (byte)	(m_biom.noise.getNormalize(glm::vec2(x, z) *	m_biom.f_tmpr) * 255.f);
		byte humidity = (byte)(		m_biom.noise.getNormalize(glm::vec2(x, z) *		m_biom.f_humidiy) * 255.f);
		for (byte i = 0; i < vox::Biom::size; i++) {
			if (bioms[i].r_tmpr.is(temperature)) return i;
		}
		return vox::Biom::plain;
	}
private:
	
	void initScript();
	void initBioms(luke::LuaRef generator);
	inline void initTree(Voxels& voxels,glm::ivec3 pos) {
		size_t lenght = sQrand::get({ 6,9 });
		for (size_t size = 1,y = lenght+pos.y+1; size < 4; size++,y--) {
			for (size_t z = pos.z - size; z <= pos.z + size; z++) {
				for (size_t x = pos.x - size; x <= pos.x + size; x++) {
					if(voxels.is(x,y,z)) voxels(x, y, z) = Voxel(vox::earth);
				}
			}
		}
		voxels(pos) = Voxel(vox::oak);
		for (size_t i = 0; i < lenght; i++) {
			pos.y += 1;
			voxels(pos) = Voxel(vox::oak);
		}
	}
	size_t calcHeight(const glm::vec2& real);

	static std::vector<Par> pars;
	luke::LuaInterface script;
	vox::Biom bioms[vox::Biom::size];

	GreenaryMap m_green;
	BiomMap		m_biom;
	HeightsMap	m_heights;
	
};

#endif
