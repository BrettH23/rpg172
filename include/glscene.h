#ifndef GLSCENE_H
#define GLSCENE_H

#include <windows.h>

#include <string.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

class GLScene
{
    public:
        GLScene();
        virtual ~GLScene();
        int drawScene();   //main render
        void drawGame();
        void drawMenu();
        int GLinit();       //initialize scene
        void GLReSize(GLsizei, GLsizei);    //resize callback function


        int winMsg(HWND,UINT,WPARAM,LPARAM);

        int screenWidth, screenHeight;

    protected:

    private:
};

#endif // GLSCENE_H
