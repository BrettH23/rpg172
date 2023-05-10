#ifndef MENU_H
#define MENU_H

#include "commons.h"
#include<textureloader.h>
#include <GL/gl.h>
#include "font.h"

enum e_type{NOACTION, PAGESET, LEVELSET, UPGRADESET, SETEQUIP, QUIT};
enum state_p{GAMEPAGE, LANDINGPAGE, MAINMENU, LEVELSELECT, HELPPAGE, CREDITPAGE, PAUSE, LEVELEND};

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
    float fontSize;
    char *text;
    vec3 textHue;
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
        font *writer;

        bool gameActive;

        GLvoid init(font*);
        GLvoid drawPage(GLfloat, GLfloat);
        menuElement generateElement(e_type, int, int, int, vec2, float, float, char*);
        void drawElement(menuElement&);
        void hover(float, float);
        void click();
        void escPressed();
        vec2 mousePos;

        float xMin, xMax, yMin, yMax;

        int currentState;

    protected:

    private:
};

#endif // MENU_H
