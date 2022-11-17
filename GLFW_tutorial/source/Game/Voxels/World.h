#ifndef WORLD_H
#define WORLD_H

#include"Game/Voxels/ChunkHandle.h"
#include "Game/Light/LightSystem.h"

class World {
public:

    World();
    void save();
    inline void init(Camera& camera) {
        chunks.update(camera.getPosition());
        chunks.create(sGameSetup::distance_render);
    }
    //Generation
    LightHandle light;
    ChunkSectorRender chunks;
    ResourceVoxelPack atlas;
};
#endif
