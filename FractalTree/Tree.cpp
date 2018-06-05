#include "stdafx.h"
#include "Tree.h"


int Tree::img = 0;
int Tree::shape = 0;

Tree::Tree()
{

}

Tree::~Tree()
{
}

int Tree::getN()
{
	return N;
}

float Tree::getbaseTall() {
	return baseTall;
}

float Tree::getk() {
	return K;
}

float Tree::getq() {
	return Q;
}

float Tree::getm() {
	return M;
}

void Tree::setN(int n)
{
	N = n;
}

void Tree::setbaseTall(float tall) {
	baseTall = tall;
}

void Tree::setk(float k) {
	K = k;
}

void Tree::setQ(float q) {
	Q = q;
}

void Tree::setM(float m) {
	M = m;
}

void Tree::setAngle(float a)
{
	angle = a;
}

float Tree::getAngle()
{
	return angle;
}

int Tree::getleaf()
{
	return leaf;
}

void Tree::setleaf(int lea)
{
	leaf = lea;
}

int Tree::getbaseR()
{
	return baseR;
}

void Tree::setbaseR(float r)
{
	baseR = r;
}

void Tree::setData() {
	R[N] = baseR*0.01;
	H[N] = baseTall*0.1;

	for (int i = N - 1; i >= 0; i--)
	{
		R[i] = R[i + 1] * Q;
		H[i] = H[i + 1] * M;
	}
}

float Tree::Gauss(float min, float max)
{
	int a = rand(), b = rand(), c = rand(), d = rand();
	int x = a + b + c + d;
	double y = (x*sqrt(3.0)) / RAND_MAX - sqrt(12.0);
	double mid = (min + max) / 2;
	float z = (float)(mid + (max - mid)*y / (2 * sqrt(3.0)));
	return z;
}

gl_texture_t * Tree::ReadPNGFromFile(const char * filename)
{
	gl_texture_t *texinfo;
	png_byte magic[8];
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth, color_type;
	FILE *fp = NULL;
	png_bytep *row_pointers = NULL;
	png_uint_32 w, h;
	int i;
	/* Open image file */
	fopen_s(&fp, filename, "rb");
	if (!fp)
	{
		fprintf(stderr, "error: couldn't open \"%s\"!\n", filename);
		return NULL;
	}
	/* Read magic number */
	fread(magic, 1, sizeof(magic), fp);
	/* Check for valid magic number */
	if (!png_check_sig(magic, sizeof(magic)))
	{
		fprintf(stderr, "error: \"%s\" is not a valid PNG image!\n", filename);
		fclose(fp);
		return NULL;
	}
	/* Create a png read struct */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		return NULL;
	}
	/* Create a png info struct */
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}
	/* Create our OpenGL texture object */
	texinfo = (gl_texture_t *)malloc(sizeof(gl_texture_t));
	/* Initialize the setjmp for returning properly after a libpng error occured */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		if (row_pointers) free(row_pointers);
		if (texinfo) {
			if (texinfo->texels)
				free(texinfo->texels);
			free(texinfo);
		}
		return NULL;
	}
	/* Setup libpng for using standard C fread() function with our FILE pointer */
	png_init_io(png_ptr, fp);
	/* Tell libpng that we have already read the magic number */
	png_set_sig_bytes(png_ptr, sizeof(magic));
	/* Read png info */
	png_read_info(png_ptr, info_ptr);
	/* Get some usefull information from header */
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	/* Convert index color images to RGB images */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	/* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
	if (bit_depth == 16) png_set_strip_16(png_ptr);
	else if (bit_depth < 8) png_set_packing(png_ptr);
	/* Update info structure to apply transformations */
	png_read_update_info(png_ptr, info_ptr);
	/* Retrieve updated information */
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
	texinfo->width = w;
	texinfo->height = h;
	/* Get image format and components per pixel */
	GetPNGtextureInfo(color_type, texinfo);
	/* We can now allocate memory for storing pixel data */
	texinfo->texels = (GLubyte *)malloc(sizeof(GLubyte) * texinfo->width * texinfo->height * texinfo->internalFormat);
	/* Setup a pointer array. Each one points at the begening of a row. */
	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * texinfo->height);
	for (i = 0; i < texinfo->height; ++i)
	{
		row_pointers[i] = (png_bytep)(texinfo->texels + ((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
	}
	/* Read pixel data using row pointers */
	png_read_image(png_ptr, row_pointers);
	/* Finish decompression and release memory */
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	/* We don't need row pointers anymore */
	free(row_pointers);
	fclose(fp);
	return texinfo;
}

GLuint Tree::loadPNGTexture(const char * filename)
{
	gl_texture_t *png_tex = NULL;
	GLuint tex_id = 0;
	GLint alignment;
	png_tex = ReadPNGFromFile(filename);
	if (png_tex && png_tex->texels)
	{
		/* Generate texture */
		glGenTextures(1, &png_tex->id);
		glBindTexture(GL_TEXTURE_2D, png_tex->id);
		/* Setup some parameters for texture filters and mipmapping */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, png_tex->internalFormat, png_tex->width, png_tex->height, 0, png_tex->format, GL_UNSIGNED_BYTE, png_tex->texels);
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		tex_id = png_tex->id;
		/* OpenGL has its own copy of texture data */
		free(png_tex->texels);
		free(png_tex);
	}
	return tex_id;
}

void Tree::GetPNGtextureInfo(int color_type, gl_texture_t * texinfo)
{
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		texinfo->format = GL_LUMINANCE;
		texinfo->internalFormat = 1;
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		texinfo->format = GL_LUMINANCE_ALPHA;
		texinfo->internalFormat = 2;
		break;

	case PNG_COLOR_TYPE_RGB:
		texinfo->format = GL_RGB;
		texinfo->internalFormat = 3;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		texinfo->format = GL_RGBA;
		texinfo->internalFormat = 4;
		break;

	default:
		/* Badness */
		break;
	}
}

