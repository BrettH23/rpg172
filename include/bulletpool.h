#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include <GL/gl.h>

#include "textureloader.h"
#include "commons.h"

typedef struct dll{
    dll* prev = nullptr;
    dll* next = nullptr;
    int index;
};

typedef struct bullet{
    vec2 verts[4];
    vec2 p;
    vec2 v;
    vec2 a;
    int ticks;
};

typedef struct type_b{
    dll* alive;
    GLuint tex;
    GLuint mask;
    float radius;
};

class bulletpool
{
    public:
        bulletpool();
        virtual ~bulletpool();

        int cap;
        float degree;

        enum type_p {PLAYER, ENEMY};
        type_p poolType;

        dll* dead;  //head pointer to one dll chain

        int typeCount;
        type_b* bulletType;  //array of types of bullets corresponding to alive indices
        //dll* alive;         //array of head pointer to dll chains
        dll* nearPlayer;


        bullet* bullets;

        void texInit();
        void draw();
        void fire(int);
        void spawn(int, int, float, float, float, vec2);
        void die(dll);
        void tick();

        textureLoader *tLoad = new textureLoader();

    protected:

    private:
};

#endif // BULLETPOOL_H
