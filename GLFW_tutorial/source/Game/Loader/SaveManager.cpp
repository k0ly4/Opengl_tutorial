#include "SaveManager.h"


///sSaveF-----------------------------------------------
const std::string sFile::DIRECTORY = "saves\\";
const std::string sFile::EXTENSION = ".region";


///RLE-----------------------------------------------
void RLE::compress(ChunkRLE& chunk, const std::vector<Voxel>& data) {
	twobyte amount = 1;
	size_t j = 0;
	for (size_t i = 1; i < data.size(); i++) {
		if (data[i] == data[j] && amount < VoxelRLE::getMaxAmount()) amount++;
		else {
			chunk.v_.push_back(VoxelRLE(amount, data[j]));
			amount = 1;
			j = i;
		}
	}
	chunk.v_.push_back(VoxelRLE(amount, data[j]));
}

bool RLE::uncompress(Voxels& voxels, const std::vector<VoxelRLE>& buffer) {
	size_t voxI = 0;
	for (size_t i = 0; i < buffer.size(); i++)
		for (size_t j = 0; j < buffer[i].amount; j++, voxI++)
			voxels[voxI].e = buffer[i].e;

	if (voxI != CHUNK_VOL) {
		LOG("!!!!voxI ==%d\n", voxI);
		return 0;
	}
	return 1;
}



///WriterRLE-----------------------------------------------
void WriterRLE::process(const Voxels& data) {
	twobyte amount = 1;
	size_t j = 0;
	for (size_t i = 1; i < data.size(); i++) {
		if (data[i] == data[j] && amount < VoxelRLE::getMaxAmount()) amount++;
		else {
			write(VoxelRLE(amount, data[j]));
			amount = 1;
			j = i;
		}
	}
	write(VoxelRLE(amount, data[j]));
}



///ReaderRLE-----------------------------------------------
bool ReaderRLE::readChunk(Chunk& chunk, size_t begin, size_t end) {
	ChunkRLE buffer;
	if (readChunk(buffer, begin, end) == 0) {
		chunk.flag.clear();
		return 0;
	}
	if (RLE::uncompress(chunk.voxels(), buffer.v_) == 0)LOG("begin =%d,end=%d\n", begin, end);
	//Set flags
	chunk.flag.toLightLevel();
	return 1;
}

bool ReaderRLE::readChunk(ChunkRLE& chunk, size_t begin, size_t end) {
	size_t sizeInByte = end - begin;
	if (sizeInByte == 0) return 0;
	chunk.v_.resize(sizeInByte / sizeof(VoxelRLE));
	readFromPos(begin, chunk.v_[0], sizeInByte);
	return 1;
}
