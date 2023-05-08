#ifndef FONT_H
#define FONT_H

#include "commons.h"
#include "textureloader.h"
#include "stdio.h"
#include "string.h"

typedef struct
{
    char letter;
    float xMin, xMax, yMin, yMax;
    vec3 fontSize;
}charSet;

class font
{
    public:
        font();
        virtual ~font();

        void initFonts(char*);
        void buildFonts(char*);
        void drawFonts(int);
        void drawLine(char*, float, float);
        float scale;
        float kerning;

        vec3 Rotate;
        vec3 position;
        float zoom;

        int cCount;
        GLuint tex;
        textureLoader* tLoad = new textureLoader();

        float oneOverHex = 1.0/16.0;    //precalc because mult is faster than div

    protected:

    private:
};

#endif // FONT_H
