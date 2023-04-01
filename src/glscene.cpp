#include "glscene.h"
#include <gllight.h>
#include <model.h>
#include <inputs.h>
#include <parallax.h>

#include "actor.h"
#include <player.h>
#include <enemy.h>
#include <checkcollision.h>

Model *myFirstModel = new Model();
Inputs *KbMs = new Inputs();
parallax *prLX = new parallax();
player *ply = new player();
checkCollision *hit = new checkCollision();

enemy ens[5];

GLScene::GLScene()
{
    //ctor
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
}

GLScene::~GLScene()
{

}
int GLScene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);   //removed since using glortho
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glLoadIdentity();

    //glColor3f(0.2,0.7,0.7); //R G B (epic)

    //*
    glPushMatrix();
        glDisable(GL_LIGHTING);     //full 2d needs no lighting
        float scale = 0.5;
        float scRat = float(screenWidth)/float(screenHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (-scale*scRat, scale*scRat, -scale, scale, 1.0, -1.0);  //ortho is very nice for scaling 2d
        //std::cout << screenHeight << std::endl;
        glPushMatrix();

            //glScalef(3.33,3.33,1.0);

            prLX->scroll(true, "y", 0.002); //autoscroll
            prLX->drawSquare(screenHeight*(3.0/4.0),screenHeight);  //constrained to 3/4 ratio to emulate certain bullet hell games, parallax is scaled to be height of 1.0

            //prLX->scroll(true,"y",0.001);
        glPopMatrix();

        glPushMatrix();
            //glOrtho(-5.0, 5.0, -5.0, 5.0, 0.1 , 1000);
            //ply->doBullet();    //bullet generator logic
            //ply->drawBullets(); //draws all bullets in the current bullet list

        glPopMatrix();

        glPushMatrix();
            //glOrtho(-5.0, 5.0, -5.0, 5.0, 0.1 , 1000);
            //glTranslatef(0.0,0.0,-1.0);
            //glScalef(0.5,0.5,1.0);
            ply->moveP();   //allows player to move, would be in idle if we had one
            ply->draw();

        glPopMatrix();




        //*
        glPushMatrix();


        for(int i = 0;i<5;i++){

            if(ens[i].position.x < -0.3){   //constrains enemies
                ens[i].actionTrigger = ens[i].WALKR;
            }else if(ens[i].position.x > 0.3){
                ens[i].actionTrigger = ens[i].WALKL;
            }

            ens[i].actions();
            ens[i].draw();
        }
        glPopMatrix();
        //*/
        glEnable(GL_LIGHTING);

    glPopMatrix();
    //*/


}

int GLScene::GLinit()
{
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    GLLight Light(GL_LIGHT0);
    Light.setLight(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);

    myFirstModel->modelInit("images/flower.jpg");
    prLX->initParallax("images/stars.png");
    ply->playerInit("images/shipA2.png");
    ply->setSize(0.05);


    ens[0].enemyTexture("images/bad.png");

    for(int i = 0;i<5;i++){
        ens[i].initEnemy(ens[0].tex, 1.0, 1.0);
        ens[i].placeEnemy(vec3{((float)rand()/(float)(RAND_MAX))*0.5-0.25f, 0.2f, 0.0f});
        ens[i].setSize(float(float(10+rand()%20)/300.0));
    }


    //KbMs->mdl = myFirstModel;   //copy model to mdl

    return true;
}

void GLScene::GLReSize(GLsizei width, GLsizei height)
{
    screenWidth = width;
    screenHeight = height;
    float ratio = (float)width/(float)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,ratio,0.1,1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int GLScene::winMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    KbMs->wParam = wParam;
    switch(uMsg)
    {
    case WM_KEYDOWN:

        //KbMs->keyPressed(myFirstModel);
        //KbMs->keyEnv(prLX,0.005);
        KbMs->keyPlayer(ply);
        break;

    case WM_KEYUP:
        ply->actions(ply->IDLE);
        KbMs->keyPlayerUp(ply);
        break;

    case WM_LBUTTONDOWN:
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_RBUTTONDOWN:
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_MBUTTONDOWN:
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_LBUTTONUP:
        KbMs->mouseBtnUp();
        break;

    case WM_RBUTTONUP:
        KbMs->mouseBtnUp();
        break;

    case WM_MBUTTONUP:
        KbMs->mouseBtnUp();
        break;

    case WM_MOUSEMOVE:
        KbMs->mouseMove(myFirstModel,LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_MOUSEWHEEL:
        KbMs->mouseWheel(myFirstModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;


    }
}
