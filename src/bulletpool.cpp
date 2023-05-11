#include "bulletpool.h"

bulletpool::bulletpool()
{

    bounds[0] = vec2{0.5, 0.5};
    bounds[1] = vec2{-0.5, -0.5};

}

bulletpool::~bulletpool()
{
    //dtor
}

void bulletpool::initE(int c)
{
    poolType = ENEMY;
    cap = c;
    bullets = new bullet[cap];
    typeCount = 6;
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

    fireAngle = 0.0;


    tLoad->loadTexture("images/sprites/bullets/tentacleBullet.png", bulletType[0].tex);
    //tLoad->loadTexture("images/sprites/bullets/tentacleBullet.png", bulletType[0].mask);
    tLoad->loadTexture("images/sprites/bullets/crabBullet.png", bulletType[1].tex);
    //tLoad->loadTexture("images/sprites/bullets/crabBullet.png", bulletType[1].mask);
    tLoad->loadTexture("images/sprites/bullets/enemyBullets.png", bulletType[2].tex);
    tLoad->loadTexture("images/sprites/bullets/spikeBullet.png", bulletType[3].tex);

    bulletType[0].radius = 0.021;
    bulletType[0].alive = nullptr;
    bulletType[0].damage = 1.0;
    bulletType[0].hue = vec3{1.0, 0.5, 0.8};

    bulletType[1].radius = 0.008;
    bulletType[1].alive = nullptr;
    bulletType[1].damage = 1.0;
    bulletType[1].hue = vec3{1.0, 0.7, 0.4};

    bulletType[2].radius = 0.008;
    bulletType[2].alive = nullptr;
    bulletType[2].damage = 1.0;
    bulletType[2].hue = vec3{1.0, 0.7, 0.4};

    bulletType[3].radius = 0.008;
    bulletType[3].alive = nullptr;
    bulletType[3].damage = 1.0;
    bulletType[3].hue = vec3{1.0, 0.7, 0.4};

    bulletType[4].radius = 0.008;
    bulletType[4].alive = nullptr;
    bulletType[4].damage = 1.0;
    bulletType[4].hue = vec3{1.0, 0.7, 0.4};

    bulletType[5].tex = bulletType[0].tex;
    bulletType[5].radius = 0.03;
    bulletType[5].alive = nullptr;
    bulletType[5].damage = 1.0;
    bulletType[5].hue = vec3{1.0, 0.1, 0.0};


}

void bulletpool::initP(int c)
{
    poolType = PLAYER;
    cap = c;
    bullets = new bullet[cap];
    typeCount = 1;
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

    fireAngle = 0.0;


    tLoad->loadTexture("images/sprites/bullets/playerBullet.png", bulletType[0].tex);
    tLoad->loadTexture("images/sprites/bullets/playerBullet.png", bulletType[0].mask);

    bulletType[0].radius = 0.01;
    bulletType[0].alive = nullptr;
    bulletType[0].damage = 1.0;
    bulletType[0].hue = vec3{1, 1.0, 1};
        //bulletType[0].hue = vec3{0.6, 1.0, 0.85};

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
            float alpha = 1.0;
            if(bullets[index].ticks > 0){
                if(bullets[index].ticks < 30){
                    alpha = float(bullets[index].ticks + 7)/30.0;
                }
                glColor4f(bulletType[i].hue.x,bulletType[i].hue.y,bulletType[i].hue.z,alpha);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0,1.0);glVertex2f(bullets[index].verts[0].x,bullets[index].verts[0].y);
                    glTexCoord2f(1.0,1.0);glVertex2f(bullets[index].verts[1].x,bullets[index].verts[1].y);
                    glTexCoord2f(1.0,0.0);glVertex2f(bullets[index].verts[2].x,bullets[index].verts[2].y);
                    glTexCoord2f(0.0,0.0);glVertex2f(bullets[index].verts[3].x,bullets[index].verts[3].y);
                glEnd();

            }else{
                float mod = -float(bullets[index].ticks - 1)/4.0;
                float mod2 = mod * bulletType[i].radius;
                glColor4f(1.0,1.0,1.0,mod);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0,1.0);glVertex2f(bullets[index].verts[0].x - mod2,bullets[index].verts[0].y - mod2);
                    glTexCoord2f(1.0,1.0);glVertex2f(bullets[index].verts[1].x + mod2,bullets[index].verts[1].y - mod2);
                    glTexCoord2f(1.0,0.0);glVertex2f(bullets[index].verts[2].x + mod2,bullets[index].verts[2].y + mod2);
                    glTexCoord2f(0.0,0.0);glVertex2f(bullets[index].verts[3].x - mod2,bullets[index].verts[3].y + mod2);
                glEnd();
            }
            temp = temp->next;
        }
    }
    glColor4f(1.0,1.0,1.0,1.0);
    glPopMatrix();
}

