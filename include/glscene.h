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
        void drawGame();
        void drawMenu();
        int GLinit();       //initialize scene
        void GLReSize(GLsizei, GLsizei);    //resize callback function

         bool LANDINGPAGE, MENUPAGE, GAMEPAGE, HELPPAGE, CREDITPAGE,QUIT,GAMEOVER,INTRO;

        menu *lPage = new menu();
        menu *mPage = new menu();
        menu *hPage = new menu();
        menu *cPage = new menu();
        menu *qPage = new menu();
        menu *goPage = new menu();


        void setMenuPage();
        void setGamePage();
        void setHelpPage();
        void setCreditPage();
        void setQuitPage();
        void setGameOverPage();
        void setIntroPage();


        int winMsg(HWND,UINT,WPARAM,LPARAM);

        int screenWidth, screenHeight;

    protected:

    private:
};

#endif // GLSCENE_H
