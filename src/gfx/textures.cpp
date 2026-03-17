#include "gfx/textures.h"
#include "stb/stb_image.h"
#include <iostream>
#include <memory>
#include <array>

// Texture class for creation, loading and binding

Texture::Texture(GLenum TextureTarget, const std::string& Filename){
	m_TextureTarget = TextureTarget;
	m_Filename = Filename;
}

int Texture::Load(){
	stbi_set_flip_vertically_on_load(1);
	int width = 0, height = 0, bpp = 0;
	unsigned char* image_data = stbi_load(m_Filename.c_str(), &width, &height, &bpp, 0);

	if(!image_data){
		std::cout << "ERR: Failed to load texture: " << image_data;
		return -1;
	}

	GLenum frmat = GL_RGB;
	if (bpp == 4) frmat = GL_RGBA;
	else if (bpp == 3) frmat = GL_RGB;
	else if (bpp == 1) frmat = GL_RED;

	glGenTextures(1, &m_TextureObj);
	glBindTexture(m_TextureTarget, m_TextureObj);
	if(m_TextureTarget == GL_TEXTURE_2D){
		glTexImage2D(m_TextureTarget, 0, frmat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	}
	else{
		std::cout << "ERR: Function for texture not implemented: " << m_TextureTarget;
	}

	glTexParameterf(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(m_TextureTarget, 0);
	stbi_image_free(image_data);

	return 0;
}

void Texture::Bind(GLenum TextureUnit){
	glActiveTexture(TextureUnit);
	glBindTexture(m_TextureTarget, m_TextureObj);
}

std::array<std::unique_ptr<Texture>, TEX_COUNT> Textures; // An array for the textures

// Make the textures accesible
void RegisterTextures() {
	Textures[TEX_GROUND]		= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/ground.png");
	Textures[TEX_GROUND_OLD]	= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/ground1.png");
	Textures[TEX_DIRT]			= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/dirt.png");
	Textures[TEX_TASR]			= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/tasr.png");
	Textures[TEX_TAL_TILE]		= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/tal_tile.png");
	Textures[TEX_LAVA]			= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/lava.png");
	Textures[TEX_COLD_LAVA]		= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/cold_lava.png");
	Textures[TEX_EREL_CRYSTAL]	= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/erel_crystal.png");
	Textures[TEX_OBSIDIAN]		= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/obsidian.png");
	Textures[TEX_TEST]			= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/test.png");
	//Textures[TEX_FONT]			= std::make_unique<Texture>(GL_TEXTURE_2D, "texs/text_map.png");
}

void LoadAllTextures() {
    for (auto& tex : Textures)
        tex->Load();
}