void Tree::DrawTreeInit()				//画树前的准备工作，减少递归中的操作，提高渲染速度
{
	texture.Init();						//绑定纹理
	qObj = gluNewQuadric();
	gluQuadricNormals(qObj, GLU_SMOOTH);  //设置二次方程表面的平滑法线
	gluQuadricTexture(qObj, GL_TRUE);	  //设置表面生成纹理坐标
}

void Tree::DrawTree(int n)
{
	if (shape == 0) {	//合轴树
		if (n == 0)		
		{
			float h = 0.5;                       //随机产生一个0.4-0.7之间的高度比例
			glBindTexture(GL_TEXTURE_2D, texture.textureObjects[WOOD_TEXTURE]);
			gluCylinder(qObj, R[n], K * R[n], H[n] * h, 40, 40);
			glTranslatef(0, 0, H[n] * h);

			glRotatef(40, 1, 0, 0);			//绕X轴旋转一个随机角度
			glRotatef(45, 0, 1, 0);						//绕Y轴旋转45°
			glRotatef(-90, 1, 0, 0);						//绕X轴旋转90°

			//img = loadPNGTexture("leaf.png");
			glBindTexture(GL_TEXTURE_2D, img);

			glAlphaFunc(GL_GREATER, 0.1);
			glEnable(GL_ALPHA_TEST);

			float leafsize = leaf * 0.01; //控制树叶大小

			glBegin(GL_QUADS);
			glNormal3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(leafsize, 0.0f, 0.0f);

			glNormal3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(leafsize, 2 * leafsize, 0.0f);

			glNormal3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-leafsize, 2 * leafsize, 0.0f);

			glNormal3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-leafsize, 0.0f, 0.0f);
			glEnd();

			glDisable(GL_ALPHA_TEST);
			return;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture.textureObjects[WOOD_TEXTURE]);
			gluCylinder(qObj, R[n], K*R[n], H[n], 20, 20);

			glTranslatef(0, 0, H[n]);

			glPushMatrix();
			glTranslatef(0, 0, -H[n] * 0.3);
			glRotatef(55, 0, 0, 1);
			glRotatef(angle, 0, 1, 0);
			DrawTree(n - 1);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 0, -H[n] * 0.5);
			glRotatef(170, 0, 0, 1);
			glRotatef(angle-15, 0, 1, 0);
			DrawTree(n - 1);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 0, -H[n] * 0.2);
			glRotatef(290, 0, 0, 1);
			glRotatef(angle, 0, 1, 0);
			DrawTree(n - 1);
			glPopMatrix();

			glRotatef(10, 0, 1, 0);
			glRotatef(angle, 0, 0, 1);
			glTranslatef(0, 0, -0.01);
			DrawTree(n - 1);
		}
	}
	else if (shape = 1) {
		if (n == 0)
		{
			float h = 0.5;                       //随机产生一个0.4-0.7之间的高度比例
			glBindTexture(GL_TEXTURE_2D, texture.textureObjects[WOOD_TEXTURE]);
			gluCylinder(qObj, R[n], K * R[n], H[n] * h, 40, 40);
			glTranslatef(0, 0, H[n] * h);

			glRotatef(40, 1, 0, 0);			//绕X轴旋转一个随机角度
			glRotatef(45, 0, 1, 0);						//绕Y轴旋转45°
			glRotatef(-90, 1, 0, 0);						//绕X轴旋转90°

															//img = loadPNGTexture("leaf.png");
			glBindTexture(GL_TEXTURE_2D, img);

			glAlphaFunc(GL_GREATER, 0.1);
			glEnable(GL_ALPHA_TEST);

			float leafsize = leaf * 0.01; //控制树叶大小

			glBegin(GL_QUADS);
			glNormal3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(leafsize, 0.0f, 0.0f);

			glNormal3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(leafsize, 2 * leafsize, 0.0f);

			glNormal3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-leafsize, 2 * leafsize, 0.0f);

			glNormal3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-leafsize, 0.0f, 0.0f);
			glEnd();

			glDisable(GL_ALPHA_TEST);
			return;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture.textureObjects[WOOD_TEXTURE]);
			gluCylinder(qObj, R[n], K*R[n], H[n], 20, 20);

			glTranslatef(0, 0, H[n]);

			glPushMatrix();
			glRotatef(10, 0, 0, 1);
			glRotatef(angle, 0, 1, 0);
			DrawTree(n - 1);
			glPopMatrix();

			glPushMatrix();
			glRotatef(150, 0, 0, 1);
			glRotatef(angle, 0, 1, 0);
			DrawTree(n - 1);
			glPopMatrix();

			glPushMatrix();
			glRotatef(260, 0, 0, 1);
			glRotatef(angle, 0, 1, 0);
			DrawTree(n - 1);
			glPopMatrix();
		}
	}
}
