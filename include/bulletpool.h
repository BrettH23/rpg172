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
    float damage;
    float radius;
    vec3 hue;
};

class bulletpool
{
    public:
        bulletpool();
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

        void initE(int);
        void initP(int);

        void draw();
        void drawMasks(vec2);

        void spawn(int, int, float, float, float, float, vec2);
        void die(int, dll*);
        void clearAll();
        void fadeAll();
        void tick();
        bool checkBounds(int, int);
        float getDamage(vec2, vec2, bool);

        void playerFire(int, int, vec2);
        void fire(int, int, vec2, vec2, float);
        void fireOcto(int, vec2, vec2, float);
        void firePiranha(int, vec2, vec2, float);
        void firePuffer(int, vec2, vec2, float);
        void fireTurt(int, vec2, vec2, float);
        void fireBoss(int, vec2, vec2, float);

        void deathThroes(int, vec2, vec2);
        void dieOcto(vec2, vec2);
        void diePiranha(vec2, vec2);
        void diePuffer(vec2, vec2);
        void dieTurt(vec2, vec2);
        void dieBoss(vec2, vec2);
        void dieUrchin(vec2, vec2);


        void doomSpiral(int, vec2);
        void aimed(int, vec2, vec2);

        textureLoader *tLoad = new textureLoader();

    protected:

    private:
};

#endif // BULLETPOOL_H
