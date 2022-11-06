#ifndef WORLD_H
#define WORLD_H
#include"Game/ChunkHandle.h"
#include "Game/LightSystem.h"
class World {
public:

    World();
    void save();
    //Generation
    LightHandle light;
    ChunkHandle chunks;
    ResourceVoxelPack atlas;
};
#endif
