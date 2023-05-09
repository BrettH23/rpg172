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

    sizeRadius = 0.5;

    speed.x = 0.002;
    speed.y = 0.002;

    topSpeed = 0.008;
    theta = 0.0;

    dUp = dDown = dRight = dLeft = firing = false; //determines which directions are being held allowing orthogonal movement
}

player::~player()   //destructor for player, needs to clear those animations
{
    for(int i = 0;i < 3; i++){
        frame* tempFrame = &animations[i];
        while(tempFrame!=nullptr){
            if(tempFrame->next == &animations[i]){
                tempFrame->next = nullptr;  //makes sure deletion can't loop back to head
            }
        }
    }
    for(int i = 0;i < 3; i++){
        frame* thisFrame = &animations[i];
        while(thisFrame!=nullptr){

            frame* tempFrame = thisFrame;
            thisFrame = thisFrame->next;
            delete tempFrame;   //destroy all frames

        }
    }
}

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

    animations = new frame[3];

    int animX[4] = {0,1,2,1};

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

    frameLimiter = 0;

    lastTick = clock();
    /*
    bulletClock = clock();  //for tracking if it's time to tick a bullet
    headBullet = nullptr;   //headbullet initialized
    */
}

void player::actions(acts action)
{
    switch(action)
    {
        case IDLE:

            if(actionTrigger!=IDLE){
                curFrame = &animations[0];
            }else{
                curFrame = curFrame->next;
            }
            xMin = xFrame*curFrame->x;      //the current frame system allows for arbitrary arrangements of sprite sheets
            xMax = xFrame*(curFrame->x+1);  //could have a list of xcoords and ycoords as well and just use curframe to access the list
            yMin = yFrame*curFrame->y;
            yMax = yFrame*(curFrame->y+1);


            actionTrigger = IDLE;
            break;

        case WALKL:
            if(actionTrigger!=WALKL){
                curFrame = &animations[1];
            }else{
                curFrame = curFrame->next;
            }
            xMin = xFrame*(curFrame->x+1);
            xMax = xFrame*curFrame->x;
            yMin = yFrame*curFrame->y;
            yMax = yFrame*(curFrame->y+1);


            actionTrigger = WALKL;
        break;
        case WALKR:
            if(actionTrigger!=WALKR){
                curFrame = &animations[1];
            }else{
                curFrame = curFrame->next;
            }
            xMin = xFrame*curFrame->x;
            xMax = xFrame*(curFrame->x+1);
            yMin = yFrame*curFrame->y;
            yMax = yFrame*(curFrame->y+1);

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
    /*
    if(abs(x1) > adjX){
std::cout << xM << std::endl;
    }else{

    }

    if(abs(y1) > adjY){
std::cout << yM << std::endl;
    }else{

    }*/
}




