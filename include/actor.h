#ifndef ACTOR_H
#define ACTOR_H

#include <GL/gl.h>
#include<textureloader.h>

#include <commons.h>

typedef struct frame
{
    bool inverted = false;
    int x;
    int y;
    frame* next = nullptr;
};

class actor
{
    public:
        actor();
        virtual ~actor();

        vec3 verts[4];
        vec3 position;
        float sizeRadius;

        vec2 speed;
        float bulletSpeed;

        enum acts{IDLE, WALKL, WALKR, JUMP, ATTACK, DIE, DEAD};
        acts actionTrigger;

        //float runSpeed;

        void draw();
        void texInit(char*);
        void setSize(float, float);

        clock_t lastTick;
        clock_t bulletClock;

        int hFrames;
        int vFrames;

        float yFrame, xFrame;

        float xMax, yMax, xMin, yMin;




        GLuint tex;
        GLuint bulletTex;
        textureLoader *tLoad = new textureLoader();

        frame* animations;
        frame* curFrame;




    protected:

    private:
};

#endif // ACTOR_H
