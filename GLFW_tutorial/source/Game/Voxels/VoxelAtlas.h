#ifndef VOXEL_ATLAS_H
#define VOXEL_ATLAS_H

#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "System/Exception.h"
#include "Resource/Shader.h"
#include "UI/Sprite.h"
#include "Scene/Convex.h"
#include "System/LuaFIle.h"
#include "Declare.h"
namespace Side2D {

	enum eSide2D :byte
	{
		left,
		right,
		bottom,
		top,
		//corner
		left_bottom,
		right_bottom,
		left_top,
		right_top,

		NuN,
	};
	inline eSide2D opposite(eSide2D side) {
		if (side == right)	return left;
		if (side == left)	return right;
		if (side == top)	return bottom;
		return top;
	}
	inline eSide2D getCornerNorm(bool x,bool y) {
		if (x) return	y ? right_top : right_bottom;
		else return		y ? left_top : left_bottom;
	}

	inline eSide2D getMainSide(int x, int y) {
		if (x > 0)return right;
		if (x < 0)return left;
		if (y < 0)return bottom;
		return top;
	}
	//normalize from [0;maxX] to [-1;1] 
	inline int normalizeU(size_t x, size_t maxX) {
		return x == maxX ? 1 : x == 0 ? -1 : 0;
	}
	//normalize from [-infinity; +infinity] to [-1;1]
	inline int normalizeI(int coord_x, int maxX) {
		return (coord_x > maxX) ? 1 : (coord_x < 0) ? -1 : 0;
	}

	inline eSide2D toSide(int norm_x, int norm_y) {
			if ((bool)norm_x == (bool)norm_y) return getCornerNorm((norm_x + 1), (norm_y + 1));
			return getMainSide(norm_x, norm_y);
	}

	inline eSide2D toSideU(size_t x, size_t y, size_t max) {	return toSide(normalizeU(x, max), normalizeU(y, max));}

	inline eSide2D toSideI(int x, int y, size_t max)		{	return toSide(normalizeI(x, max), normalizeI(y, max));}
}
namespace Side {
	enum eSide:size_t
	{
		left,
		right,
		bottom,
		top,
		back,
		front,
	};

}

///VoxelType----------------------------------------------
/// <summary>
/// 
/// </summary>
struct Block {

	std::string name;
	//uv wrap
	union 
	{
		struct { size_t idSide[6]; };
		/// if liquid
		struct { size_t idIdleSide, idActSide;};
	};
	
	//emission
	Uint8RGB emission;
	//Draw method
	byte drawGroup;
	//physiccs
	byte physGroup;
	//is emission
	bool isEmission;

	inline void setSide(luke::LuaRef ref) {
		if (ref.isTable()) {
			for (size_t i = 0; i < 6; i++) {
				idSide[i] = ref[i + 1];
			}
		}
		else {
			size_t side_ = ref;
			for (size_t i = 0; i < 6; i++) idSide[i] = side_;
		}
	}
	inline void setEmission(luke::LuaRef ref) {
		if (ref.isTable()) {
			emission.r = ref[1];
			emission.g = ref[2];
			emission.b = ref[3];
		}
		else emission.r = emission.g = emission.b = ref;
		setEmissionFlag();
	}
	inline void setEmissionFlag() {isEmission = emission.r || emission.g || emission.b;}
	
};
struct TexturePackMetadata {
	std::string name, version;
};
struct TexAtlas {
	//кол-во элементов
	size_t size_el;
	// кол-во элементов в ряду
	size_t size_row_el;
	// размер элемента в пикселях
	float el_px;
		// размер элемента в шейдере
	glm::vec2 el_uv;
	// размер текстуры в пикселях
	glm::ivec2 size_px;
	std::vector<glm::vec2> uv;
	std::shared_ptr<Texture2D> tex;
	inline void qCreateUV() {
		size_t size_el_anim = 2;
		size_t offset = size_el_anim * size_row_el;
		uv.resize(size_el);
		size_t index =0;
		for (size_t i = 0; i < size_el- size_el_anim; i++) {
			uv[index++] = toUV(i+ offset);
		}
		for (size_t i = 0; i < size_el_anim; i++) {
			uv[index++] = toUV(i*size_row_el);
		}
	}
	inline glm::vec2 toUV(size_t id) {
		float u = (id % size_row_el) * el_uv.x;
		float v = 1.f - ((1.f + id / size_row_el) * el_uv.y);
		return glm::vec2(u, v);
	}
	inline glm::uvec2 coord(size_t id)const noexcept { return toCoord2(id, size_row_el);}
};

