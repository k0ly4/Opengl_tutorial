#include "World.h"



World::World() :light(&chunks) {
    VoxelPack::set(&atlas);
    if (atlas.load("asset\\image\\") == 0)exit(0);
    chunks.pushObserver(&light);
   
}

void World::save() {
	chunks.save();
}