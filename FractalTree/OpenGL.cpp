#include "stdafx.h"
#include "OpenGL.h"

COpenGL::COpenGL()
	:hDC(NULL)
	,hRC(NULL)
{
}


COpenGL::~COpenGL()
{
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}


bool COpenGL::SetupPixelFormat(HDC hdc)
{
	hDC = hdc;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小
		1,                                // 版本号
		PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图
		PFD_SUPPORT_OPENGL |              // 支持OpenGL
		PFD_DOUBLEBUFFER,                 // 双缓存模式
		PFD_TYPE_RGBA,                    // RGBA 颜色模式
		24,                               // 24 位颜色深度
		0, 0, 0, 0, 0, 0,                 // 忽略颜色位
		0,                                // 没有非透明度缓存
		0,                                // 忽略移位位
		0,                                // 无累加缓存
		0, 0, 0, 0,                       // 忽略累加位
		32,                               // 32 位深度缓存   
		0,                                // 无模板缓存
		0,                                // 无辅助缓存
		PFD_MAIN_PLANE,                   // 主层
		0,                                // 保留
		0, 0, 0                           // 忽略层,可见性和损毁掩模
	};

	int nPixelFormat;    // 像素点格式
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"can not find proper mode", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	SetPixelFormat(hDC, nPixelFormat, &pfd);
	hRC = wglCreateContext(hDC);    //利用GDI绘图所使用的HDC创建对应的HGLRC
	wglMakeCurrent(hDC, hRC);        //使OpenGL绘图所使用的HGLRC为当前绘图工具

	return TRUE;
}

void COpenGL::SetupRC(void)
{
	M3DVector3f vPoints[3] = { { 0.0f, -0.4f, 0.0f },
	{ 10.0f, -0.4f, 0.0f },
	{ 5.0f, -0.4f, -5.0f } };
	
	
			  
	glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);  // 灰色背景


	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glClearStencil(0);
	glStencilFunc(GL_EQUAL, 0x0, 0x01);


	glCullFace(GL_BACK);              //禁用多边形背面上的光照、阴影和颜色计算及操作
	glFrontFace(GL_CCW);			  //逆时针为正面
	glEnable(GL_CULL_FACE);			  //背面剔除
	glEnable(GL_DEPTH_TEST);          //深度测试
	glEnable(GL_MULTISAMPLE_ARB);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fLowLight);	//设置全局环境光为黑色
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);		//环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fLowLight);		//散射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);	//镜面光
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// 根据地面上三个点获得平面方程
	M3DVector4f pPlane;
	m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);

	//计算投影矩阵，在地面上绘制阴影
	m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);


	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	
	Tree::img = tree.loadPNGTexture("leaf.png");
	glEnable(GL_MULTISAMPLE);  //打开多重采样
}

void COpenGL::ShutDownRC(void)
{
	glDeleteTextures(3, tex.textureObjects);
	
}

void COpenGL::RenderSence(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();
	tree.setData();//设置数据

	glTranslatef(0.0f, 0.0f, -5.0f);  //视图变换
	frameCamera.ApplyCameraTransform();
	glRotatef(xRot, 0.0f, 1.0f, 0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

	
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
		SG.DrawSky();
	glPopMatrix();

	SG.DrawGround();

	glDisable(GL_FOG);


	glColor3f(1.0f, 1.0f, 1.0f);
	// 画阴影
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_STENCIL_TEST);
	glPushMatrix();
		glMultMatrixf(mShadowMatrix);
		DrawInhabitants(1);
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

		DrawInhabitants(0);

	glPopMatrix();

	//glFlush();
	SwapBuffers(wglGetCurrentDC());
	//glutSwapBuffers();
}

void COpenGL::ChangeSize(int w, int h)
{
	GLfloat fAspect;

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void COpenGL::DrawInhabitants(int nShadow)
{
	if (nShadow == 0)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
		glColor4f(0.0f, 0.0f, 0.0f, 0.6f);  // 阴影的颜色

	glPushMatrix();
	glTranslatef(0.0f, -0.4f, 0.0f);
	glRotatef(-90, 1.0, 0, 0);

	if (nShadow == 0)		//如果是画树的话
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, fBrightLight);
	}
	
	
	tree.DrawTreeInit();  //画树前的准备工作
	int n = tree.getN();
	tree.DrawTree(n);

	glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
	glPopMatrix();
}

void COpenGL::SpecialKeys(int key, int x, int y)
{
	float fa = frameCamera.GetOriginZ();

	if (key == VK_UP && fa > -3.0)
		frameCamera.MoveForward(0.1f);

	if (key == VK_DOWN && fa < 15.0)
		frameCamera.MoveForward(-0.1f);

	if (key == VK_LEFT)
		xRot -= 5.0f;
	if (key == VK_RIGHT)
		xRot += 5.0f;
	if (xRot > 356.0f)
		xRot = 0.0f;
	if (xRot < -1.0f)
		xRot = 355.0f;

	// 重画窗口
	//glutPostRedisplay();
	
}
