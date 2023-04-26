#include "bulletpool.h"

bulletpool::bulletpool()
{
    cap = 3;
    bullets = new bullet[cap];
    typeCount = 2;
    bulletType = new type_b[typeCount];
    dead = nullptr;
    for(int i = 0;i < cap;i++){
        dll* temp = new dll;
        temp->prev = nullptr;
        temp->next = dead;
        temp->index = i;
        if(dead != nullptr){
            dead->prev = temp;
        }
        dead = temp;
    }


    bulletType[0].radius = 0.05;
    bulletType[0].alive = nullptr;


    bulletType[1].radius = 0.02;
    bulletType[1].alive = nullptr;
}

void bulletpool::texInit()
{
    tLoad->loadTexture("images/bullets/bullet1.png", bulletType[0].tex);
    tLoad->loadTexture("images/bullets/mask1.png", bulletType[0].mask);
    tLoad->loadTexture("images/bullets/bullet2.png", bulletType[1].tex);
    tLoad->loadTexture("images/bullets/mask2.png", bulletType[1].mask);
}


bulletpool::~bulletpool()
{
    //dtor
}
void bulletpool::draw()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    for(int i = 0;i < typeCount;i++){
        glBindTexture(GL_TEXTURE_2D, bulletType[i].tex);
        dll* temp = bulletType[i].alive;
        while(temp!= nullptr){
            int index = temp->index;
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,1.0);glVertex2f(bullets[index].verts[0].x,bullets[index].verts[0].y);
                glTexCoord2f(1.0,1.0);glVertex2f(bullets[index].verts[1].x,bullets[index].verts[1].y);
                glTexCoord2f(1.0,0.0);glVertex2f(bullets[index].verts[2].x,bullets[index].verts[2].y);
                glTexCoord2f(0.0,0.0);glVertex2f(bullets[index].verts[3].x,bullets[index].verts[3].y);
            glEnd();
            temp = temp->next;
        }
    }
    glPopMatrix();
}

void bulletpool::fire(int cycle)
{
    vec2 p = vec2{(0.2*float(cycle)),0.1};
    //vec2 v = vec2{0,0};
    //vec2 a = vec2{0,0};
    spawn(0, 10, -0.008, 0.0001, 0.0, p);
}

void bulletpool::spawn(int type, int life, float vel, float accel, float ang, vec2 p)
{
    if(dead!=nullptr){

        dll* temp = dead;
        dead = dead->next;
        if(dead!=nullptr){
            dead->prev = nullptr;
        }
        temp->prev = nullptr;
        temp->next = bulletType[type].alive;
        //std::cout << "Hereiam" << std::endl;
        if(bulletType[type].alive != nullptr){
            bulletType[type].alive->prev = temp;
        }
        bulletType[type].alive = temp;
        int index = temp->index;
        float rad = bulletType[type].radius;
        bullets[index].verts[0] = vec2{-rad,-rad};
        bullets[index].verts[1] = vec2{rad,-rad};
        bullets[index].verts[2] = vec2{rad,rad};
        bullets[index].verts[3] = vec2{-rad,rad};

        for(int i = 0;i < 4;i++){
            float tempX = bullets[index].verts[i].x;
            float tempY = bullets[index].verts[i].y;
            bullets[index].verts[i].x = tempX*cos(ang) - tempY*sin(ang) + p.x;
            bullets[index].verts[i].y = tempX*sin(ang) + tempY*cos(ang) + p.y;

        }
        bullets[index].ticks = life;
        bullets[index].a.x = accel*sin(ang);//std::cout << bullets[index].a.x << std::endl;
        bullets[index].a.y = accel*cos(ang);//std::cout << bullets[index].a.y << std::endl;
        bullets[index].v.x = vel*sin(ang);
        bullets[index].v.y = vel*cos(ang);
        bullets[index].p = p;


    }
}

void bulletpool::die(dll bIndex)
{

}

void bulletpool::tick()
{
    for(int i = 0;i < typeCount; i++){
        dll* temp = bulletType[i].alive;
        while(temp!= nullptr){

            //std::cout << bullets[temp->index].v.y << std::endl;
            bullets[temp->index].p.x += bullets[temp->index].v.x;
            bullets[temp->index].p.y += bullets[temp->index].v.y;
            bullets[temp->index].v.x += bullets[temp->index].a.x;
            bullets[temp->index].v.y += bullets[temp->index].a.y;
            for(int j = 0;j < 4;j++){
                bullets[temp->index].verts[j].x += bullets[temp->index].v.x;
                bullets[temp->index].verts[j].y += bullets[temp->index].v.y;
            }
            temp = temp->next;
        }
    }
}


