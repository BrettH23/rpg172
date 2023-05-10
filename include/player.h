#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include<textureloader.h>

#include <commons.h>
#include "actor.h"

class player : public actor
{
    public:
        player();
        virtual ~player();

        float topSpeed;
        float attack;

        //float runSpeed;

        void playerInit(char*);
        void actions(acts);
        void moveP();
        void follow(float, float);
        void drawCursor(float, float, bool);
        void setFire(bool);

        bool direction;
        bool dUp, dDown, dRight, dLeft, firing;
        char playerDir; //determine direction including up and down

        int frameLimiter;

        /*
        void doBullet();
        void pushBullet();
        void cullBullet(bullet*);
        bullet* headBullet;
        void drawBullets();
        */


    protected:

    private:
};

#endif // PLAYER_H
