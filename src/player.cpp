#include "player.h"

player::player()
{
    //ctor

    verts[0].x = -0.5; verts[0].y = -0.5; verts[0].z = 0.0;
    verts[1].x = 0.5; verts[1].y = -0.5; verts[1].z = 0.0;
    verts[2].x = 0.5; verts[2].y = 0.5; verts[2].z = 0.0;
    verts[3].x = -0.5; verts[3].y = 0.5; verts[3].z = 0.0;

    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;

    actionTrigger = IDLE;
    direction = true;
    playerDir = 'R';

    sizeRadius = vec2{0.5, 0.5};

    speed.x = 0.002;
    speed.y = 0.002;

    topSpeed = 0.006;
    theta = 0.0;

    maxHP = 4.0;
    attack = 10.0;

    invul = 0;

    dUp = dDown = dRight = dLeft = firing = firing1 = false; //determines which directions are being held allowing orthogonal movement
}

player::~player()   //destructor for player, needs to clear those animations
{


}

void player::drawCursor(float mouseX, float mouseY, bool showLine)
{
    if(actionTrigger != DEAD){
        glPushMatrix();
            glDisable(GL_TEXTURE_2D);
            glColor3f(0.0,0.0,0.0);
            glPointSize(8.0);
            glBegin(GL_POINTS);
            glVertex2f(position.x, position.y);
            glEnd();

            glColor3f(1.0,1.0,1.0);
            glPointSize(4.0);
            glBegin(GL_POINTS);
            glVertex2f(position.x, position.y);
            glEnd();

            glPointSize(4.0);
            glBegin(GL_POINTS);
            glVertex2f(mouseX, mouseY);
            glEnd();

            if(showLine){
                glColor4f(1.0,1.0,1.0,0.2);
                glLineWidth(2.0);
                glBegin(GL_LINES);
                glVertex2f(position.x, position.y);
                glVertex2f(mouseX, mouseY);
                glEnd();
            }

            glColor4f(1.0,1.0,1.0,1.0);

            glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

float animX[4] = {0,1,2,1};
void player::playerInit(char* filename)
{
    texInit(filename);

    vFrames = 1;
    hFrames = 3;

    xMax = 1.0/(float)hFrames;
    xMin = 0.0;
    yMax = 1.0/(float)vFrames;
    yMin = 0.0;

    xFrame = xMax;
    yFrame = yMax;
    curX = 0;

    /*
    animations = new frame[3];



    for(int j = 0; j < 3; j++){ //custom animation system, uses loops since all animations are exactly four frames.
        curFrame = &animations[j];  //current frame of animation stores the 'coordinates' of the current sprite tile
        curFrame->x = 0;            //first frame initialized, loop gets the rest.
        curFrame->y = 0;
        curFrame->next = nullptr;   //animations are linked lists

        for(int i = 1; i < 4; i++){
            frame* tempFrame = new frame;   //similar to previous operations, animations are pretty much a grid
            curFrame->next = tempFrame;
            curFrame = tempFrame;
            curFrame->x = animX[i];
            curFrame->y = 0;
            curFrame->next = nullptr;
        }
        curFrame->next = &animations[j];
    }
    curFrame = &animations[0];  //loop animation. Now advancing a frame is always an o(1) operation
*/
    frameLimiter = 0;



}

void player::setFire(bool b)
{
    if(actionTrigger != DEAD){
        firing = b;
    }
    else{
        firing = false;
    }
}


void player::actions(acts action)
{
    switch(action)
    {
        case DEAD:
            firing = false;
        break;


        case WALKR:

            xMin = xFrame*(animX[curX]);
            xMax = xFrame*(1.0+animX[curX]);
            curX++;
            if(curX >= 4)curX = 0;

            actionTrigger = WALKR;
        break;
    }
}


void player::moveP()    //used for moving the player, also checks if time is allowed
{
    if(clock()-lastTick > 15){  //only runs every 15
        lastTick = clock();
        float xSpeed = float(dRight) - float(dLeft);    //speed calculated from which directions are held
        float ySpeed = float(dUp) - float(dDown);       //system allows diagonal directions

        if(xSpeed != 0.0){          //scales down speed to be rougly equal to 1 unit for fun
            ySpeed *= 0.7;
        }
        if(ySpeed != 0.0){
            xSpeed *= 0.7;
        }

        if(xSpeed>0&&position.x >= 0.325 || xSpeed<0&&position.x <= -0.325){    //constrains movement
            xSpeed = 0.0;
        }
        if(ySpeed>0&&position.y >= 0.375 || ySpeed<0&&position.y <= -0.375){
            ySpeed = 0.0;
        }
        position.x += xSpeed*speed.x; //speed scaled to player speed
        position.y += ySpeed*speed.y;
        if(xSpeed>0){
            this->actions(WALKR);   //also sets action
        }else if(xSpeed<0){
            this->actions(WALKL);
        }else{
            this->actions(IDLE);
        }




    }

}

void player::follow(float xM, float yM)
{
    if(actionTrigger != DEAD){
        float x1 = xM - position.x;
        float y1 = yM - position.y;
        float fRatio = sqrt(x1*x1 + y1*y1);
        float adjX = (x1*topSpeed)/fRatio;
        float adjY = (y1*topSpeed)/fRatio;
        frameLimiter++;
        if(frameLimiter > 3){
            this->actions(WALKR);
            frameLimiter = 0;
        }


        if(topSpeed > fRatio){
            position.x = xM;
            position.y = yM;

        }else{
            position.x += adjX;
            position.y += adjY;
        }
        float xOffset = 0.5-0.002;
        float yOffset = 0.5-0.002;
        if(position.x >= xOffset){
            position.x = xOffset;
        }if(position.x <= -xOffset){
            position.x = -xOffset;
        }

        if(position.y >= yOffset){
            position.y = yOffset;
        }if(position.y <= -yOffset){
            position.y = -yOffset;
        }

    }
}




