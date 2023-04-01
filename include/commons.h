#ifndef COMMONS_H
#define COMMONS_H
#include <math.h>
#define GRAVITY 9.81
#define PI 3.14159
#include <time.h>

typedef struct vec3
{
    float x;
    float y;
    float z;
}vec3;

typedef struct vec2
{
    float x;
    float y;
}vec2;

class commons
{
    public:
        commons();
        virtual ~commons();

    protected:

    private:
};

#endif // COMMONS_H
