#include "menu.h"

menu::menu()
{
    //ctor
    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
}

menu::~menu()
{
    //dtor
}
GLvoid menu::pageInit(char* fileName)
{
    tex->loadTexture1(fileName);
}

GLvoid menu::drawPage(GLfloat w , GLfloat h)
{
    glPushMatrix();

    tex->TextureBinder();
    //glScaled(4.15,4.14,1.0);

    glBegin(GL_POLYGON);
    glTexCoord2f(xMin,yMin);
    glVertex2f(-w/(2*h),0.5);
    glTexCoord2f(xMax,yMin);
    glVertex2f(w/(2*h),0.5);
    glTexCoord2f(xMax,yMax);
    glVertex2f(w/(2*h),-0.5);
    glTexCoord2f(xMin,yMax);
    glVertex2f(-w/(2*h),-0.5);

    glEnd();
}
