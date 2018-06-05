#include "Tree.h"
#include "Texture.h"
#include "SkyGround.h"
#pragma once
class COpenGL
{
public:
	HDC hDC;
	HGLRC hRC;
	GLfloat fLightPos[4] = { 50.0, 50.0, 30.0, 1.0f };  // 光照位置
	GLfloat fNoLight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat fLowLight[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat fBrightLight[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	M3DMatrix44f mShadowMatrix;
	Texture tex;
	SkyGround SG;
	GLFrame frameCamera;
	GLfloat xRot = 0.0f;

public:
	COpenGL();
	~COpenGL();
	Tree tree;


	bool SetupPixelFormat(HDC hdc); //设置像素格式为适合OpenGL的格式
	void SetupRC(void);
	void ShutDownRC(void);
	void RenderSence(void);
	void ChangeSize(int w, int h);
	void DrawInhabitants(int nShadow);
	void SpecialKeys(int key, int x, int y);
};

