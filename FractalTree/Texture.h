
#define GROUND_TEXTURE  0	//大地纹理
#define WOOD_TEXTURE    1	//树干纹理
#define SKY_TEXTURE		2	//天空纹理
#define NUM_TEXTURES    3	//纹理数目

#pragma once
class Texture
{
public:
	GLuint  textureObjects[NUM_TEXTURES];
	const char *szTextureFiles[NUM_TEXTURES] = { "grass.tga", "wood.tga","sky.tga" };
	Texture();
	~Texture();
	void Init();
};

