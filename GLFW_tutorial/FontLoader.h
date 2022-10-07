#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <ft2build.h>
#include FT_FREETYPE_H 
#include <map>
#include <iostream>

/////FaceFont-----------------------------------------
class FaceFont {
public:

	FaceFont(const FT_Face& face) :face_(face)
	{}

	bool loadGlyph(size_t code,FT_Int32 flag = FT_LOAD_RENDER) {
		if (FT_Load_Char(face_, code, flag)) {
			printf("(!)ERROR::FaceFont::Failed to load glyph (code:%d)\n", code);
			return 0;
		}
		return 1;
	}
	const FT_GlyphSlot& getSlot() {
		return face_->glyph;
	}
	const FT_Face& get()const  {
		return face_;
	}

	void setPixelSize(size_t width,size_t height) {
		FT_Set_Pixel_Sizes(face_, width, height);
	}

	~FaceFont() {
		FT_Done_Face(face_);
	}

private:

	FT_Face face_;
};

/////FontResource-----------------------------------------
class FontResource {
		
public:

	FontResource(const FT_Library& lib,const std::string& path) {
		FT_Face face;
		if (FT_New_Face(lib, path.c_str(), 0, &face)) {
			printf("(!)ERROR::FontResource::Failed to load font from path:%s\n", path.c_str());
			font = 0;
		}
		font = std::make_shared<FaceFont>(face);
	}
	FontResource() {
		font = 0;
	}
	const std::shared_ptr<FaceFont>& get()const {
		return font;
	}
	
private:

	std::shared_ptr< FaceFont> font;
};

/////FontLoader-----------------------------------------
class FontLoader
{
	struct FreeTypeLib {
	public:

		FreeTypeLib() {
			if (FT_Init_FreeType(&lib)) {
				printf("(!)ERROR::Font::FREETYPE not init");
				init = 0;
			}
			init = 1;
		}

		FT_Library& get() {
			return lib;
		}

		bool isInit() const {
			return init;
		}

		void free() {
			FT_Done_FreeType(lib);
		}

	private:

		bool init;
		FT_Library lib;

	};

public:

	static const std::shared_ptr <FaceFont>& get(const std::string& path) {
		auto face = fonts.find(path);
		if (face == fonts.end()) {
			return load(path);
		}
		else return face->second.get();
	}

	static bool isInit(){
		return lib.isInit();
	}
private:

	static const std::shared_ptr <FaceFont>& load(const std::string& path)
	{
		FontResource font(lib.get(), path);
		if (font.get()) {
			fonts[path] = font;
			return fonts[path].get();
		}
		return 0;
	}

	FontLoader() {}
	~FontLoader() {}

	static std::map< std::string, FontResource > fonts;
	static FreeTypeLib lib;

};
#endif