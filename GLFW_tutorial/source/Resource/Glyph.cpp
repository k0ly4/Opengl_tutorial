#include "Glyph.h"

void Glyph::load(const FT_Face& face) {
    texture_ = std::make_shared<Texture2DLite>();
    size = glm::uvec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    advantage = face->glyph->advance.x >> 6;

    sTexture::bind2D(*texture_);
    sTexture::dataImage2D( {face->glyph->bitmap.width, face->glyph->bitmap.rows} , { GL_RED,GL_RED }, face->glyph->bitmap.buffer);
   /* glTexImage2D(GL_TEXTURE_2D, 
        0, 
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0, 
        GL_RED, 
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer);*/

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
