#ifndef CHECKCOLLISION_H
#define CHECKCOLLISION_H
#include <commons.h>

class checkCollision
{
    public:
        checkCollision();
        virtual ~checkCollision();

        bool isLinearCollision(float, float);
        bool isRadialCollision(float, float, float, float, float, float);
        bool isSphereCollision(vec3 x, vec3 y);
        bool isQuadCollision(vec2 x, vec2 y);

    protected:

    private:
};

#endif // CHECKCOLLISION_H
