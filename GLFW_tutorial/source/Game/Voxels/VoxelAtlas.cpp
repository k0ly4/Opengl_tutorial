#include "VoxelAtlas.h"
#include "Declare.h"
#include "System/LuaContext.h"
struct TextureConfig {
	std::string path;
	ColorU color;
	byte frame;
	inline bool isAnim() { return frame > 1; }
	inline void init(luke::LuaRef ref) {
		frame = ref["frame"].isNil() ? 1 : ref["frame"];
		color = luke::toColorU(ref["color"]);
		path = ref["path"].cast<std::string>();
	}
	TextureConfig() {}
};
//VoxelAtlas-------------------------------------
/// <summary>
///// 
///// </summary>
void  TexturePack::renderMainAtlas(luke::LuaRef asset,const std::string& global_dir) {
	std::string dir = global_dir + asset["dir_block"].cast<std::string>();
	luke::LuaRef texBlocks = asset["tex_block"];
	a_main.el_px = asset["size_px"].cast<float>();
	a_main.size_el = texBlocks.length();
	//parse---------------------
	std::vector< TextureConfig> tex, a_tex;
	a_main.size_px = glm::ivec2(512);
	tex.reserve(a_main.size_el);
	for (size_t i = 1; i < a_main.size_el + 1; i++) {
		TextureConfig tData; 
		tData.init(texBlocks[i]);
		if (tData.isAnim()) {
			if (a_main.size_px.x < tData.frame * a_main.el_px)a_main.size_px.x = tData.frame * a_main.el_px;
			a_tex.push_back(tData);
		}
		else tex.push_back(tData);
	}
	
	a_main.size_row_el = a_main.size_px.x / (size_t)a_main.el_px;
	a_main.el_uv = glm::vec2(a_main.el_px) / glm::vec2(a_main.size_px);
	////contextRender------------
	a_main.tex = std::make_shared<Texture2D>(Texture2D(a_main.size_px, TextureFormatData( GL_RGBA,GL_RGBA ), 0, 0));
	a_main.tex->filter(tFilter::Nearest, tFilter::NearestMipmapNearest);
	render.bind(a_main.tex);

	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	Depth::Mask(false);
	CullFace::Enable(false);
	Blend::Enable(true);
	Render::bind(render);
	Render::setClearColor(0.f, 0.f, 0.f, 0.f);
	Render::clear(Render::COLOR_BIT);
	//render------------
	const Shader& shader = glShader::get(glShader::texture_2d);
	shader.use();
	shader.uniform("projection", glm::ortho(0.f, (float)a_main.size_px.x, (float)a_main.size_px.y, 0.f));
	size_t cell = 0;
	glm::vec2 pos;
	shader.uniform("model", glm::mat4(1.f));
	for (size_t i = 0; i < a_tex.size(); i++) {
		res.load_no_reg(dir + a_tex[i].path);
		res.use(0);
		shader.uniform("color", a_tex[i].color);
		pos = a_main.el_px * glm::vec2(a_main.coord(cell));
		cell += a_main.size_px.x/ a_main.el_px;
		drawElRotate(pos, glm::vec2(a_main.el_px, a_main.el_px * a_tex[i].frame));
	}
	for (size_t i = 0; i < tex.size(); i++) {
		res.load_no_reg(dir + tex[i].path);
		res.use(0);
		shader.uniform("color", tex[i].color);
		pos = a_main.el_px * glm::vec2(a_main.coord(cell));
		cell += tex[i].frame;
		drawEl(pos, glm::vec2(a_main.el_px));
	}
	Render::unbind();
	Depth::Mask(true);
	a_main.tex->resource()->setMipmaps(4);
}
void  TexturePack::renderEnvAtlas(luke::LuaRef asset, const std::string& global_dir) {
	std::string dir = global_dir + asset["dir_env"].cast<std::string>();
	luke::LuaRef texEnv = asset["tex_env"];
	
	sun.load(dir + texEnv[1]["path"].cast<std::string>());
}
bool TexturePack::load() {
	initScript();
	luke::LuaRef pack = script.get("pack");
	metadata.name = pack["name"].cast<std::string>();
	LOG("TexturePack:\"%s\".", metadata.name.c_str());
	metadata.version = pack["version"].cast<std::string>();

	luke::LuaRef asset = script.get("asset");
	std::string global_dir ("asset\\image\\"+asset["dir_global"].cast<std::string>());
	//a_main
	renderMainAtlas(asset,global_dir);
	a_main.qCreateUV();
	//a_env
	renderEnvAtlas(asset, global_dir);
	//blocks-----------
	luke::LuaRef rBlocks = pack["blocks"];
	blocks.resize(rBlocks.length());
	for (size_t i = 0; i < blocks.size(); i++) {
		luke::LuaRef block = rBlocks[i + 1];
		blocks[i].name = block["name"].cast<std::string>();
		blocks[i].physGroup = block["physGroup"];
		blocks[i].drawGroup = block["drawGroup"];
		//drawGroup
		if ((blocks[i].drawGroup == draw_non))
			blocks[i].isEmission = 0;
		else {
			if (blocks[i].physGroup == ph_liquid) {
				blocks[i].idActSide =	block["act"];
				blocks[i].idIdleSide =	block["idle"];
			}
			else blocks[i].setSide(block["side"]);
			if(block["emission"].isNil())blocks[i].isEmission = 0;
			else	blocks[i].setEmission(block["emission"]);
			 
		}
	}
	icon.setTexture(*render.texture());
	icon.transform.setPosition(glm::vec2(0.f));
	icon.transform.setScale(2.f, 2.f);
	LOG("success\n");
	///-----------
	return 1;
}

