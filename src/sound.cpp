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
    if(!engine->isCurrentlyPlaying(File)){
        engine->play2D(File,true);
    }

}

void sound::playSound(char* File)
{
    if(!engine->isCurrentlyPlaying(File)){
        ISound* sou = engine->play2D(File,false,false);
        if (sou){
            //sou->setVolume(0.5);
        }

    }

}

void sound::pauseSound(char* File)
{
    engine->play2D(File,true,false);
}

void sound::tickSounds()
{
    if(!engine->isCurrentlyPlaying(tracks[currentTrack])){
        engine->play2D(tracks[currentTrack],true);
    }
}

void sound::setTrack(int newTrack)
{
    if(newTrack != currentTrack){
        engine->stopAllSounds();
        currentTrack = newTrack;
        engine->play2D(tracks[currentTrack],true);
    }

}

void sound::pop()
{
    engine->play2D(popSound);
}


int sound::initMusic()
{
    popSound = engine->addSoundSourceFromFile("sounds/bubblePop.mp3");
    popSound->setDefaultVolume(0.18f);
    tracks = new char*[6];
    tracks[0] = "sounds/level1.mp3";
    tracks[1] = "sounds/level2.mp3";
    tracks[2] = "sounds/level3.mp3";
    tracks[3] = "sounds/level4.mp3";
    tracks[4] = "sounds/level5.mp3";
    tracks[5] = "sounds/menu.mp3";
    currentTrack = 5;
    if(!engine)
    {
       return 0;
    }
    return 1;
}

