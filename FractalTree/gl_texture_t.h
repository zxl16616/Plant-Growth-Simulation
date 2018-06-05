#pragma once
class gl_texture_t
{
public:
	gl_texture_t();
	~gl_texture_t();
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLint internalFormat;
	GLuint id;
	GLubyte *texels;
};

