#ifndef MASTER_GENERATION
#define MASTER_GENERATION

#include "Game/Voxels/Generation/BiomMap.h"
#include "Game/Voxels/Chunk.h"
#include "Math/Noise.h"
#include "System/Random.h"

/////Generator----------------------------------
class Generator {
public:
	virtual void generate(Chunk& chunk) = 0;
	virtual BiomMap& getBiomMap() = 0;
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
	inline static BiomMap& getBiomMap() { return generator->getBiomMap(); }

private:

	MasterGeneration() {}
	~MasterGeneration() {}

	static Generator* generator;
};

#include "System/LuaFIle.h"

/////CustomGenerator----------------------------------
class CustomGenerator :public Generator
{
	struct mBiom{
	public:
		Array2d< CHUNK_W, byte> map;
		float f_tmpr, f_humidiy;
		FastNoise noise;
	};

	struct mHeights {
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
		biomMap.create();
	}
	CustomGenerator(const char* path):CustomGenerator()
	{
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
	inline BiomMap& getBiomMap() {return biomMap; }
	
private:
	
	void initScript();
	void initBioms(luke::LuaRef generator);
	 void initTree(Voxels& voxels,glm::ivec3 pos) {
		size_t lenght = sQrand::get({ 6,9 });
		for (int y = lenght - 4; y < lenght; y++) {
			for (int z = -2; z <3; z++) {
				for (int x = -2; x < 3; x++) {
					glm::ivec3 p(pos + glm::ivec3(x, y, z));
					if (voxels.is(p)) voxels(p) = Voxel(vox::leaves_oak);
				}
			}
		}
		/*for (size_t size = 1,y = lenght+pos.y+1; size < 4; size++,y--) {
			for (size_t z = pos.z - size; z < (pos.z + size+1); z++) {
				for (size_t x = pos.x - size; x < (pos.x + size+1); x++) {
					if(voxels.is(x,y,z)) voxels(x, y, z) = Voxel(vox::leaves_oak);
				}
			}
		}*/
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
	mBiom		m_biom;
	mHeights	m_heights;
	BiomMap biomMap;
	
};

#endif
