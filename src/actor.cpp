#include "actor.h"

actor::actor()
{
    //ctor

    verts[0].x = -0.5; verts[0].y = -0.5; verts[0].z = 0.0;
    verts[1].x = 0.5; verts[1].y = -0.5; verts[1].z = 0.0;
    verts[2].x = 0.5; verts[2].y = 0.5; verts[2].z = 0.0;
    verts[3].x = -0.5; verts[3].y = 0.5; verts[3].z = 0.0;

    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;

    actionTrigger = IDLE;

}

actor::~actor()   //destructor for player, needs to clear those animations
{


}

void actor::draw()
{

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,tex);
    glTranslated(position.x,position.y,position.z);
    glBegin(GL_QUADS);

        glTexCoord2f(xMin,yMax);
        glVertex3f(verts[0].x,verts[0].y,verts[0].z);

        glTexCoord2f(xMax,yMax);
        glVertex3f(verts[1].x,verts[1].y,verts[1].z);

        glTexCoord2f(xMax,yMin);
        glVertex3f(verts[2].x,verts[2].y,verts[2].z);

        glTexCoord2f(xMin,yMin);
        glVertex3f(verts[3].x,verts[3].y,verts[3].z);
    glEnd();
    glPopMatrix();
}

void actor::texInit(char* filename)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);




    tLoad->loadTexture(filename, tex);
    //tLoad->loadTexture("images/bullet.png", bulletTex); //bullet is stored in the player class
}

void actor::setSize(float sizeToSet)
{
    sizeRadius = sizeToSet;
    for(int i = 0; i < 4; i++){ //resizes all verts, allowed me to change the size around easily
        verts[i].x = sizeRadius;
        verts[i].y = sizeRadius;
    }
    verts[0].x *= -1.0;verts[3].x *= -1.0;
    verts[0].y *= -1.0;verts[1].y *= -1.0;
}
