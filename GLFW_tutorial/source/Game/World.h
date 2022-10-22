#ifndef WORLD_H
#define WORLD_H
#include"Game/Chunk.h"

class World {
public:

    World() {
        chunks.create(glm::uvec3(6,1,6));
        atlas.load("asset\\image\\voxel_atlas.png", 32);       
        chunks.setAtlas(atlas);
    }
    //Generation
   
    ChunkHandle chunks;
    VoxelAtlas atlas;
};
#endif
