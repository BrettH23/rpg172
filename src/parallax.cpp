#include "parallax.h"

parallax::parallax()
{
    xMax = 1.0;
    xMin = 0.0;
    yMax = 0.0;
    yMin = 1.0;

    startTime = clock();
}

parallax::~parallax()
{
    //dtor
}

void parallax::drawSquare(float width, float height)
{
    glColor3f(1.0, 1.0, 1.0);
    bTex->binder(tex);
    float pRatio = width/height;
    glBegin(GL_POLYGON);
        glTexCoord2f(xMin, yMin);
        glVertex3f(-0.5*pRatio, -0.5, 0.0);

        glTexCoord2f(xMax, yMin);
        glVertex3f(0.5*pRatio, -0.5, 0.0);

        glTexCoord2f(xMax, yMax);
        glVertex3f(0.5*pRatio, 0.5, 0.0);

        glTexCoord2f(xMin, yMax);
        glVertex3f(-0.5*pRatio, 0.5, 0.0);

    glEnd();
}

void parallax::initParallax(char* filename)
{
    bTex->loadTexture(filename, tex);
}

void parallax::scroll(bool scrl, string dir, float speed)
{
    if(scrl){

        if(clock()-startTime>15){
            if(dir == "x"){
                xMin += speed;
                xMax += speed;
            }
            else if(dir == "-x"){
                xMin -= speed;
                xMax -= speed;
            }
            else if(dir == "y"){
                yMin -= speed;
                yMax -= speed;
            }
            else {
                yMin += speed;
                yMax += speed;
            }
            startTime=clock();
        }


    }
}
