#ifndef WORLD_H
#define WORLD_H
#include"Game/ChunkHandle.h"

class World {
public:

    World() {
        chunks.create(glm::uvec3(4,4,4));
        atlas.load("asset\\image\\voxel_atlas.png", 32); 
        
        chunks.setAtlas(atlas);
    }
    void save();
    //Generation
   
    ChunkHandle chunks;
    VoxelAtlas atlas;
};
#endif
