#include "stdafx.h"
#include "SkyGround.h"


SkyGround::SkyGround()
{
}


SkyGround::~SkyGround()
{
}

void SkyGround::DrawSky()
{
	GLfloat fExtent = 20.0f;
	glCullFace(GL_BACK);
	texture.Init();
	glBindTexture(GL_TEXTURE_2D, texture.textureObjects[SKY_TEXTURE]);

	qObj = gluNewQuadric();
	gluQuadricNormals(qObj, GLU_SMOOTH);  //设置二次方程表面的平滑法线
	gluQuadricTexture(qObj, GL_TRUE);	  //设置表面生成纹理坐标
	gluQuadricOrientation(qObj, GLU_INSIDE);

	
	gluSphere(qObj, fExtent, 40, 40);
}

void SkyGround::DrawGround()
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iStrip, iRun;
	GLfloat s = 0.0f;
	GLfloat t = 0.0f;
	GLfloat texStep = 1.0f / (fExtent * .075f);

	texture.Init();
	glBindTexture(GL_TEXTURE_2D, texture.textureObjects[GROUND_TEXTURE]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
	{
		t = 0.0f;
		glBegin(GL_TRIANGLE_STRIP);

		for (iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
		{
			glTexCoord2f(s, t);
			glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
			glVertex3f(iStrip, y, iRun);

			glTexCoord2f(s + texStep, t);
			glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
			glVertex3f(iStrip + fStep, y, iRun);

			t += texStep;
		}
		glEnd();
		s += texStep;
	}
}
