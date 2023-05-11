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
    delay = 0;
    progress = 0;
    eTypes = 5;
    totalHits = 0;
    currentScore = 0;
    finalRank = 'F';
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
    eMaxHP[4] = 60.0;


    levelList = new levelData[totalLevels];
    enemies = new enemy[maxEnemies];
    for(int i = 0; i < maxEnemies; i++){
        enemies[i].initEnemy(eTextures[0], 1, 1);
    }
    eData = new behaviorType[maxEnemies];
    buildLevels();
    currentLevel = -1;
    tOffset = 0.0;
    finalTime = 0.0;
    levelStart = clock();
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
    levelList[0].scoreRank = 'F';
    levelList[0].highScore = 0;
    behaviorType b1[] = {STILL, ZAG, ORBIT, ORBIT};
    int e1[] = {0,1,4,3};
    float t1[] = {0.0, PI, 0.5, 0.0};
    vec2 o1[] = {vec2{0.0,0.0},  vec2{-0.1,0.1},   vec2{0.0,0.1},   vec2{0.1,0.1}};
    vec2 m1[] = {vec2{0.0,0.0},  vec2{0.2,0.1},  vec2{0.2,0.2},  vec2{0.15,0.15}};

    for(int i = 0; i < 4; i++){
        levelList[0].behs[i] = b1[i];
        levelList[0].eTypes[i] = e1[i];
        levelList[0].thetas[i] = t1[i];
        levelList[0].origins[i] = o1[i];
        levelList[0].mods[i] = m1[i];
    }
    levelList[0].par = 30;


    initLevel(5, levelList[1]);
    levelList[1].scoreRank = 'F';
    levelList[1].highScore = 0;

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
    levelList[1].par = 30;
}

void level::drawEnemies()
{

    for(int i = 0; i < maxEnemies; i++){
        if(eData[i] != INACTIVE){
            enemies[i].draw();
        }
    }


}

void level::drawEnemyMasks()
{
    if(levelLive){

        for(int i = 0; i < maxEnemies; i++){
            if(eData[i] != INACTIVE){
                glColor3f(1.0, 0.0, 0.0);
                enemies[i].draw();
            }
        }
        glColor3f(1.0, 1.0, 1.0);
    }
}

void level::loadLevel(int lC)
{
    delay = 600;
    eBullets->clearAll();
    pBullets->clearAll();
    tOffset = 0.0;
    bulletCycle = 0;
    levelLive = true;
    levelEnding = false;
    victory = false;
    currentLevel = lC;
    maxKillScore = 0;
    levelData *l = &levelList[lC];
    for(int i = 0; i < maxEnemies; i++){
        if(i < l->eCount){
            enemies[i].tex = eTextures[l->eTypes[i]];
            enemies[i].setSize(eScales[l->eTypes[i]].x, eScales[l->eTypes[i]].y);
            eData[i] = l->behs[i];
            enemies[i].position = {l->origins[i].x, l->origins[i].y + 100.0, 0.0};
            enemies[i].type = l->eTypes[i];
            enemies[i].HP = enemies[i].maxHP = eMaxHP[enemies[i].type];
            enemies[i].actionTrigger = enemies[i].IDLE;
            maxKillScore += 100* enemies[i].maxHP;
        }else{
            eData[i] = INACTIVE;
        }
    }

    ply->position = vec3{0.0, -0.15, 0.0};
    ply->actions(ply->WALKR);
    ply->HP = ply->maxHP;
    ply->invul = 50;
    totalHits = 0;
    currentScore = 0;
    levelStart = clock();
}

