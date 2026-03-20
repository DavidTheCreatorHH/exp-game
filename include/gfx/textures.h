#ifndef TEXTURES_H
#define TEXTURES_H

#include "glew.h"
#include <string>
#include <memory>
#include <array>

// Class for the textures
class Texture{
	public:
		Texture(GLenum TextureTarget, const std::string& Filename);
		int Load();
		void Bind(GLenum TextureUnit);
		GLuint getID() const { return m_TextureObj; }
	private:
		std::string m_Filename;
		GLenum m_TextureTarget;
		GLuint m_TextureObj;
};

// Number Identifiers for Textures
enum TextureID {
    TEX_GROUND,
    TEX_GROUND_OLD,
    TEX_DIRT,
    TEX_TASR,
    TEX_TAL_TILE,
    TEX_LAVA,
    TEX_COLD_LAVA,
    TEX_EREL_CRYSTAL,
    TEX_OBSIDIAN,
    TEX_TEST,
    TEX_COUNT
};

extern std::array<std::unique_ptr<Texture>, TEX_COUNT> Textures; // An array for the textures

// Make the textures accesible
extern void RegisterTextures();

extern void LoadAllTextures();

#endif