class TexturePack {
public:
	enum eDrawGroup :byte {
		draw_non = 0,
		draw_opaque,
		draw_transparent,
		draw_alpha,
	};

	enum ePhysics :byte {
		ph_gas = 0,
		ph_solid,
		ph_liquid,
	};
	TexturePack() {
		mesh.vertices.resize(4);
		mesh[0].v1.z = 0.0f;	mesh[0].v1.w =	1.f;
		mesh[1].v1.z = 1.f;		mesh[1].v1.w =	1.f;
		mesh[2].v1.z = 0.f;		mesh[2].v1.w =	0.f;
		mesh[3].v1.z = 1.f;		mesh[3].v1.w =	0.f;
		
	}
	const Block& get(Voxel id)const noexcept { return blocks[id.e.id]; }
	//path to json file
	bool load();
	inline const glm::vec2& get(Voxel id, int side)const noexcept { return isLiquid(id) ? getLiqiud(id) : atlas.uv[blocks[id.e.id].idSide[side]];}
	inline const glm::vec4& get4(Voxel id, int side)const noexcept { 
		glm::vec2 v1(get(id,side));
		return glm::vec4(v1, v1 + atlas.el_uv);
	}
	inline const glm::vec2& getLiqiud(Voxel id)const noexcept	  {	return atlas.uv[blocks[id.e.id].idIdleSide]; }

	inline void use(const Shader& shader)const {
		shader.uniform("configMaterial", 1);
		shader.uniform("baseColor", glm::vec3(1.f));
		render.texture()->use(0);
	}
	inline const Texture2D& getAtlas()const {		return *(render.texture()); }
	//path to json file
	bool load(const std::string& directory);
	inline const TexAtlas& data()const noexcept { return atlas; }

	inline Sprite& getSprite(Voxel id)const {
		glm::vec2 size = glm::vec2(render.texture()->getSize());
		glm::vec2 norm(get(id, Side::top));
		glm::vec2 pos(
			glm::vec2(norm.x, 1.f - norm.y -atlas.el_uv.y) * size
		);
		FloatRect rect(pos, glm::vec2((float)atlas.el_px));
		icon.setTextureRect(rect);
		return icon;
	}
	inline glm::vec2 getNormalizeSizeVoxel()const noexcept {	return atlas.el_uv; }
	inline size_t getSizeVoxel()const noexcept {				return atlas.el_px; }

	inline bool isLiquid(Voxel id)const					noexcept { return get(id).physGroup == TexturePack::ph_liquid; }
private:
	inline const glm::vec2& get(twobyte id, int side)const { return atlas.uv[blocks[id].idSide[side]]; }

	void renderTextureAtlas();
	inline void drawElRotate(glm::vec2 pos, glm::vec2 size) {
		mesh[2].v1.x = pos.x;				mesh[2].v1.y = pos.y + size.x;
		mesh[0].v1.x = pos.x + size.y,		mesh[0].v1.y = pos.y + size.x;
		mesh[3].v1.x = pos.x,				mesh[3].v1.y = pos.y;
		mesh[1].v1.x = pos.x + size.y,		mesh[1].v1.y = pos.y;
		mesh.saveInBuffer();
		mesh.drawArrays(Render::TRIANGLES_STRIP);
	}
	inline void drawEl(glm::vec2 pos,glm::vec2 size) {
		mesh[0].v1.x = pos.x;			mesh[0].v1.y = pos.y + size.y;
		mesh[1].v1.x = pos.x + size.x,	mesh[1].v1.y = pos.y + size.y;
		mesh[2].v1.x = pos.x,			mesh[2].v1.y = pos.y;
		mesh[3].v1.x = pos.x + size.x,	mesh[3].v1.y = pos.y;
		mesh.saveInBuffer();
		mesh.drawArrays(Render::TRIANGLES_STRIP);
	}
	inline void initScript() {
		const char* path = "asset\\image\\init.lua";
		integrateScript();
		script.openB(path);
	}
	inline void integrateScript() {}
	TexturePackMetadata metadata;
	qGeometry<Vertex1<glm::vec4>> mesh;
	std::vector<Block> blocks;

