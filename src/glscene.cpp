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

#include "time.h"

Model *myFirstModel = new Model();
Inputs *KbMs = new Inputs();
parallax *prLX = new parallax();
parallax *waves = new parallax();
player *ply = new player();
checkCollision *hit = new checkCollision();
bulletpool *eBullets = new bulletpool();
sound *snds = new sound();

enemy ens[5];

clock_t timer;

GLuint tempTex;
GLuint bulletTex;

bool gameActive = false;

GLubyte* collider = new GLubyte[1];

float mouseX = 0.0;
float mouseY = 0.0;

int bulletCycle = 0;

GLScene::GLScene()
{
    //ctor
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);

    LANDINGPAGE = true;
    MENUPAGE = false;
    GAMEPAGE = false;
    HELPPAGE = false;
    CREDITPAGE = false;
    QUIT = false;
    GAMEOVER = false;
    INTRO = false;


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

            if(gameActive){
        drawGame();
    }else{
        drawMenu();
    }
    /*
    drawGame();
    if(!gameActive){
        drawMenu();
    }
    */
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
    if(x >= 0.025 && gameActive){
        timer = clock();
        //std::cout << int(clock()-timer) << ", " << x << std::endl;
        prLX->scroll(true, "y", 0.002); //autoscroll
        waves->scroll(true, "y", 0.0008+0.002*sin(PI*float(bulletCycle)/100));
        //ply->moveP();   //allows player to move, would be in idle if we had one
        ply->follow(mouseX, mouseY, bulletCycle);
        eBullets->fire(bulletCycle); //std::cout << jz << std::endl;
        eBullets->aimed(bulletCycle, vec2{ply->position.x, ply->position.y});
        bulletCycle++;
        if(bulletCycle >= 100){ //cycles based on 5 second intervals
            bulletCycle = 0;
        }
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

            //glColor3f(1.0,0.0,0.0);
            //glScalef(0.2, 0.2, 1.0);
            //glBindTexture(GL_TEXTURE_2D, bulletTex);
            //drawSquare();
            //glColor3f(1.0,1.0,1.0);
            eBullets->drawMasks(vec2{ply->position.x, ply->position.y});

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


            prLX->drawSquare(screenHeight*(1.5),screenHeight);  //constrained to 3/4 ratio to emulate certain bullet hell games, parallax is scaled to be height of 1.0



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

            glColor4f(1.0,1.0,1.0,0.2);
            glLineWidth(2.0);
            glBegin(GL_LINES);
            glVertex2f(ply->position.x, ply->position.y);
            glVertex2f(mouseX, mouseY);
            glEnd();
            glColor4f(1.0,1.0,1.0,1.0);

            glEnable(GL_TEXTURE_2D);

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

            //glColor4f(1.0,1.0,1.0, 0.1);

            eBullets->draw();

            //drawSquare();
            //glColor4f(1.0,1.0,1.0, 1.0);
        glPopMatrix();

        glPushMatrix();

            glColor4f(1.0,1.0,1.0, 0.4);

            waves->drawSquare(screenHeight*(1.5),screenHeight);

            //drawSquare();
            glColor4f(1.0,1.0,1.0, 1.0);
        glPopMatrix();

        glPushMatrix();
            glDisable(GL_TEXTURE_2D);
            glColor3f(0.0, 0.0, 0.0);

            drawSquare((7.0/8.0), 0.0, 0.5);
            drawSquare((-7.0/8.0), 0.0, 0.5);

            glColor3f(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, tempTex);
            drawSquare(0.5, 0.3, 0.06);
        glPopMatrix();



        //*/
        glEnable(GL_LIGHTING);

    glPopMatrix();
    //*/

}

