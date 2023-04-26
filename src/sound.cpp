#include "sound.h"

sound::sound()
{
    //ctor
}

sound::~sound()
{
    //dtor
    engine->drop();
}

void sound::playMusic(char* File)
{
    engine->play2D(File,true);
}

void sound::playSound(char* File)
{
    if(!engine->isCurrentlyPlaying(File))
    engine->play2D(File,false,false);
}

void sound::pauseSound(char* File)
{
    engine->play2D(File,true,false);
}

int sound::initSounds()
{
    if(!engine)
    {
       return 0;
    }
    return 1;
}

