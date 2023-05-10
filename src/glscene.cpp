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
#include <sound.h>
#include <font.h>
#include <level.h>

#include "time.h"

Model *myFirstModel = new Model();
Inputs *KbMs = new Inputs();
parallax *prLX = new parallax();
parallax *waves = new parallax();
player *ply = new player();
checkCollision *hit = new checkCollision();
bulletpool *eBullets = new bulletpool();
bulletpool *pBullets = new bulletpool();
sound *snds = new sound();
font *writer = new font();
level *lv = new level();
menu *menuHud = new menu();



clock_t timer;

GLuint tempTex;

bool gameActive = false;

GLubyte* collider = new GLubyte[1];

float mouseX = 0.0;
float mouseY = 0.0;

float waveCycle = 0.0;

GLScene::GLScene()
{
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
}

GLScene::~GLScene()
{

}

void drawSquare(float x, float y, float s){
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(-s + x, s + y);
        glTexCoord2f(1.0, 1.0); glVertex2f(s + x, s + y);
        glTexCoord2f(1.0, 0.0); glVertex2f(s + x, -s + y);
        glTexCoord2f(0.0, 0.0); glVertex2f(-s + x, -s + y);
    glEnd();
}

int GLScene::drawScene()
{


    int coordY = screenHeight-int((0.5-ply->position.y) * float(screenHeight))-1;
    int coordX = int((0.5+ply->position.x) * float(screenHeight)) + (screenWidth - screenHeight)/2;
    //*
    double timePassed = double(clock()-timer)/double(CLOCKS_PER_SEC);
    if(timePassed >= 0.015 && gameActive){
        timer = clock();
        //std::cout << int(clock()-timer) << ", " << timePassed << std::endl;
        prLX->scroll(true, "y", 0.002); //autoscroll
        waves->scroll(true, "y", 0.0015+0.0008*sin(waveCycle));
        //ply->moveP();   //allows player to move, would be in idle if we had one
        if(ply->invul <=0 && int(collider[0]) < 25){
            ply->hit(1.0);
            ply->invul = 100;
        }
        ply->follow(mouseX, mouseY);
        ply->tick();
        waveCycle+= 0.02;
        if(waveCycle >= 2*PI){ //cycles based on 3 second intervals
            waveCycle -= 2*PI;
        }
        lv->tickLevel();
        eBullets->tick();
        pBullets->tick();
    }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_DEPTH_TEST);   //removed since using glortho
    //glEnable (GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    glPushMatrix();
        glDisable(GL_LIGHTING);     //full 2d needs no lighting
        float scale = 0.5;
        float scRat = float(screenWidth)/float(screenHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (-scale*scRat, scale*scRat, -scale, scale, 0.5, -0.5);  //ortho is very nice for scaling 2d

        glPushMatrix();

            //glColor3f(1.0,0.0,0.0);
            //glScalef(0.2, 0.2, 1.0);
            //glBindTexture(GL_TEXTURE_2D, bulletTex);
            //drawSquare();
            //glColor3f(1.0,1.0,1.0);
            lv->drawEnemyMasks();
            eBullets->drawMasks(vec2{ply->position.x, ply->position.y});


            glBindTexture(GL_TEXTURE_2D, tempTex);
            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, coordX, coordY, 1, 1,  0);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_UNSIGNED_BYTE, collider);
            //std::cout << int(collider[0]) << std::endl;

        glPopMatrix();
        //std::cout << screenHeight << std::endl;




        glPushMatrix();

            prLX->drawSquare(screenHeight*1.5,screenHeight);  //constrained to 3/4 ratio to emulate certain bullet hell games, parallax is scaled to be height of 1.0

        glPopMatrix();

        glPushMatrix();

            glColor4f(1.0,1.0,1.0, 0.4);

            waves->drawSquare(screenHeight*(1.5),screenHeight);

            //drawSquare();
            glColor4f(1.0,1.0,1.0, 1.0);
        glPopMatrix();

        glPushMatrix();
            lv->drawEnemies();
        glPopMatrix();

        glPushMatrix();

            ply->draw();




        glPopMatrix();





        glPushMatrix();

            //glColor4f(1.0,1.0,1.0, 0.1);

            eBullets->draw();
            pBullets->draw();
            //drawSquare();
            //glColor4f(1.0,1.0,1.0, 1.0);
        glPopMatrix();

        glPushMatrix();
            glDisable(GL_TEXTURE_2D);
            glColor3f(0.5, 0.5, 0.5);

            drawSquare(1.0, 0.0, 0.5);
            drawSquare(-1.0, 0.0, 0.5);

            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
            writer->drawLineCentered("MWM0", 0.05, 0.65,0.1, 1,3);
            writer->drawLine("HAHA", 0.6,0.0);
            writer->drawLine("yeah", 0.6,-0.1);
        glPopMatrix();

        glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D, tempTex);
            //drawSquare(0.5, 0.3, 0.06);
        glPopMatrix();

        glPushMatrix();
            ply->drawCursor(mouseX, mouseY, gameActive);
            menuHud->drawPage(1.0,1.0);
            if(!gameActive){
                drawCursor();
            }
        glPopMatrix();



        //*/
        glEnable(GL_LIGHTING);

    glPopMatrix();
    //*/

}

