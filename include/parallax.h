#ifndef PARALLAX_H
#define PARALLAX_H

#include <textureloader.h>
#include <SOIL.h>
#include<GL/gl.h>
#include <string.h>
#include <time.h>

using namespace std;

class parallax
{
    public:
        parallax();
        virtual ~parallax();

        void drawSquare(float, float); //epic
        void initParallax(char*);
        void scroll(bool,string,float);

        float xMax, xMin, yMax, yMin;

        textureLoader *bTex = new textureLoader();

        clock_t startTime;

        GLuint tex;


    protected:

    private:
};

#endif // PARALLAX_H
