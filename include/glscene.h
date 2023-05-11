#ifndef GLSCENE_H
#define GLSCENE_H

#include <windows.h>

#include <string.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <menu.h>

class GLScene
{
    public:
        GLScene();
        virtual ~GLScene();
        int drawScene();   //main render
        int GLinit();       //initialize scene
        void GLReSize(GLsizei, GLsizei);    //resize callback function
        bool quit = false;
        void dumbDestructor();

        void drawCursor();


        int winMsg(HWND,UINT,WPARAM,LPARAM);

        int screenWidth, screenHeight;

    protected:

    private:
};

#endif // GLSCENE_H
