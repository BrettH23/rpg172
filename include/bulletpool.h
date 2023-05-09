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
    float angle;
    float angVel;
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
        bulletpool(int);
        virtual ~bulletpool();

        int cap;
        float fireAngle;

        enum type_p {PLAYER, ENEMY};
        type_p poolType;

        dll* dead;  //head pointer to one dll chain

        vec2 bounds[2];

        int typeCount;
        type_b* bulletType;  //array of types of bullets corresponding to alive indices
        //dll* alive;         //array of head pointer to dll chains
        dll* nearPlayer;


        bullet* bullets;

        void texInit();
        void draw();
        void drawMasks(vec2);

        void spawn(int, int, float, float, float, float, vec2);
        void die(dll*);
        void clearAll();
        void tick();
        bool checkBounds(int, int);

        void fire(int, int, vec2, vec2);
        void doomSpiral(int, vec2);
        void aimed(int, vec2, vec2);

        textureLoader *tLoad = new textureLoader();

    protected:

    private:
};

#endif // BULLETPOOL_H