void level::tickLevel()
{
    tOffset += PI/200.0;
    if(tOffset >= 2*PI){
        tOffset -= 2*PI;
    }
    if(!levelLive){
        return;
    }
    levelData *l = &levelList[currentLevel];
    if(ply->firing){
        pBullets->playerFire(0, bulletCycle, vec2{ply->position.x, ply->position.y + ply->sizeRadius.y});
    }
    int deadEnemies = 0;
    for(int i = 0; i < maxEnemies; i++){
        if(i < l->eCount && eData[i] != INACTIVE &&enemies[i].actionTrigger != enemies[i].DEAD){

            float updatedX = l->origins[i].x;
            float updatedY = l->origins[i].y;
            float tempX = enemies[i].position.x;
            switch(eData[i]){
            case ZAG:
                updatedX = l->mods[i].x * (sin(tOffset + l->thetas[i]))  + l->origins[i].x;
                updatedY = l->mods[i].y * (sin(tOffset + l->thetas[i]))  + l->origins[i].y;
                break;
            case ORBIT:
                updatedX = l->mods[i].x * (sin(tOffset + l->thetas[i]) + l->origins[i].x);
                updatedY = l->mods[i].y * (cos(tOffset + l->thetas[i]) + l->origins[i].y);
                bool modifyOffset =  0.0 > l->mods[i].x * l->mods[i].y;
                //enemies[i].theta = float(2 * int(modifyOffset) - 1) * tOffset *180.0/ PI;
                break;
            }
            if(updatedX > tempX){
                enemies[i].actions(enemies[i].WALKL);
            }else{
                enemies[i].actions(enemies[i].WALKR);
            }
            enemies[i].position = {updatedX, updatedY, 0.0};
            if(delay > 0){
                enemies[i].position.y+= 0.6 * (1-cos((PI/2.0) * (float(delay)/400.0)));
                delay--;
            }else{
                eBullets->fire(enemies[i].type, bulletCycle, vec2{enemies[i].position.x, enemies[i].position.y}, vec2{ply->position.x, ply->position.y});
            }


            float damageTotal = ply->attack * pBullets->getDamage(vec2{enemies[i].position.x, enemies[i].position.y}, enemies[i].sizeRadius);
            if(damageTotal > 0.0){
                enemies[i].hit(damageTotal);
                if(enemies[i].actionTrigger == enemies[i].DEAD){
                    currentScore += 100 * enemies[i].maxHP;
                }
            }
        }
        if(eData[i] == INACTIVE || enemies[i].actionTrigger == enemies[i].DEAD){
            deadEnemies++;
        }
        finalTime = float(clock() - levelStart)/float(CLOCKS_PER_SEC);
    }

    if(ply->actionTrigger == ply->DEAD){
        endLevel(false);
    }
    if(deadEnemies == maxEnemies){
        victory = true;
        endLevel(true);
    }

    bulletCycle++;
    if(bulletCycle >= 200){
        bulletCycle = 0;
    }
}


int level::endLevel(bool win)
{
    if(!levelEnding){
        ply->invul = 200;
        levelEnding = true;
        endDelay = 200;
        if(win){
            float timePenalty = (finalTime/levelList[currentLevel].par) - 1.0;
            float timeBonus;
            if(timePenalty <= 0){
                timeBonus = 1.0;
            }else if(timePenalty >= 1){
                timeBonus = 0;
            }else{
                timeBonus = floor(100.0*(1.0 - timePenalty)) / 100.0;
            }
            float hitBonus = 0;;
            switch(totalHits){
            case 0:
                hitBonus = 1.0;
                break;
            case 1:
                hitBonus = 0.7;
                break;
            case 2:
                hitBonus = 0.4;
                break;
            }

            finalTimeBonus = int(maxKillScore * timeBonus/2);
            finalHitBonus = int(maxKillScore * hitBonus / 2);

            finalScore = currentScore + finalTimeBonus + finalHitBonus;
            float rankCheck = float(finalScore) / float(maxKillScore * 2);
            if(rankCheck >= 1.0){
                finalRank = 'P';
            }else if(rankCheck >= 0.95){
                finalRank = 'S';
            }else if(rankCheck >= 0.85){
                finalRank = 'A';
            }else if(rankCheck >= 0.75){
                finalRank = 'B';
            }else if(rankCheck >=0.55){
                finalRank = 'C';
            }else{
                finalRank = 'D';
            }
            if(finalScore > levelList[currentLevel].highScore){
                levelList[currentLevel].highScore = finalScore;
                levelList[currentLevel].scoreRank = finalRank;
            }
        }

    }else{
        if(endDelay <= 0){
            levelLive = false;
        }
        endDelay--;
    }


}
