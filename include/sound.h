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
        void pop();
        int initMusic();
        void tickSounds();
        void setTrack(int);
        char** tracks;
        int currentTrack;
        ISound *music;
        char** sfx;
        ISoundSource* popSound;


    protected:

    private:
};

#endif // SOUND_H
