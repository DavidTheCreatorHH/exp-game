// it's just fully deprecated. this will be deleted in the future version
/*
#include <GL/glew.h>
#include "gfx/graphics.h"
#include "gfx/text.h"

void TDraw_Char(Texture* atlas, char c, float x, float y, float size){
    int cols = 16;
    int rows = 16;

    int ascii = (unsigned char)c;
    if(ascii < 32 || ascii > 127) ascii = 0;

    int index = ascii;
    int col = index % cols;
    int row = index / cols;

    float uvW = 1.0f / cols;
    float uvH = 1.0f / rows;

	float u0 = col * uvW;
	float u1 = u0 + uvW;

	float v0 = row * uvH;
	float v1 = v0 + uvH;

    
	int atlasW = 32;
	int atlasH = 32;

	float padU = 1.0f / atlasW; // 1 pixel
	float padV = 1.0f / atlasH;

	u0 += padU;
	u1 -= padU;
	v0 += padV;
	v1 -= padV;

	Draw_Quad_Char(atlas, x, y, 0.f, u0, u1, v0, v1, size);
}

void TDraw_Text(Texture* atlas, const std::string& text, float x, float y, float size, float spacing = 0.0f){
	glPushMatrix();	
	for(unsigned int i = 0; i < text.size(); i++) {
		glPushMatrix();
		glScalef(size / 10.f, size / 10.f, 1.f);
		TDraw_Char(atlas, text[i], x + i * (size + spacing), y, size);
		glPopMatrix();
	}
	glPopMatrix();
}
*/
