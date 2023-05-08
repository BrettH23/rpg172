#include "font.h"

font::font()
{
    cCount = 0;
    Rotate.x = Rotate.y = Rotate.z = 0;
    position.x = position.y = position.z = 0;
    zoom = 0;
    scale = 0.1;
    kerning = 0;
}

font::~font()
{
    //dtor
}

void font::initFonts(char* file)
{
    tLoad->loadTexture(file, tex);
}

void font::buildFonts(char* str)
{

}

void font::drawFonts(int)
{

}

void font::drawLine(char* str, float startX, float startY)
{
    float scale_t = scale*0.5;
    float posX = startX;
    float posY = startY;

    for(int i = 0; i < strlen(str); i++){

        int charAsInt = int(str[i]);
        float xCoord = oneOverHex*float(charAsInt%16);
        float yCoord = oneOverHex*float(charAsInt/16);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,tex);


        glBegin(GL_QUADS);

            glTexCoord2f(xCoord,yCoord+oneOverHex);
            glVertex3f(posX-scale_t, posY-scale_t, 0.0);

            glTexCoord2f(xCoord+oneOverHex,yCoord+oneOverHex);
            glVertex3f(posX+scale_t, posY-scale_t, 0.0);

            glTexCoord2f(xCoord+oneOverHex,yCoord);
            glVertex3f(posX+scale_t, posY+scale_t, 0.0);

            glTexCoord2f(xCoord,yCoord);
            glVertex3f(posX-scale_t, posY+scale_t, 0.0);
        glEnd();
        glPopMatrix();
        posX += scale + kerning;
    }
}
