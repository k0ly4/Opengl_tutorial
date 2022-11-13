#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H
#include "Game/Chunk.h"


#define REGION_SIZE 12
#define REGION_VOLUME REGION_SIZE*REGION_SIZE
#define REGION_RADIUS REGION_SIZE / 2

/////ReaderChunk----------------------------------
class sSaveF {
public:

	static inline std::string pathChunk(const glm::uvec2& beg) {
		return DIRECTORY + std::to_string(beg.x) + "@" + std::to_string(beg.y) + EXTENSION;
	}

	static inline std::string pathChunk(size_t beg_x,size_t beg_y) {
		return DIRECTORY + std::to_string(beg_x) + "@" + std::to_string(beg_y) + EXTENSION;
	}

private:
	
	sSaveF() {}
	~sSaveF() {}

	static const std::string DIRECTORY;
	static const std::string EXTENSION;

};

///////ReaderChunk----------------------------------
//class ReaderChunk :public ReaderFile {
//public:
//	//x,z - global world coord 
//	inline void openForChunk(size_t x, size_t z) { open(sSaveF::pathChunk(x / REGION_SIZE, z / REGION_SIZE)); }
//
//	inline void readChunk(Chunk& data) {
//		read(data.isGenerated);
//		if (data.isGenerated) readVoxels(data.getVoxels(), CHUNK_SIZE);
//	}
//
//	inline void readChunk(Chunk& data, size_t chunkIndexInFile) {
//		set(chunkIndexInFile * ( (size_t)CHUNK_SIZE * sizeof(Voxel) + sizeof(bool)));
//		readChunk(data);
//	}
//
//	inline void readVoxels(std::vector<Voxel>& data, size_t size) {
//		data.resize(size);
//		for (size_t i = 0; i < size; i++) read(data[i].id);
//	}
//
//private:
//
//};
//
//////WriterChunk--------------------------
//class WriterChunk :public Writer {
//
//public:
//	//Структура записи
//	//bool - isGenerated
//	//std::vector<Voxel> - id каждого вокселя(int)
//	inline void writeChunk(const Chunk& data) {
//		write(data.isGenerated);
//		writeVoxels(data.getVoxels());
//	}
//
//	inline void writeVoxels(const std::vector<Voxel>& data) {
//		for (size_t i = 0; i < data.size(); i++) write(data[i].id);
//	}
//
//private:
//
//};

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

////WriterChunk--------------------------
class WriterRLE :public Writer {

public:

	inline void writeVoxels(const std::vector<Voxel>& data) { process(data); }

private:

	void process(const std::vector<Voxel>& data);

};


/////ReaderRLE----------------------------------
class ReaderRLE :public ReaderFile {
public:
	//x,z - global world coord 
	inline void openForChunk(size_t x, size_t z) { open(sSaveF::pathChunk(x / REGION_SIZE, z / REGION_SIZE)); }
	
	void readChunk(Chunk& chunk, size_t begin, size_t end);

	inline void readChunk(Chunk& chunk, size_t index) {
		size_t begin, end;
		getInterval(index, begin, end);
		readChunk(chunk, begin, end);
	}

	inline void getInterval(size_t index,size_t& begin,size_t& end) {
		//begin
		if (index == 0) begin = ReaderRLE::beginChunkBlock();
		else  readFromPos((index-1) * sizeof(size_t), begin);
		//end
		readFromPos(index * sizeof(size_t), end);
	}

	//get size of pos block
	constexpr static inline size_t beginChunkBlock() { return sizeof(size_t[REGION_VOLUME]); }

private:

	void convert(std::vector<Voxel>& voxels, std::vector<VoxelRLE>& buffer, size_t size);

};
#endif

