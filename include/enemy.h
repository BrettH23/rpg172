#ifndef ENEMY_H
#define ENEMY_H

#include <GL/gl.h>
#include<textureloader.h>

#include "actor.h"
#include <commons.h>
#include <time.h>


class enemy: public actor
{
    public:
        enemy();
        virtual ~enemy();
        void placeEnemy(vec3);
        void initEnemy(GLuint, int, int);
        void enemyTexture(char*);
        void doBehavior();

        void actions();


        vec3 eRotation;      //rotation

        clock_t startTime;

        float theta, velocity, t;//t is projectile time


        char direction; //determine direction including up and down


    protected:

    private:
};

#endif // ENEMY_H
