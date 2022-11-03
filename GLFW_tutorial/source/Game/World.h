#ifndef WORLD_H
#define WORLD_H
#include"Game/ChunkHandle.h"
#include "Game/LightSystem.h"
class World {
public:

    World():light(&chunks) {
        chunks.create(glm::uvec3(4,4,4));
        if (atlas.load("asset\\image\\")==0)exit(0);
        chunks.setAtlas(atlas);
        light.init();
    }
    void save();
    //Generation
    LightHandle light;
    ChunkHandle chunks;
    VoxelAtlas atlas;
};
#endif
