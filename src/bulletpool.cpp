#include "bulletpool.h"

bulletpool::bulletpool()
{
    cap = 1000;
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

    bounds[0] = vec2{(0.5*(3.0/4.0)), 0.5};
    bounds[1] = vec2{(-0.5*(3.0/4.0)), -0.5};

    bulletType[0].radius = 0.02;
    bulletType[0].alive = nullptr;

    bulletType[1].radius = 0.008;
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

void bulletpool::drawMasks(vec2 ply)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    for(int i = 0;i < typeCount;i++){
        glBindTexture(GL_TEXTURE_2D, bulletType[i].mask);
        dll* temp = bulletType[i].alive;
        while(temp!= nullptr){
            int index = temp->index;
            float xRange = 0.7*abs(bullets[index].p.x - ply.x);
            float yRange = 0.7*abs(bullets[index].p.y - ply.y);
            if(xRange < bulletType[i].radius && yRange < bulletType[i].radius){
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0,1.0);glVertex2f(bullets[index].verts[0].x,bullets[index].verts[0].y);
                    glTexCoord2f(1.0,1.0);glVertex2f(bullets[index].verts[1].x,bullets[index].verts[1].y);
                    glTexCoord2f(1.0,0.0);glVertex2f(bullets[index].verts[2].x,bullets[index].verts[2].y);
                    glTexCoord2f(0.0,0.0);glVertex2f(bullets[index].verts[3].x,bullets[index].verts[3].y);
                glEnd();
            }

            temp = temp->next;
        }
    }
    glPopMatrix();
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
            bullets[index].verts[i].x = tempX*cos(ang) + tempY*sin(ang) + p.x;
            bullets[index].verts[i].y = -tempX*sin(ang) + tempY*cos(ang) + p.y;

        }
        bullets[index].ticks = life;
        bullets[index].a.x = accel*sin(ang);//std::cout << bullets[index].a.x << std::endl;
        bullets[index].a.y = accel*cos(ang);//std::cout << bullets[index].a.y << std::endl;
        bullets[index].v.x = vel*sin(ang);
        bullets[index].v.y = vel*cos(ang);
        bullets[index].p = p;


    }
}

void bulletpool::die(dll* bIndex)
{
    if(bIndex!=nullptr){
        bool wasFirst = false;
        for(int i = 0;i < typeCount; i++){
            if(bIndex == bulletType[i].alive){
                bulletType[i].alive = bulletType[i].alive->next;
                if(bulletType[i].alive!=nullptr){
                    bulletType[i].alive->prev = nullptr;
                }
            wasFirst = true;
            }
        }
        if(!wasFirst){
            if(bIndex->prev!=nullptr){
                bIndex->prev->next = bIndex->next;
            }if(bIndex->next!=nullptr){
                bIndex->next->prev = bIndex->prev;
            }
        }
        bIndex->prev = nullptr;
        bIndex->next = dead;
        dead = bIndex;
    }

}
bool bulletpool::checkBounds(int index, int thisType)
{
    bullet* b = &bullets[index];
    float r = 1.5*bulletType[thisType].radius;
    return (b->p.x <= bounds[0].x + r) && (b->p.y <= bounds[0].y + r) && (b->p.x >= bounds[1].x - r) && (b->p.y >= bounds[1].y - r);
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
            bullets[temp->index].ticks--;
            //bullets[temp->index].ticks <= 0
            if(!checkBounds(temp->index, i)){
                dll* temp2 = temp;
                temp = temp->next;
                die(temp2);
            }else{
                temp = temp->next;
            }

        }
    }
}
float fireAngle = 0.0;
void bulletpool::fire(int cycle)
{
    vec2 p = vec2{0.2,0.1};
    //vec2 v = vec2{0,0};
    //vec2 a = vec2{0,0};
    spawn(0, 1000, 0.0025, 0.0000, fireAngle, p);
    spawn(0, 1000, -0.0025, 0.0000, fireAngle, p);
    fireAngle += 0.088*PI;
    if(fireAngle >= 2*PI){
        fireAngle -= 2*PI;
    }
}
void bulletpool::aimed(int cycle, vec2 ply)
{
    if(cycle > 20 && cycle <= 27){
        vec2 p = vec2{-0.2, 0.1};
        float theta = atan((ply.x - p.x)/ (ply.y - p.y));
        if(p.y > ply.y){
            theta += PI;
        }
        for(int i = 0; i < 20; i++){
            float rand1 = (2*float(rand())/float(RAND_MAX) - 1)*0.18;
            float rand2 = (2*float(rand())/float(RAND_MAX) - 1)*0.003;
            int rand3 = rand()%4;
            if(rand3 > 1) rand3 = 1;
            spawn(rand3, 1000, 0.004+ rand2, 0.0001, theta + rand1, p);
        }

    }

}






