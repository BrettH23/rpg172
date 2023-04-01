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




        enum acts{IDLE, WALKL, WALKR, JUMP, ATTACK, DIE};
        acts actionTrigger;

        //float runSpeed;

        void playerInit(char*);
        void actions(acts);
        void moveP();


        bool direction;
        bool dUp, dDown, dRight, dLeft, firing;
        char playerDir; //determine direction including up and down

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
