#ifndef LEVEL_H
#define LEVEL_H

typedef struct levelData{
    int points;
    int totalEnemies;
    int liveEnemies;

    int* enemyTypes;
    int* fireTypes;
    int* deathThroes;

};

class level
{
    public:
        level();
        virtual ~level();

        void loadLevel();

    protected:

    private:
};

#endif // LEVEL_H