void GLScene::drawMenu()
{
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);   //removed since using glortho
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);     //full 2d needs no lighting
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float scale = 0.5;
    float scRat = float(screenWidth)/float(screenHeight);

    glPushMatrix();
        glOrtho (-scale*scRat, scale*scRat, -scale, scale, 0.5, -0.5);
        glColor4f(1.0,1.0,1.0, 0.2);
        if(LANDINGPAGE){
        //glPushMatrix();
        lPage->drawPage(screenHeight, screenHeight);
        //glPopMatrix();
        }
        if(MENUPAGE){
             //glPushMatrix();
             mPage->drawPage(screenHeight, screenHeight);
            //glPopMatrix();
        }
        if(HELPPAGE){
             //glPushMatrix();
            hPage->drawPage(screenHeight,screenHeight);
            //glPopMatrix();
        }
        if(CREDITPAGE){
            cPage->drawPage(screenHeight,screenHeight);
        }
        if(QUIT){
            qPage->drawPage(screenHeight/2,screenHeight/2);
        }
        glColor4f(1.0,1.0,1.0, 1.0);
    glPopMatrix();
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
    prLX->initParallax("SPRITES/wavebg.png");
    waves->initParallax("SPRITES/waveoverlay.png");
    ply->playerInit("SPRITES/player.png");
    ply->setSize(0.03, 0.0419);

    lPage->pageInit("images/menuTitle.png");//image landing page
    mPage->pageInit("images/menuPage4.png");//image Menu page
    hPage->pageInit("images/helpPage2.png");//image Help page
    qPage->pageInit("images/pausePage2.png"); //image Pause Page
    cPage->pageInit("images/creditsPage.png"); //image Credit Page

    snds->playMusic("sounds/gamePlay.mp3");

    //eBullets->tLoad->loadTexture("images/bullets/bullet1.png", eBullets->bulletType[0].tex);
    eBullets->texInit();

    ens[0].enemyTexture("SPRITES/piranha.png");

    for(int i = 0;i<5;i++){
        ens[i].initEnemy(ens[0].tex, 1.0, 1.0);
        ens[i].placeEnemy(vec3{((float)rand()/(float)(RAND_MAX))*0.5-0.25f, 0.2f, 0.0f});
        float randomSize = float(float(10+rand()%20)/300.0);
        ens[i].setSize(randomSize, randomSize);
    }
    glGenTextures(1, &tempTex);

    //KbMs->mdl = myFirstModel;   //copy model to mdl

    timer = clock();

    if(snds->initSounds()){
        return true;
    }
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
    /*
    if(wParam == 's')
    {
        setMenuPage();
    }
    if(wParam == 'n'){
    if(LANDINGPAGE == true)
    {
        setMenuPage();
    }else{
    LANDINGPAGE = false;
    MENUPAGE = false;
    GAMEPAGE = false;
    HELPPAGE = false;
    CREDITPAGE = false;
    QUIT = false;
    GAMEOVER = false;
    INTRO = false;
    gameActive = true;
    }
    }
    if(wParam == 'h'){
        //gameActive = false;
        //setHelpPage();
        if(gameActive == true){
            gameActive = false;
            setHelpPage();
        }
        else{
            setHelpPage();
        }
    }
    */
    if(wParam == 's' || wParam == 'S'){
        setMenuPage();
        gameActive = false;
    }
    if((wParam == 'n' || wParam == 'N')){
        gameActive = true;
    }
    if(wParam == 'h' || wParam == 'H'){
        setHelpPage();
        gameActive = false;
    }
    if(wParam == 'b' || wParam == 'B'){
        setMenuPage();
        gameActive = false;
    }
    if(wParam == 'c' || wParam == 'C'){
        setCreditPage();
        gameActive = false;
    }
    if((wParam == 'p' || wParam == 'P') && gameActive == true){
        setQuitPage();
        gameActive = false;
    }
    if((wParam == 'r' || wParam == 'R') && QUIT == true){
        setGamePage();
        gameActive = true;
    }
    if((wParam == 'M' || wParam == 'm') && QUIT == true){
        setMenuPage();
        gameActive = false;
    }
        if((wParam == 'M' || wParam == 'm') && gameActive == true){

    }


}

void GLScene::setMenuPage()
{
    LANDINGPAGE = false;
    MENUPAGE = true;
    GAMEPAGE = false;
    HELPPAGE = false;
    CREDITPAGE = false;
    QUIT = false;
    GAMEOVER = false;
    INTRO = false;
}

void GLScene::setGamePage()
{
    LANDINGPAGE = false;
    MENUPAGE = false;
    GAMEPAGE = true;
    HELPPAGE = false;
    CREDITPAGE = false;
    QUIT = false;
    GAMEOVER = false;
    INTRO = false;
}

void GLScene::setHelpPage()
{
    LANDINGPAGE = false;
    MENUPAGE = false;
    GAMEPAGE = false;
    HELPPAGE = true;
    CREDITPAGE = false;
    QUIT = false;
    GAMEOVER = false;
    INTRO = false;
}

void GLScene::setCreditPage()
{
    LANDINGPAGE = false;
    MENUPAGE = false;
    GAMEPAGE = false;
    HELPPAGE = false;
    CREDITPAGE = true;
    QUIT = false;
    GAMEOVER = false;
    INTRO = false;
}

void GLScene::setQuitPage()
{
    LANDINGPAGE = false;
    MENUPAGE = false;
    GAMEPAGE = false;
    HELPPAGE = false;
    CREDITPAGE = false;
    QUIT = true;
    GAMEOVER = false;
    INTRO = false;
}

void GLScene::setGameOverPage()
{
    LANDINGPAGE = true;
    MENUPAGE = false;
    GAMEPAGE = false;
    HELPPAGE = false;
    CREDITPAGE = false;
    QUIT = false;
    GAMEOVER = true;
    INTRO = false;
}


