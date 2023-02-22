#include "World.h"



World::World() :chunks(&region),light(&region) {
    VoxPack::set(&resPack);
    if (resPack.load() == 0)exit(0);
    cProcess::queue.pushObserver(&light);
    cProcess::queue.init(&chunks, &region);
    chunks.pushObserver(&light);

    //chunks.pushObserver(&light);
   
}

void World::save() {
	region.save();
}