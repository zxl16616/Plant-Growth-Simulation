#include "stdafx.h"
#include "Texture.h"


Texture::Texture()
{
	
}


Texture::~Texture()
{
}

void Texture::Init()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(NUM_TEXTURES, textureObjects);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		GLbyte *pBytes;
		GLint iWidth, iHeight, iComponents;
		GLenum eFormat;

		glBindTexture(GL_TEXTURE_2D, textureObjects[i]);

		pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat);
		gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
		free(pBytes);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//ÏßÐÔ¹ýÂË
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}
