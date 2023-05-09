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

    sizeRadius = vec2{0.5, 0.5};

    startTime = clock();
    actionTrigger = WALKL;

    theta = 0.0;
    velocity = 35;
    type = 0;
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
    vFrames = vFrm;
    hFrames = hFrm;

    xMax = 1.0/(float)hFrames;
    xMin = 0.0;
    yMax = 1.0/(float)vFrames;
    yMin = 0.0;

    this->tex = tex;
}

void enemy::actions(acts a)
{
    actionTrigger = a;
    switch(a)
    {
    case IDLE:

    break;

    case WALKL:
        xMax = 0.0;
        xMin = 1.0/(float)hFrames;
    break;
    case WALKR:
        xMax = 1.0/(float)hFrames;
        xMin = 0.0;
    break;
    }
}