void bulletpool::drawMasks(vec2 plyPos)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0,0.0,0.0);
    for(int i = 0;i < typeCount;i++){
        glBindTexture(GL_TEXTURE_2D, bulletType[i].tex);
        dll* temp = bulletType[i].alive;
        while(temp!= nullptr){
            int index = temp->index;
            if(bullets[index].ticks > 0){
                float xRange = 0.7*abs(bullets[index].p.x - plyPos.x);
                float yRange = 0.7*abs(bullets[index].p.y - plyPos.y);
                if(xRange < bulletType[i].radius && yRange < bulletType[i].radius){
                    glBegin(GL_QUADS);
                        glTexCoord2f(0.0,1.0);glVertex2f(bullets[index].verts[0].x,bullets[index].verts[0].y);
                        glTexCoord2f(1.0,1.0);glVertex2f(bullets[index].verts[1].x,bullets[index].verts[1].y);
                        glTexCoord2f(1.0,0.0);glVertex2f(bullets[index].verts[2].x,bullets[index].verts[2].y);
                        glTexCoord2f(0.0,0.0);glVertex2f(bullets[index].verts[3].x,bullets[index].verts[3].y);
                    glEnd();
                }
            }
            temp = temp->next;
        }
    }
    glColor3f(1.0,1.0,1.0);
    glPopMatrix();
}

void bulletpool::spawn(int type, int life, float vel, float accel, float ang, float angVel, vec2 p)
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
        bullets[index].angVel = angVel;
        bullets[index].angle = ang;


    }
}

void bulletpool::die(int a, dll* bIndex)
{
    if(bIndex!=nullptr){

        if(bIndex->prev!=nullptr){
            bIndex->prev->next = bIndex->next;
        }

        if(bIndex->next!=nullptr){
            bIndex->next->prev = bIndex->prev;
        }

        if(bIndex == bulletType[a].alive){
            bulletType[a].alive = bulletType[a].alive->next;
        }

        if(bulletType[a].alive!=nullptr){
            bulletType[a].alive->prev = nullptr;
        }

        bIndex->prev = nullptr;
        bIndex->next = dead;
        dead = bIndex;
    }

}
void bulletpool::clearAll()
{
    for(int i = 0;i < typeCount; i++){
        dll* temp = bulletType[i].alive;
        while(temp!= nullptr){
            dll* temp2 = temp->next;
            die(i, temp);
            temp = temp2;
        }
    }
}


bool bulletpool::checkBounds(int index, int thisType)
{
    bullet* b = &bullets[index];
    float r = 1.5*bulletType[thisType].radius;
    return (b->p.x <= bounds[0].x + r) && (b->p.y <= bounds[0].y + r) && (b->p.x >= bounds[1].x - r) && (b->p.y >= bounds[1].y - r);
}

float bulletpool::getDamage(vec2 origin, vec2 sz)
{
    float impacts = 0.0;
    vec2 b0 = vec2{origin.x + sz.x, origin.y + sz.y};
    vec2 b1 = vec2{origin.x - sz.x, origin.y - sz.y};
    for(int i = 0;i < typeCount; i++){
        dll* temp = bulletType[i].alive;
        float r = bulletType[i].radius;
        while(temp!= nullptr){
            bullet* b = &bullets[temp->index];
            bool test = sqrt(pow(b->p.x - origin.x, 2)+ pow(b->p.y - origin.y,2))<r + sz.x;
            if(test && b->ticks > 0){
                impacts+= bulletType[i].damage;
                bullets[temp->index].ticks = 0;
            }
            temp = temp->next;

        }
    }
    return impacts;
}