	TexAtlas atlas;
	//std::shared_ptr<Texture2D> icons_;
	RenderTexture render;
	//temp
	Texture2D res;
	luke::LuaInterface script;
	mutable Sprite icon;
};
///VoxelAtlas---------------------------
/// <summary>
/// 
/// </summary>
class ResourcePack {

public:

	enum eDrawGroup:byte {
		draw_non =0,
		draw_opaque,
		draw_transparent,
		draw_alpha,
	};

	enum ePhysics :byte {
		ph_gas = 0,
		ph_solid,
		ph_liquid,
	};
	const Block& get(Voxel id)const noexcept { return blocks[id.e.id];}
	//path to json file
	bool load(const std::string& directory);
	inline const glm::vec2& get(Voxel id, int side)const noexcept { return uv[blocks[id.e.id].idSide[side]];}

	inline float getNormalizeSizeVoxel()const noexcept {return uvSize_;}
	inline size_t getSizeVoxel()const noexcept { return sizeVoxel_; }

	inline void use(const Shader& shader)const {
		shader.uniform("configMaterial", 1);
		shader.uniform("baseColor", glm::vec3(1.f));
		texture_.use(0);
	}
	const Texture2D& getTexture()const {return texture_;}
	inline Sprite& getSprite(Voxel id)const {
		glm::vec2 size = glm::vec2(texture_.getSize());
		glm::vec2 norm(get(id, Side::top));
		glm::vec2 pos(
			glm::vec2(norm.x, 1.f - norm.y - uvSize_) * size
		);
		FloatRect rect(pos, glm::vec2((float)sizeVoxel_));
		icon.setTextureRect(rect);
		return icon;
	}
private:

	inline const glm::vec2& get(twobyte id, int side)const { return uv[blocks[id].idSide[side]];}
	inline glm::vec2 getUV(int id) {
		float u = (id % voxelInSide) * uvSize_;
		float v = 1.f - ((1.f + id / voxelInSide) * uvSize_);
		return glm::vec2(u, v);
	}

	std::vector<glm::vec2> uv;
	std::vector<Block> blocks;

	float uvSize_;
	size_t voxelInSide;
	size_t sizeVoxel_;
	Texture2D texture_;
	mutable Sprite icon;
};

///VoxelPack---------------------------
/// <summary>
/// 
/// </summary>
class VoxPack {
public:
	static inline		Sprite& getSprite(Voxel id)			noexcept { return res->getSprite(id); }

	static inline const Block& get(Voxel id)				noexcept { return res->get(id); }
	static inline const glm::vec2& get(Voxel id, byte side)	noexcept { return res->get(id, side);}
	//vec4 - границы
	static inline const glm::vec4& get4(Voxel id, byte side)noexcept { return res->get4(id, side); }
	static inline const TexturePack* get()					noexcept { return res; }
	//Light parr
	static inline bool isEmission(Voxel id)					noexcept { return  get(id).isEmission;}
	//Draw parr
	static inline bool isRender(Voxel id)					noexcept { return (get(id).drawGroup != TexturePack::draw_non); }
	static inline bool isSelectable(Voxel id)				noexcept { return (get(id).drawGroup != TexturePack::draw_non); }
	static inline bool isOpaque(Voxel id)					noexcept { return (get(id).drawGroup == TexturePack::draw_opaque);}
	static inline bool isAlpha(Voxel id)					noexcept { return (get(id).drawGroup == TexturePack::draw_alpha); }
	//Physics parr
	static inline bool isGas(Voxel id)						noexcept { return get(id).physGroup == TexturePack::ph_gas; }
	static inline bool isSolid(Voxel id)					noexcept { return get(id).physGroup == TexturePack::ph_solid;}
	static inline bool isLiquid(Voxel id)					noexcept { return get(id).physGroup == TexturePack::ph_liquid; }
	static inline bool isSourceLiquid(Voxel id)				noexcept { return id.e.m1 == maxConcLiquid; }
	//пока что только если жидкость
	static inline bool isActive(Voxel id)					noexcept { return get(id).physGroup == TexturePack::ph_liquid; }

	static inline void set(const TexturePack* resource)	noexcept { res = resource;}
	const static byte maxConcLiquid = 0x08u;
private:

	VoxPack() {}
	~VoxPack() {}
	static const TexturePack* res;

};
#endif