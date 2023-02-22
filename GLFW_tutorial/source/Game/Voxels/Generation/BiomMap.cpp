#include "BiomMap.h"

void BiomMap::create()noexcept {
	//bioms1
	initBioms();
	//bioms2
	bZoom();
	addIslands();
	//bioms3
	bZoom();
	addIslands();
	addIslands();
	markDepthOceans();
	//tempr1
	initTempr();
	smoothTempr();
	tMapToPNG("screenshots\\tMap1.png");
	//bioms4
	bZoom();
	addIslands();
	addIslands();
	markDepthOceans();
	//tempr2
	tZoom();
	tMapToPNG("screenshots\\tMap2.png");

	bMapToPNG("screenshots\\final.png");
	tMap.modified = 1;
	bMap.modified = 1;
}

void BiomMap::smoothTempr() {
	vector2d<byte> t(tarr.v_size());
	for (size_t y = 0; y < t.v_size().y; y++) {
		for (size_t x = 0; x < t.v_size().x; x++) {
			byte curTempr = tarr(x, y);

			if (curTempr == freeze) {
				if (

					tarr.get(x - 1, y, warm) == hot ||
					tarr.get(x, y - 1, warm) == hot ||
					tarr.get(x + 1, y, warm) == hot ||
					tarr.get(x, y + 1, warm) == hot) {
					t(x, y) = cool;
					continue;
				}
			}
			else if (curTempr == hot) {
				if (
					tarr.get(x - 1, y, warm) == freeze ||
					tarr.get(x, y - 1, warm) == freeze ||
					tarr.get(x + 1, y, warm) == freeze ||
					tarr.get(x, y + 1, warm) == freeze) {
					t(x, y) = warm;
					continue;
				}


			}
			t(x, y) = curTempr;

		}
	}
	tarr = t;
}

void BiomMap::tZoom() {

	vector2d<byte> t;
	t.setSize(tarr.v_size() * 2);
	size_t clos[4];
	for (size_t y = 0; y < tarr.v_size().y; y++) {
		for (size_t x = 0; x < tarr.v_size().x; x++) {

			clos[0] = tarr(x, y);
			clos[1] = tarr.get(x + 1, y, clos[0]);
			clos[2] = tarr.get(x, y + 1, clos[0]);
			clos[3] = tarr.get(x + 1, y + 1, clos[0]);

			glm::ivec2 r(x * 2, y * 2);
			t(r) = clos[0];
			t(r.x + 1, r.y) = (clos[1] + clos[0]) / 2;
			t(r.x, r.y + 1) = (clos[2] + clos[0]) / 2;
			t(r.x + 1, r.y + 1) = (clos[2] + clos[0] + clos[1] + clos[3]) / 4;
		}
	}
	tarr = t;
}

void BiomMap::bZoom() {

	vector2d<byte> world;
	world.setSize(barr.v_size() * 2);
	size_t clos[4];
	for (size_t y = 0; y < barr.v_size().y; y++) {
		for (size_t x = 0; x < barr.v_size().x; x++) {
			clos[0] = barr(x, y) > sea;
			clos[1] = barr.get(x + 1, y, clos[0]) > sea;
			clos[2] = barr.get(x, y + 1, clos[0]) > sea;
			clos[3] = barr.get(x + 1, y + 1, clos[0]) > sea;

			world(x * 2, y * 2) = clos[0];
			world(x * 2 + 1, y * 2) = (clos[0] || clos[1]) ? (rand() % 2 == 0 ? land : sea) : sea;
			world(x * 2, y * 2 + 1) = (clos[0] || clos[2]) ? (rand() % 2 == 0 ? land : sea) : sea;
			world(x * 2 + 1, y * 2 + 1) = ((clos[0] + clos[1] + clos[2] + clos[3]) / 4.f >= 0.5f) ? (rand() % 2 == 0 ? land : sea) : sea;
		}
	}

	barr = world;
}

void BiomMap::addIslands() {
	for (size_t i = 0; i < barr.size(); i++) {
		if (barr[i] == sea && (rand() % 10) == 0) {
			barr[i] = land;
		}
	}
}

void BiomMap::markDepthOceans() {
	for (size_t y = 0; y < barr.v_size().y; y++) {
		for (size_t x = 0; x < barr.v_size().x; x++) {
			if (isSea(barr.get(x - 1, y, sea)) &&
				isSea(barr.get(x + 1, y, sea)) &&
				isSea(barr.get(x, y - 1, sea)) &&
				isSea(barr.get(x, y + 1, sea)) &&
				(rand() % 2 == 0)
				)barr(x, y) = deepSea;
		}
	}

}