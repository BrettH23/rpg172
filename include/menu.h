#ifndef MENU_H
#define MENU_H

#include<textureloader.h>
#include <GL/gl.h>


class menu
{
    public:
        menu();
        virtual ~menu();
         textureLoader* tex = new textureLoader();

        GLvoid pageInit(char*);
        GLvoid drawPage(GLfloat, GLfloat);

        float xMin,xMax, yMin, yMax;

    protected:

    private:
};

#endif // MENU_H
