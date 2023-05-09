#include "level.h"

level::level()
{
    maxEnemies = 8;
    totalLevels = 3;
}

level::~level()
{

}

void level::init(player *p, bulletpool *eB, bulletpool *pB)
{
    eTypes = 5;
    ply = p;
    eBullets = eB;
    pBullets = pB;
    eTextures = new GLuint[eTypes];
    tLoad->loadTexture("images/sprites/enemies/octopus.png", eTextures[0]);
    tLoad->loadTexture("images/sprites/enemies/piranha.png", eTextures[1]);
    tLoad->loadTexture("images/sprites/enemies/pufferfish.png", eTextures[2]);
    tLoad->loadTexture("images/sprites/enemies/turtle.png", eTextures[3]);
    tLoad->loadTexture("images/sprites/enemies/urchin.png", eTextures[4]);

    eScales = new vec2[eTypes];
    eScales[0] = vec2 {0.1,0.1};
    eScales[1] = vec2 {0.04 * (85.0/94.0),0.04};
    eScales[2] = vec2 {0.04,0.04};
    eScales[3] = vec2 {0.08*(18.0/22.0),0.08};
    eScales[4] = vec2 {0.05,0.05};

    eMaxHP = new float[eTypes];
    eMaxHP[0] = 120.0;
    eMaxHP[1] = 60.0;
    eMaxHP[2] = 80.0;
    eMaxHP[3] = 150.0;
    eMaxHP[04] = 60.0;


    levelList = new levelData[totalLevels];
    enemies = new enemy[maxEnemies];
    for(int i = 0; i < maxEnemies; i++){
        enemies[i].initEnemy(eTextures[0], 1, 1);
    }
    eData = new behaviorType[maxEnemies];
    buildLevels();
    currentLevel = -1;
    tOffset = 0.0;
}

void level::initLevel(int n,  levelData &l)
{
    l.eCount = n;
    l.highScore = 0;

    l.behs = new behaviorType[n];
    l.eTypes = new int[n];
    l.thetas = new float[n];

    l.origins = new vec2[n];
    l.mods = new vec2[n];
}

void level::buildLevels()
{
    initLevel(4, levelList[0]);

    behaviorType b1[] = {STILL, ZAG, ORBIT, ORBIT};
    int e1[] = {0,1,4,3};
    float t1[] = {0.0, PI, PI, 0.0};
    vec2 o1[] = {vec2{0.0,0.0},  vec2{-0.1,0.1},   vec2{0.0,0.1},   vec2{0.1,0.1}};
    vec2 m1[] = {vec2{0.0,0.0},  vec2{0.2,0.1},  vec2{0.2,0.2},  vec2{0.15,-0.15}};

    for(int i = 0; i < 4; i++){
        levelList[0].behs[i] = b1[i];
        levelList[0].eTypes[i] = e1[i];
        levelList[0].thetas[i] = t1[i];
        levelList[0].origins[i] = o1[i];
        levelList[0].mods[i] = m1[i];
    }

    initLevel(5, levelList[1]);

    behaviorType b2[] = {ZAG, ZAG, ZAG, ZAG, ZAG};
    int e2[] = {4,4,4,4,4};
    float t2[] = {0.0, 2.0*PI/5.0, 4.0*PI/5.0, 6.0*PI/5.0, 8.0*PI/5.0};
    vec2 o2[] = {vec2{0.0,0.2},  vec2{0.0,0.15}, vec2{0.0,0.1},  vec2{0.0,0.05},  vec2{0.0,0.25}};
    vec2 m2[] = {vec2{0.2,0.0},  vec2{0.2,0.0},  vec2{0.2,0.0},  vec2{0.2,0.0},   vec2{0.2,0.0}};

    for(int i = 0; i < 5; i++){
        levelList[1].behs[i] = b2[i];
        levelList[1].eTypes[i] = e2[i];
        levelList[1].thetas[i] = t2[i];
        levelList[1].origins[i] = o2[i];
        levelList[1].mods[i] = m2[i];
    }

}

void level::drawEnemies()
{
    if(levelLive){
        for(int i = 0; i < maxEnemies; i++){
            if(eData[i] != INACTIVE){
                enemies[i].draw();
            }
        }
    }

}

void level::drawEnemyMasks()
{
    if(levelLive){
        glColor3f(1.0, 0.0, 0.0);
        for(int i = 0; i < maxEnemies; i++){
            if(eData[i] != INACTIVE){
                enemies[i].draw();
            }
        }
        glColor3f(1.0, 1.0, 1.0);
    }
}

void level::loadLevel(int lC)
{
    eBullets->clearAll();
    tOffset = 0.0;
    bulletCycle = 0;
    levelLive = true;
    currentLevel = lC;
    levelData *l = &levelList[lC];
    for(int i = 0; i < maxEnemies; i++){
        if(i < l->eCount){
            enemies[i].tex = eTextures[l->eTypes[i]];
            enemies[i].setSize(eScales[l->eTypes[i]].x, eScales[l->eTypes[i]].y);
            eData[i] = l->behs[i];
            enemies[i].position = {l->origins[i].x, l->origins[i].y, 0.0};
            enemies[i].type = l->eTypes[i];
            enemies[i].HP = enemies[i].maxHP = eMaxHP[enemies[i].type];
        }else{
            eData[i] = INACTIVE;
        }
    }

    ply->position = vec3{0.0, -0.15, 0.0};

}

void level::tickLevel()
{
    tOffset += PI/200.0;
    if(tOffset >= 2*PI){
        tOffset -= 2*PI;
    }
    levelData *l = &levelList[currentLevel];
    for(int i = 0; i < maxEnemies; i++){
        if(i < l->eCount && eData[i] != INACTIVE){
            float updatedX = l->origins[i].x;
            float updatedY = l->origins[i].y;
            float tempX = enemies[i].position.x;
            switch(eData[i]){
            case ZAG:
                updatedX = l->mods[i].x * (sin(tOffset + l->thetas[i]))  + l->origins[i].x;
                updatedY = l->mods[i].y * (sin(tOffset + l->thetas[i]))  + l->origins[i].y;
                if(updatedX > tempX){
                    enemies[i].actionTrigger = enemies[i].WALKL;
                }else{
                    enemies[i].actionTrigger = enemies[i].WALKR;
                }
                break;
            case ORBIT:
                updatedX = l->mods[i].x * (sin(tOffset + l->thetas[i]) + l->origins[i].x);
                updatedY = l->mods[i].y * (cos(tOffset + l->thetas[i]) + l->origins[i].y);
                bool modifyOffset =  0.0 > l->mods[i].x * l->mods[i].y;
                enemies[i].theta = float(2 * int(modifyOffset) - 1) * tOffset *180.0/ PI;
                break;
            }
            enemies[i].position = {updatedX, updatedY, 0.0};

            enemies[i].actions();
            eBullets->fire(enemies[i].type, bulletCycle, vec2{enemies[i].position.x, enemies[i].position.y}, vec2{ply->position.x, ply->position.y});
        }
    }
    bulletCycle++;
    if(bulletCycle >= 200){
        bulletCycle = 0;
    }
}

int level::endLevel()
{

}
