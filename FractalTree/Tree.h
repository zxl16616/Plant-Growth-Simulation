
#include "Texture.h"
#include "gl_texture_t.h"

#pragma once
class Tree
{
private:
	int N = 5;
	float baseTall = 8;		//初始高度
	float baseR = 6;		//初始底面半径
	float R[7];			//底面半径数组
	float H[7];			//树高数组
	float K = 0.5;			//顶面半径相对于底面半径缩放比例
	float Q = 0.5;			//底面半径缩放比例
	float M = 0.65;			//高度缩放比例
	float angle = 55;		//树枝开枝的角度
	int leaf = 9;
	GLUquadric *qObj;
	
	

public:
	Texture texture;
	Tree();
	~Tree();

	 static int img;
	 static int shape; //控制树形态，合轴为默认，值为0。分轴值为1。

	int getN();

	float getbaseTall();

	float getk();

	float getq();

	float getm();

	void setN(int n);

	void setbaseTall(float tall);

	void setk(float k);

	void setQ(float q);

	void setM(float m);

	void setAngle(float a);

	float getAngle();

	int getleaf();

	void setleaf(int lea);

	int getbaseR();

	void setbaseR(float r);

	void setData();		//设置半径数组和高度数组

	void DrawTree(int n);		//绘制根据迭代次数n绘制树

	float Gauss(float min, float max);

	gl_texture_t *ReadPNGFromFile(const char *filename);

	GLuint loadPNGTexture(const char *filename);

	void GetPNGtextureInfo(int color_type, gl_texture_t *texinfo);

	void DrawTreeInit();   //加载纹理等操作
};

