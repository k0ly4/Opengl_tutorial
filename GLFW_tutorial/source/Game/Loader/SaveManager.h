#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include "Game/Voxels/Chunk.h"

#define REGION_SIZE 24
#define REGION_VEC	glm::uvec2(REGION_SIZE,REGION_SIZE)
#define REGION_IVEC glm::ivec2(REGION_SIZE,REGION_SIZE)
#define REGION_VOLUME REGION_SIZE*REGION_SIZE
#define REGION_RADIUS REGION_SIZE / 2

/////ReaderChunk----------------------------------
class sFile {
public:

	static inline std::string pathRegion(const glm::uvec2& beg) {
		return DIRECTORY + std::to_string(beg.x) + "@" + std::to_string(beg.y) + EXTENSION;
	}

	static inline std::string pathRegion(size_t beg_x,size_t beg_y) {
		return DIRECTORY + std::to_string(beg_x) + "@" + std::to_string(beg_y) + EXTENSION;
	}

private:
	
	sFile() {}
	~sFile() {}

	static const std::string DIRECTORY;
	static const std::string EXTENSION;

};

////VoxelRLE--------------------------
struct VoxelRLE {

	twobyte amount;
	extendedID e;

	VoxelRLE(twobyte amount_, extendedID id_)
		: e(id_),
		amount(amount_) {}
	VoxelRLE(twobyte amount_, Voxel id_)
		: e(id_.e),
		amount(amount_) {}
	VoxelRLE() {}

	static inline constexpr twobyte getMaxAmount() { return 32767 * 2; }
};

////ChunkRLE--------------------------
struct ChunkRLE {
	inline bool empty() const { return v_.empty(); }
	inline size_t sizeInByte()const { return (v_.size() * sizeof(VoxelRLE)); }
	std::vector<VoxelRLE> v_;
};

////RegionRLE--------------------------
struct RegionRLE {
	inline bool empty(size_t index)const { return c_[index].empty(); }
	ChunkRLE c_[REGION_VOLUME];
};

////RLE--------------------------------------------------
class RLE {
public:

	static bool uncompress(std::vector<Voxel>& voxels, const std::vector<VoxelRLE>& buffer);
	static void compress(ChunkRLE& chunk, const std::vector<Voxel>& data);
	constexpr static inline size_t beginChunkBlock() { return sizeof(size_t[REGION_VOLUME]); }

private:
	RLE() {}
	~RLE() {}
};

////WriterChunk-------------------------------------------
class WriterRLE :public WriterBin {

public:
	WriterRLE(const std::string& path) :WriterBin(path) {}
	inline void writeVoxels(const std::vector<Voxel>& data) { process(data); }

	inline void writeChunk(const ChunkRLE& chunk) {
		//for(size_t i=0; i < chunk.v_.size(); i++){
		//	write(chunk.v_[i]);
		//}
		write(chunk.v_[0], chunk.sizeInByte());
	}

	inline void writeRegion(const RegionRLE& region) {
		size_t pos_buffer[REGION_VOLUME];
		for (size_t i = 0; i < REGION_VOLUME; i++) {

			if (region.empty(i)) {
				pos_buffer[i] = i == 0 ? RLE::beginChunkBlock() : pos_buffer[i - 1];
				continue;
			}

			writeChunk(region.c_[i]);
			pos_buffer[i] = getPos();

		}
		write(0,pos_buffer);
	}

	inline void fillNull() {
		size_t pos_buffer[REGION_VOLUME];
		for (size_t i = 0; i < REGION_VOLUME; i++)pos_buffer[i] = RLE::beginChunkBlock();
		write(0,pos_buffer);
	}

private:

	void process(const std::vector<Voxel>& data);

};

/////ReaderRLE----------------------------------
class ReaderRLE :public ReaderBin{
public:
	ReaderRLE() {}
	ReaderRLE(const std::string& path):ReaderBin(path) {}
	//x,z - global world coord 
	inline void openForChunk(size_t x, size_t z) { open(sFile::pathRegion(x / REGION_SIZE, z / REGION_SIZE)); }

	template<typename CH>
	inline bool readChunk(CH& chunk, size_t index) {
		size_t begin, end;
		getInterval(index, begin, end);
		return readChunk(chunk, begin, end);
	}

	bool readChunk(Chunk& chunk, size_t begin, size_t end);

	bool readChunk(ChunkRLE& chunk, size_t begin, size_t end);

	inline void getInterval(size_t index,size_t& begin,size_t& end) {
		//begin
		if (index == 0) begin = RLE::beginChunkBlock();
		else  readFromPos((index-1) * sizeof(size_t), begin);
		//end
		readFromPos(index * sizeof(size_t), end);
	}
private:

};
#endif

