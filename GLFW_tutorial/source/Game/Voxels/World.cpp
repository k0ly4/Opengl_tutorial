#include "World.h"



World::World() :chunks(&region),light(&chunks) {
    VoxelPack::set(&atlas);
    if (atlas.load("asset\\image\\") == 0)exit(0);
    cProcess::queue.pushObserver(&light);
    //chunks.pushObserver(&light);
   
}

void World::save() {
	region.save();
}