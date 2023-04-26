#ifndef SOUND_H
#define SOUND_H

#include<SNDS/irrKlang.h>
#include<stdlib.h>
#include<iostream>


using namespace irrklang;

class sound
{
    public:
        sound();
        virtual ~sound();

        ISoundEngine *engine = createIrrKlangDevice();

        void playMusic(char *);
        void playSound(char *);
        void pauseSound(char *);
        int initSounds();


    protected:

    private:
};

#endif // SOUND_H
