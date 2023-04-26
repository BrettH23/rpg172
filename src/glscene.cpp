#include "glscene.h"
#include <gllight.h>
#include <model.h>
#include <inputs.h>
#include <parallax.h>

#include "actor.h"
#include <player.h>
#include <enemy.h>
#include <checkcollision.h>
#include <bulletpool.h>

#include "time.h"

Model *myFirstModel = new Model();
Inputs *KbMs = new Inputs();
parallax *prLX = new parallax();
player *ply = new player();
checkCollision *hit = new checkCollision();
bulletpool *eBullets = new bulletpool();

enemy ens[5];

clock_t timer;

GLuint tempTex;
GLuint bulletTex;

bool gameActive = true;

GLubyte* collider = new GLubyte[1];

float mouseX = 0.0;
float mouseY = 0.0;


GLScene::GLScene()
{
    //ctor
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);


}

GLScene::~GLScene()
{

}

void drawSquare(){
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
        glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
        glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
        glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
    glEnd();
}
int jz = 0;
int GLScene::drawScene()
{
    if(gameActive){
        drawGame();
    }else{
        drawMenu();
    }

}

void GLScene::drawGame()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);   //removed since using glortho
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glLoadIdentity();

    //glColor3f(0.2,0.7,0.7); //R G B (epic)

    int coordY = screenHeight-int((0.5-ply->position.y) * float(screenHeight))-1;
    int coordX = int((0.5+ply->position.x) * float(screenHeight)) + (screenWidth - screenHeight)/2;
    //*
    double x = double(clock()-timer)/double(CLOCKS_PER_SEC);
    if(x >= 0.025){
        timer = clock();
        //std::cout << int(clock()-timer) << ", " << x << std::endl;
        prLX->scroll(true, "y", 0.002); //autoscroll
        //ply->moveP();   //allows player to move, would be in idle if we had one
        ply->follow(mouseX, mouseY);
        eBullets->fire(jz); jz++;//std::cout << jz << std::endl;
        eBullets->tick();
    }


    glPushMatrix();
        glDisable(GL_LIGHTING);     //full 2d needs no lighting
        float scale = 0.5;
        float scRat = float(screenWidth)/float(screenHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (-scale*scRat, scale*scRat, -scale, scale, 0.5, -0.5);  //ortho is very nice for scaling 2d

        glPushMatrix();

            glColor3f(1.0,0.0,0.0);
            glScalef(0.2, 0.2, 1.0);
            glBindTexture(GL_TEXTURE_2D, bulletTex);
            drawSquare();
            glColor3f(1.0,1.0,1.0);

            glBindTexture(GL_TEXTURE_2D, tempTex);
            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, coordX, coordY, 1, 1,  0);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, collider);
            //std::cout << int(collider[0]) << std::endl;

        glPopMatrix();
        //std::cout << screenHeight << std::endl;




        glPushMatrix();

            //glScalef(3.33,3.33,1.0);


            prLX->drawSquare(screenHeight*(3.0/4.0),screenHeight);  //constrained to 3/4 ratio to emulate certain bullet hell games, parallax is scaled to be height of 1.0

            eBullets->draw();

        glPopMatrix();

        glPushMatrix();

            ply->draw();

            glDisable(GL_TEXTURE_2D);
            glColor3f(0.0,0.0,0.0);
            glPointSize(8.0);
            glBegin(GL_POINTS);
            glVertex2f(ply->position.x, ply->position.y);
            glEnd();

            glColor3f(1.0,1.0,1.0);
            glPointSize(4.0);
            glBegin(GL_POINTS);
            glVertex2f(ply->position.x, ply->position.y);
            glEnd();

            glPointSize(4.0);
            glBegin(GL_POINTS);
            glVertex2f(mouseX, mouseY);
            glEnd();

            glColor4f(1.0,1.0,1.0,0.3);
            glLineWidth(2.0);
            glBegin(GL_LINES);
            glVertex2f(ply->position.x, ply->position.y);
            glVertex2f(mouseX, mouseY);
            glEnd();
            glColor4f(1.0,1.0,1.0,1.0);

            glEnable(GL_TEXTURE_2D);

        glPopMatrix();


        glPushMatrix();

            //glColor3f(1.0,0.0,0.0);
            glScalef(0.2, 0.2, 1.0);
            glBindTexture(GL_TEXTURE_2D, bulletTex);
            drawSquare();
            //glColor3f(1.0,1.0,1.0);
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

        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, tempTex);

            glTranslatef(0.5,0.35,0.0);
            glScalef(0.2, 0.2, 1.0);
            drawSquare();

        glPopMatrix();

        //*/
        glEnable(GL_LIGHTING);

    glPopMatrix();
    //*/

}

void GLScene::drawMenu()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);   //removed since using glortho
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_LIGHTING);     //full 2d needs no lighting
    float scale = 0.5;
    float scRat = float(screenWidth)/float(screenHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (-scale*scRat, scale*scRat, -scale, scale, 0.5, -0.5);  //ortho is very nice for scaling 2d
}


int GLScene::GLinit()
{
    glClearDepth(1.0f);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    GLLight Light(GL_LIGHT0);
    Light.setLight(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);

    //myFirstModel->modelInit("images/flower.jpg");
    prLX->initParallax("images/stars.png");
    ply->playerInit("images/shipA2.png");
    ply->setSize(0.05);

    //eBullets->tLoad->loadTexture("images/bullets/bullet1.png", eBullets->bulletType[0].tex);
    eBullets->texInit();

    ens[0].enemyTexture("images/bad.png");

    for(int i = 0;i<5;i++){
        ens[i].initEnemy(ens[0].tex, 1.0, 1.0);
        ens[i].placeEnemy(vec3{((float)rand()/(float)(RAND_MAX))*0.5-0.25f, 0.2f, 0.0f});
        ens[i].setSize(float(float(10+rand()%20)/300.0));
    }
    glGenTextures(1, &tempTex);

    //KbMs->mdl = myFirstModel;   //copy model to mdl

    timer = clock();
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
    //switch(uMsg)
    //{
    if(uMsg == WM_KEYDOWN){
        KbMs->keyPlayer(ply);
        if(wParam == 'm' || wParam == 'M'){
            gameActive = !gameActive;
        }
    }
    if(uMsg == WM_KEYUP){
        ply->actions(ply->IDLE);
        KbMs->keyPlayerUp(ply);
    }

    if(uMsg == WM_LBUTTONDOWN){
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
    }
    if(uMsg == WM_RBUTTONDOWN){
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
    }
    if(uMsg == WM_MBUTTONDOWN){
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
    }
    if(uMsg == WM_LBUTTONUP){
        KbMs->mouseBtnUp();
    }
    if(uMsg == WM_RBUTTONUP){
        KbMs->mouseBtnUp();
    }
    if(uMsg == WM_MBUTTONUP){
        KbMs->mouseBtnUp();
    }
    if(uMsg == WM_MOUSEMOVE){
        KbMs->mouseMove(myFirstModel,LOWORD(lParam),HIWORD(lParam));
        mouseX = (double(LOWORD(lParam))+1.0) / double(screenWidth) - 0.5;
        mouseX *= float(screenWidth)/float(screenHeight);
        mouseY = 0.5-(double(HIWORD(lParam))+1.0) / double(screenHeight);
        //std::cout <<"Mouse:" << mouseX << ", " << mouseY << std::endl;
        //std::cout << "Player: " << ply->position.x << ", " << ply->position.y << std::endl << std::endl;
    }
    if(uMsg == WM_MOUSEWHEEL){
        KbMs->mouseWheel(myFirstModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
    }



}