void GLScene::drawCursor()
{
    float c_size = 0.008;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, pBullets->bulletType->tex);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex2f(mouseX - c_size, mouseY + c_size);
    glTexCoord2f(1,0);glVertex2f(mouseX + c_size, mouseY + c_size);
    glTexCoord2f(1,1);glVertex2f(mouseX + c_size, mouseY - c_size);
    glTexCoord2f(0,1);glVertex2f(mouseX - c_size, mouseY - c_size);
    glEnd();
    glPopMatrix();
}


int GLScene::GLinit()
{
    ShowCursor(FALSE);
    glClearDepth(1.0f);
    glClearColor(0.0f,0.0f,1.0f,0.0f);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    GLLight Light(GL_LIGHT0);
    Light.setLight(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);

    //myFirstModel->modelInit("images/flower.jpg");
    prLX->initParallax("images/sprites/wavebg.png");
    waves->initParallax("images/sprites/waveoverlay.png");
    ply->playerInit("images/sprites/player.png");
    ply->setSize(0.03, 0.0419);

    //eBullets->tLoad->loadTexture("images/bullets/bullet1.png", eBullets->bulletType[0].tex);
    eBullets->initE(1000);
    pBullets->initP(100);


    writer->initFonts("images/jokerman.png");
    writer->kerning = -0.35;
    menuHud->init(writer);

    lv->init(ply, eBullets, pBullets);
    lv->loadLevel(0);

    glGenTextures(1, &tempTex);

    timer = clock();

    snds->initSounds();

    //snds->playMusic("sounds/gameSound.mp3");

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

    if(uMsg == WM_KEYDOWN){
        KbMs->keyPlayer(ply);
        if(wParam == 'm' || wParam == 'M'){
            gameActive = !gameActive;
        }
        if(wParam == 'n' || wParam == 'N'){
            menuHud->currentState++;
            if(menuHud->currentState > 7){
                menuHud->currentState = 0;
            }
        }
    }
    if(uMsg == WM_KEYUP){
        KbMs->keyPlayerUp(ply);
    }

    if(uMsg == WM_LBUTTONDOWN){
        KbMs->mouseBtnDown(myFirstModel,LOWORD(lParam),HIWORD(lParam));
        menuHud->click();
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
        menuHud->hover(mouseX, mouseY);
    }
    if(uMsg == WM_MOUSEWHEEL){
        KbMs->mouseWheel(myFirstModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
    }

    if(wParam == '1'){
        lv->loadLevel(1);
    }
    if(wParam == '0'){
        lv->loadLevel(0);
    }


}



