#include "inputs.h"




Inputs::Inputs()
{
    //ctor
}

Inputs::~Inputs()
{
    //dtor
}
void Inputs::keyEnv(parallax* plx, float speed)
{
    switch(wParam)
    {
    case VK_LEFT:
        plx->xMax -=speed;
        plx->xMin -=speed;
        break;
    case VK_RIGHT:
        plx->xMax +=speed;
        plx->xMin +=speed;
        break;
    case VK_UP:
        plx->yMax -=speed;
        plx->yMin -=speed;
        break;
    case VK_DOWN:
        plx->yMax +=speed;
        plx->yMin +=speed;
        break;
    }
}

void Inputs::keyPressed(Model * mdl)
{
    switch(wParam)
    {
    case VK_LEFT:
        mdl->rotateY -=1.0;
        break;
    case VK_RIGHT:
        mdl->rotateY +=1.0;
        break;
    case VK_UP:
        mdl->rotateX -=1.0;
        break;
    case VK_DOWN:
        mdl->rotateX +=1.0;
        break;

    case VK_SUBTRACT:
        mdl->zoom -=1.0;
        break;
    case VK_ADD:
        mdl->zoom +=1.0;
        break;
    }
}
void Inputs::keyPlayer(player* ply)
{
    //switch(wParam){ //instead of incrementing user, sets a user state, then the user deals with the motion accordingly. Allows speed to not be locked to input speed
        if(wParam == ' ')ply->firing = true;

        if(wParam == 'a'||wParam == 'A')ply->dLeft = true;

        if(wParam == 'd'||wParam == 'D')ply->dRight = true;

        if(wParam == 'w'||wParam == 'W')ply->dUp = true;

        if(wParam == 's'||wParam == 'S')ply->dDown = true;

    //}


}

void Inputs::keyPlayerUp(player* ply)
{

        if(wParam == ' ')ply->firing = false;

        if(wParam == 'a'||wParam == 'A')ply->dLeft = false;

        if(wParam == 'd'||wParam == 'D')ply->dRight = false;

        if(wParam == 'w'||wParam == 'W')ply->dUp = false;

        if(wParam == 's'||wParam == 'S')ply->dDown = false;

}




void Inputs::keyUp()
{
    switch(wParam){

    }
}

void Inputs::mouseBtnDown(Model *mdl, double x, double y)
{
    prev_Mx = x;
    prev_My = y;

    switch(wParam){
        case MK_LBUTTON:
            Mouse_rotate = true;
        break;
        case MK_RBUTTON:
            Mouse_translate = true;
        break;
        case MK_MBUTTON:break;
    }
}

void Inputs::mouseBtnUp()
{
    Mouse_rotate = false;
    Mouse_translate = false;
}

void Inputs::mouseWheel(Model *mdl,double delta)
{
    mdl->zoom += delta/100.0;
}

void Inputs::mouseMove(Model *mdl, double x, double y)
{
    //std::cout << x << ", "<< y <<  std::endl;

    if(Mouse_translate){
        mdl->posX+=(x-prev_Mx)/100.0;
        mdl->posY-=(y-prev_My)/100.0;

        prev_Mx = x;
        prev_My = y;
    }
    if(Mouse_rotate){
        mdl->rotateY+=(x-prev_Mx)/3.0;
        mdl->rotateX+=(y-prev_My)/3.0;

        prev_Mx = x;
        prev_My = y;
    }
}

