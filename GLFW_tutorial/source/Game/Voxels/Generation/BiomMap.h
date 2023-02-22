#ifndef BIOM_MAP_H
#define BIOM_MAP_H

#include "System/Entities.h"
#include "Graphic/TextureMap.h"
#include "UI/Sprite.h"

class BiomMap {

public:

	BiomMap() {

		bMap.setFilter(tFilter::Nearest);
		bMap.setWrap(tWrap2D::ClampToEdge);
		tMap.setFilter(tFilter::Nearest);
		tMap.setWrap(tWrap2D::ClampToEdge);
		initConstants();

	}
	void create()noexcept;

	inline Sprite& getSprite(float x, float y) {
		bSolveModified();
		bsprite.transform.setPosition(x, y);
		return bsprite;
	}

private:

	inline void tMapToPNG(const std::string& path) {
		tMap.modified = 1;
		tSolveModified();
		tMap.exportToPNG(path.c_str());
	}
	inline void bMapToPNG(const std::string& path) {
		bMap.modified = 1;
		bSolveModified();
		bMap.exportToPNG(path.c_str());
	}

	inline void bSolveModified() {
		if (bMap.modified) {
			bConvert();
			bsprite.setTexture(bMap.texture());
			bsprite.setTextureRect(FloatRect(0, 0, 128, 128));
			bsprite.transform.setScale(5.f, 5.f);
		}
	}
	inline void tSolveModified() {
		if (tMap.modified) {
			tConvert();
			tsprite.setTexture(tMap.texture());
			tsprite.setTextureRect(FloatRect(0, 0, 128, 128));
			tsprite.transform.setScale(5.f, 5.f);
		}
	}

	inline byte randTempr() {
		byte r = rand() % 6;
		return r == 0 ? freeze : r == 1 ? hot : r > 3 ? cool : warm;
	}

	void smoothTempr();
	void tZoom();
	void bZoom();
	void addIslands();

	void markDepthOceans();

	inline void tConvert() {
		tMap.create(tarr.v_size());
		for (size_t i = 0; i < tarr.size(); i++) tMap.map[i] = tColDraw[tarr[i]];
	}
	inline void bConvert() {
		bMap.create(barr.v_size());
		for (size_t i = 0; i < barr.size(); i++) bMap.map[i] = bColDraw[barr[i]];
	}
	//inits
	inline void initTempr() {
		tMap.modified = 1;
		tarr.setSize(barr.v_size());
		for (size_t i = 0; i < tarr.size(); i++) {
			tarr[i] = randTempr();
		}
	}
	inline void initBioms() {
		bMap.modified = 1;
		barr.setSize({ 32,32 });
		for (size_t i = 0; i < barr.size(); i++) {
			barr[i] = ((rand() % 3) == 0) ? sea : land;
		}
	}
	void initConstants() {
		bColDraw[sea] = ColorU(0, 102, 255);
		bColDraw[land] = ColorU(255, 183, 0);
		bColDraw[deepSea] = ColorU(30, 0, 227);


		tColDraw[freeze] = { 0, 0, 255 };
		tColDraw[cool] = { 0, 180, 255 };
		tColDraw[warm] = { 255, 200, 0 };
		tColDraw[hot] = { 255, 0, 0 };
	}
	inline bool isSea(byte id) { return id <= sea; }

	enum bID :byte {
		deepSea,
		sea,
		land,
		bNun,
		bSize
	};

	enum tID :byte {
		freeze,
		cool,
		warm,
		hot,
		tSize

	};
	ColorU bColDraw[bSize];
	ColorU tColDraw[tSize];
	vector2d<byte> barr;
	vector2d<byte> tarr;

	Sprite tsprite;
	Sprite bsprite;

	TextureMap tMap;
	TextureMap bMap;
};
#endif 