//VoxelAtlas-------------------------------------
/// <summary>
/// 
/// </summary>
bool ResourcePack::load(const std::string& directory) {
	//Json file
	JSON json;
	if (FileManager::read(directory+"atlas.json", json) == 0)return 0;
	//Texture
	ImageLoader::flipVerticallyOnLoad(1);
	if (texture_.load(directory +json.value("path", ""), 4) == 0) return 0;
	texture_.filter(tFilter::Nearest, tFilter::NearestMipmapNearest);
	//Parse
	sizeVoxel_ = json.value("size_uv",0);
	if (sizeVoxel_ == 0)
	{
		LOG(LogWarning, "VoxelAtlas::size_uv not set\n");
		sizeVoxel_ = 32;
	}

	uvSize_ = (float)sizeVoxel_ / (float)texture_.getSize().x;
	//scan
	blocks.resize(json.value("max_size_blocks",0));
	JSON& json_blocks = json["blocks"];
	for (size_t i = 0; i < blocks.size(); i++) {
		//name
		auto& block = json_blocks[std::to_string(i)];
		blocks[i].name = block["name"];
		//physGroup
		blocks[i].physGroup = block["physGroup"];
		//drawGroup
		blocks[i].drawGroup = block["drawGroup"];
		if (blocks[i].drawGroup == draw_non) {
			blocks[i].isEmission = 0;
		}
		else {
			//side
			auto& sides = block["side"];
			for (size_t side = 0; side < 6; side++) blocks[i].idSide[side] = sides[side];
			//emission
			auto& emission = block["emission"];
			blocks[i].emission = Uint8RGB(emission[0], emission[1], emission[2]); blocks[i].setEmissionFlag();
		}
	}
	//uv
	voxelInSide = texture_.getSize().x / sizeVoxel_;
	uv.resize(voxelInSide * voxelInSide);
	for (size_t i = 0; i < uv.size(); i++) uv[i] = getUV(i);

	icon.setTexture(VoxPack::get()->getAtlas());
	icon.transform.setPosition(glm::vec2(0.f));
	icon.transform.setScale(2.f, 2.f);
	return 1;
}

const TexturePack* VoxPack::res = 0;