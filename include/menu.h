#ifndef MENU_H
#define MENU_H

#include "commons.h"
#include <sound.h>
#include<textureloader.h>
#include <GL/gl.h>
#include "font.h"
#include "level.h"
#include "sound.h"

enum e_type{NOACTION, HEADER, PAGESET, LEVELSET, UPGRADESET, SETEQUIP, QUITGAME, OPENQUIT, CLOSEQUIT, RESULTSCREEN};
enum state_p{GAMEPAGE, LANDINGPAGE, MAINMENU, LEVELSELECT, HELPPAGE, CREDITPAGE, PAUSE, LEVELEND, QUITMENU};

typedef struct v_bounds{
    vec2 center;
    vec2 bound0;
    vec2 bound1;
};

typedef struct menuElement{
    e_type elemType;
    int index;
    int lineCount;
    int lineLength;
    bool selected;
    bool outlines;
    float fontSize;
    char *text;
    vec3 textHue;
    vec3 bgHue;
    float bgAlpha;
    v_bounds bounds;
};

typedef struct page{
    state_p state;
    GLuint background;
    int totalElements;
    float alpha;
    menuElement *elements;
};



class menu
{
    public:
        menu();
        virtual ~menu();
        textureLoader* tLoad = new textureLoader();

        page *pages;
        page quitOverlay;
        font *writer;
        char** failure;

        bool paused;
        bool quit;
        bool checkQuit;

        GLvoid init(font*, level*, sound*);
        GLvoid drawPage(GLfloat, GLfloat);
        menuElement generateElement(e_type, int, int, int, vec2, vec3, float, float, char*);
        void drawElement(menuElement&);
        void hover(float, float);
        void click();
        void escPressed();
        void tick();
        void setRankColor(char);
        vec2 mousePos;
        level *levels;
        sound *sfxEngine;

        float xMin, xMax, yMin, yMax;

        int currentState;
        sound *snds2 = new sound();

    protected:

    private:
};

#endif // MENU_H
