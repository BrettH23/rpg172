#include "enemy.h"

enemy::enemy()
{
    verts[0].x = -0.5; verts[0].y = -0.5; verts[0].z = 0.0;
    verts[1].x = 0.5; verts[1].y = -0.5; verts[1].z = 0.0;
    verts[2].x = 0.5; verts[2].y = 0.5; verts[2].z = 0.0;
    verts[3].x = -0.5; verts[3].y = 0.5; verts[3].z = 0.0;

    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;

    eRotation.x = eRotation.y = eRotation.z = 0;

    speed.x = 0.002*(1+(float(rand()%100)/50.0));
    speed.y = 0.0;

    sizeRadius = 0.5;

    startTime = clock();
    actionTrigger = WALKL;

    theta = 30*PI/180;
    velocity = 35;
    t = 0;
}

enemy::~enemy()
{

}

void enemy::placeEnemy(vec3 p)
{
    position.x = p.x;
    position.y = p.y;
    position.z = p.z;
}

void enemy::enemyTexture(char* filename)
{
    tLoad->loadTexture(filename, tex);

}


void enemy::initEnemy(GLuint tex, int vFrm, int hFrm)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    vFrames = vFrm;
    hFrames = hFrm;

    xMax = 1.0/(float)hFrames;
    xMin = 0.0;
    yMax = 1.0/(float)vFrames;
    yMin = 0.0;

    this->tex = tex;
}

void enemy::actions()
{
    switch(actionTrigger)
    {
    case IDLE:
        if(clock()-startTime>15){
            xMin +=1.0/(float)hFrames;  //only needed one frame for this
            xMax +=1.0/(float)hFrames;
            startTime = clock();
        }
    break;

    case WALKL:
        if(clock()-startTime>15){
            //xMin +=1.0/(float)hFrames;
            //xMax +=1.0/(float)hFrames;

            position.x -=speed.x;
            //position.y = 0.01*cos(position.x);
            //position.y = -0.5;
            //yMax = 1.0/(float)vFrames;
            //yMin = 0.0;
            eRotation.z = 0.0;
            startTime = clock();
        }
    break;
    case WALKR:
        if(clock()-startTime>15){
            //xMin +=1.0/(float)hFrames;
            //xMax +=1.0/(float)hFrames;

            position.x +=speed.x;
            //position.y = 0.01*cos(position.x);
            //position.y = -0.5;
            //yMax = 1.0;
            //yMin = 1.0/(float)vFrames;
            eRotation.z = 0.0;
            startTime = clock();
        }
    break;
    }

}