void bulletpool::tick()
{
    fireAngle += 0.025*PI;
    if(fireAngle >= 2*PI){
        fireAngle -= 2*PI;
    }
    for(int i = 0;i < typeCount; i++){
        dll* temp = bulletType[i].alive;
        while(temp!= nullptr){
            if(bullets[temp->index].ticks > 0){
                //std::cout << bullets[temp->index].v.y << std::endl;
                bullets[temp->index].p.x += bullets[temp->index].v.x;
                bullets[temp->index].p.y += bullets[temp->index].v.y;
                bullets[temp->index].v.x += bullets[temp->index].a.x;
                bullets[temp->index].v.y += bullets[temp->index].a.y;
                if(bullets[temp->index].angVel != 0.0){
                    bullets[temp->index].angle+= bullets[temp->index].angVel;
                    float ang = bullets[temp->index].angle;
                    vec2 p = bullets[temp->index].p;
                    float rad = bulletType[i].radius;
                    bullets[temp->index].verts[0] = vec2{-rad,-rad};
                    bullets[temp->index].verts[1] = vec2{rad,-rad};
                    bullets[temp->index].verts[2] = vec2{rad,rad};
                    bullets[temp->index].verts[3] = vec2{-rad,rad};

                    for(int j = 0;j < 4;j++){
                        float tempX = bullets[temp->index].verts[j].x;
                        float tempY = bullets[temp->index].verts[j].y;
                        bullets[temp->index].verts[j].x = tempX*cos(ang) + tempY*sin(ang) + p.x;
                        bullets[temp->index].verts[j].y = -tempX*sin(ang) + tempY*cos(ang) + p.y;
                    }

                }
                for(int j = 0;j < 4;j++){
                    bullets[temp->index].verts[j].x += bullets[temp->index].v.x;
                    bullets[temp->index].verts[j].y += bullets[temp->index].v.y;
                }

            }
            bullets[temp->index].ticks--;
            //bullets[temp->index].ticks <= 0
            if(bullets[temp->index].ticks<= -3 || !checkBounds(temp->index, i)){
                dll* temp2 = temp;
                temp = temp->next;
                die(i, temp2);
            }else{
                temp = temp->next;
            }

        }
    }
}

void bulletpool::playerFire(int type, int cycle, vec2 plyPos)
{
    float randTheta = (2*float(rand())/float(RAND_MAX) - 1)*0.085;
    spawn(type, 50, 0.01, 0.0, randTheta, 0.0, plyPos);
}


void bulletpool::fire(int type, int cycle, vec2 origin, vec2 plyPos, float health)
{
    switch(type){
    case 0:
        fireOcto(cycle, origin, plyPos, health);
        break;
    case 1:
        aimed(cycle,origin, plyPos);
        break;

    }

}
float doomMod = 1.0;

void bulletpool::fireOcto(int cycle, vec2 origin, vec2 player, float health)
{
    int typeMod = 0;
    if(health <= 0.3){
        typeMod = 5;
    }
    if((cycle) % 40 == 7){
        float mainOffset = PI * 0.05 * float(cycle / 40);
        float cycleIterator = 0.0;
        for(int i = 0;i < 8; i++){
            spawn(typeMod, 1000, -0.0015, 0.0000, mainOffset + cycleIterator, 0.0, origin);
            cycleIterator+= PI*0.25;
        }

    }
    if(health <=0.6 && (cycle) % 40 == 27){
        float mainOffset = -PI * 0.05 * float(cycle / 40);
        float cycleIterator = 0.0;
        for(int i = 0;i < 8; i++){
            spawn(typeMod, 1000, -0.0015, 0.0000, mainOffset + cycleIterator, 0.0, origin);
            cycleIterator+= PI*0.25;
        }
    }
}


void bulletpool::doomSpiral(int cycle, vec2 origin)
{
    if((10+cycle)%10 == 0){

        //vec2 v = vec2{0,0};
        //vec2 a = vec2{0,0};
        float cycleIterator = 0.0;
        for(int i = 0;i < 5; i++){
            spawn(0, 1000, -0.0015, 0.0000, float(doomMod)*fireAngle + cycleIterator, 0.0, origin);
            cycleIterator+= PI*0.4;
        }


    }

    //spawn(0, 1000, -0.0015, 0.0000, fireAngle + PI, 0.0, p);

}


void bulletpool::aimed(int cycle, vec2 origin, vec2 plyPos)
{
    if(cycle > 20 && cycle <= 40 ){
        float theta = PI + atan((plyPos.x - origin.x)/ (plyPos.y - origin.y));
        if(origin.y > plyPos.y){
            theta += PI;
        }
        for(int i = 0; i < 20; i++){
            float rand1 = (2*float(rand())/float(RAND_MAX) - 1)*0.18;
            float rand2 = (2*float(rand())/float(RAND_MAX) - 1)*0.0015;
            //int rand3 = rand()%4;
            //if(rand3 > 1) rand3 = 1;
            spawn(1, 1000, -0.002+ rand2, -0.00008, theta + rand1, 0.15, origin);
        }

    }

}






