#include "Texture.h"

#pragma once
class SkyGround
{
public:
	Texture texture;
	GLUquadric *qObj;

	SkyGround();
	~SkyGround();

	void DrawSky();
	void DrawGround();
};

