#ifndef LEVEL_H
#define LEVEL_H

#include "commons.h"
#include "enemy.h"
#include "player.h"
#include "bulletpool.h"
#include "textureloader.h"
#include "sound.h"

enum behaviorType{STILL, ZAG, ORBIT, FALL, INACTIVE};


typedef struct levelData{
    char scoreRank;
    int highScore;
    int eCount;
    int *eTypes;
    float *thetas;
    float par;
    behaviorType *behs;
    vec2 *origins;
    vec2 *mods;
};

class level
{
    public:
        level();
        virtual ~level();

        void init(player*, bulletpool*, bulletpool*, sound*);
        void initLevel(int, levelData&);
        void buildLevels();
        void loadLevel(int);
        void tickLevel();
        void drawEnemies();
        void drawEnemyMasks();
        float getTime(bool);
        int endLevel(bool);

        bool levelLive;
        bool levelEnding;
        bool victory;

        int currentLevel;
        int maxEnemies;
        int totalLevels;
        int bulletCycle;
        int progress;
        int delay;
        int endDelay;

        char finalRank;

        clock_t levelStart;

        float finalTime;
        int currentScore;
        int maxKillScore;
        int totalHits;

        int finalScore;
        int finalTimeBonus;
        int finalHitBonus;

        levelData *levelList;

        enemy *enemies;
        behaviorType *eData;
        float tOffset;
        sound *sfxEngine;

        player *ply;
        bulletpool *eBullets;
        bulletpool *pBullets;

        GLuint* eTextures;
        vec2* eScales;
        float* eMaxHP;
        int enemyTypes;

        textureLoader *tLoad = new textureLoader();


    protected:

    private:
};

#endif // LEVEL_H
