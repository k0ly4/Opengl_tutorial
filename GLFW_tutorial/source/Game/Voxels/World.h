#ifndef WORLD_H
#define WORLD_H

#include"Game/Voxels/ChunkHandle.h"
#include "Game/Light/LightSystem.h"

class World {
public:

    World();
    void save();
    inline void init(Camera& camera) {
        chunks.create(sSetup::distance_render, camera.getPosition());
        chunks.weather = &weather;
    }
    WeatherHandle weather;
    //Generation
    LightHandle light;
    ChunkSectorRender chunks;
    ResourcePack resPack;
    SupReg region;
};
#endif
