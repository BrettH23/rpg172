#ifndef LEVEL_H
#define LEVEL_H

#include "commons.h"
#include "enemy.h"
#include "player.h"
#include "bulletpool.h"
#include "textureloader.h"

enum behaviorType{STILL, ZAG, ORBIT, INACTIVE};


typedef struct levelData{
    int highScore;
    int eCount;

    int *eTypes;
    float *thetas;
    behaviorType *behs;
    vec2 *origins;
    vec2 *mods;
};

class level
{
    public:
        level();
        virtual ~level();

        void init();
        void initLevel(int, levelData&);
        void buildLevels();
        void loadLevel(int);
        void tickLevel();
        void drawEnemies();
        int endLevel();

        bool levelLive;
        int currentLevel;
        int maxEnemies;
        int totalLevels;


        levelData *levelList;

        enemy *enemies;
        behaviorType *eData;

        player *ply;
        bulletpool *eBullets;
        bulletpool *pBullets;

        GLuint* eTextures;
        vec2* eScales;

        textureLoader *tLoad = new textureLoader();


    protected:

    private:
};

#endif // LEVEL_H
