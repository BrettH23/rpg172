#include "level.h"

level::level()
{
    maxEnemies = 8;
    totalLevels = 3;
}

level::~level()
{

}

void level::init()
{
    eTextures = new GLuint[5];
    tLoad->loadTexture("images/sprites/octopus.png", eTextures[0]);
    tLoad->loadTexture("images/sprites/piranha.png", eTextures[1]);
    tLoad->loadTexture("images/sprites/pufferfish.png", eTextures[2]);
    tLoad->loadTexture("images/sprites/turtle.png", eTextures[3]);
    tLoad->loadTexture("images/sprites/urchin.png", eTextures[4]);

    eScales = new vec2[5];
    eScales[0] = vec2 {0.1,0.1};
    eScales[1] = vec2 {0.1,0.1};
    eScales[2] = vec2 {0.1,0.1};
    eScales[3] = vec2 {0.1,0.1};
    eScales[4] = vec2 {0.1,0.1};

    levelList = new levelData[totalLevels];
    enemies = new enemy[maxEnemies];
    eData = new behaviorType[maxEnemies];
    buildLevels();
    currentLevel = -1;
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

    behaviorType b1[] = {STILL, ZAG, ORBIT, STILL};
    int e1[] = {0,1,0,2};
    float t1[] = {0.0, 0.0, 0.0, 0.0};
    vec2 o1[] = {vec2{0.0,0.0},  vec2{0.0,-0.1},   vec2{0.0,0.1},   vec2{0.1,0.0}};
    vec2 m1[] = {vec2{0.0,0.0},  vec2{0.2,0.1},  vec2{0.3,0.1},  vec2{0.0,0.0}};

    for(int i = 0; i < 4; i++){
        levelList[0].behs[i] = b1[i];
        levelList[0].eTypes[i] = e1[i];
        levelList[0].thetas[i] = t1[i];
        levelList[0].origins[i] = o1[i];
        levelList[0].mods[i] = m1[i];
    }
}

void level::drawEnemies()
{
    if(levelLive){
        for(int i = 0; i < maxEnemies; i++){
            if(eData[i] != INACTIVE){
                enemies[i].draw();
                std::cout << enemies[i].verts[1].x << std::endl;
            }
        }
    }

}

void level::loadLevel(int lC)
{

    levelLive = true;
    currentLevel = lC;
    levelData *l = &levelList[lC];
    for(int i = 0; i < maxEnemies; i++){
        if(i < l->eCount){
            enemies[i].tex = eTextures[l->eTypes[i]];
            enemies[i].setSize(eScales[l->eTypes[i]].x, eScales[l->eTypes[i]].y);
            eData[i] = l->behs[i];
            enemies[i].position = {l->origins[i].x, l->origins[i].y, 0.0};

        }else{
            eData[i] = INACTIVE;
        }
    }
}

void level::tickLevel()
{
    levelData *l = &levelList[currentLevel];
    for(int i = 0; i < maxEnemies; i++){
        if(i < l->eCount && eData[i] != INACTIVE){
            enemies[i].position = {l->origins[i].x, l->origins[i].y, 0.0};
        }
    }
}

int level::endLevel()
{

}
