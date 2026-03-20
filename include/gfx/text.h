#ifndef TEXT_H
#define TEXT_H

#include "glew.h"
#include <string.h>
#include "textures.h"

extern void Draw_Quad(Texture& tex);
extern void Draw_Quad_Char(Texture* tex, float x, float y, float z, float u0, float u1, float v0, float v1, float size);

// Init the VBOs
extern void init_quadvbo();

extern void init_quaduv();

// For drawing the actual text
extern void TDraw_Char(Texture* atlas, char c, float x, float y, float size);

extern void TDraw_Text(Texture* atlas, const std::string& text, float x, float y, float size, float spacing);

#endif
