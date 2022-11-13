#include "SaveManager.h"
///sSaveF-----------------------------------------------
const std::string sSaveF::DIRECTORY = "saves\\";
const std::string sSaveF::EXTENSION = ".region";

///WriterRLE-----------------------------------------------
void WriterRLE::process(const std::vector<Voxel>& data) {
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

void ReaderRLE::readChunk(Chunk& chunk, size_t begin, size_t end) {
	size_t size = end - begin;
	if (size == 0) {
		chunk.isGenerated = 0;
		return;
	}
	set(begin);

	size = size / sizeof(VoxelRLE);
	std::vector<VoxelRLE>buffer(size);
	for (size_t i = 0; i < size; i++) {
		read(buffer[i]);
	}
	convert(chunk.getVoxels(), buffer, size);
	//Set flags
	chunk.setModified(); chunk.isInitLightMap = 0; chunk.isGenerated = 1;
}

void ReaderRLE::convert(std::vector<Voxel>& voxels, std::vector<VoxelRLE>& buffer, size_t size) {
	voxels.resize(CHUNK_SIZE);
	size_t voxI = 0;

	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < buffer[i].amount; j++, voxI++) {
			voxels[voxI].e = buffer[i].e;
		}
	}

	if (voxI != CHUNK_SIZE) LOG("!!!!voxI ==%d\n", voxI);
}