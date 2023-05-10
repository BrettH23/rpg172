#include "menu.h"

menu::menu()
{
    //ctor
    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
}

menu::~menu()
{
    //dtor
}
GLvoid menu::init(font *f)
{
    currentState = 1;
    writer = f;
    mousePos = vec2{0.0,0.0};
    pages = new page[8];

    pages[0].state = GAMEPAGE;
    pages[0].alpha = 0.0;
    pages[0].totalElements = 0;

    pages[1].state = LANDINGPAGE;
    tLoad->loadTexture("images/TitleMenu.png", pages[1].background);
    pages[1].alpha = 1.0;
    pages[1].totalElements = 2;
    pages[1].elements = new menuElement[2];
    pages[1].elements[0] = generateElement(NOACTION, 0, 8, 1, vec2{0.0,0.25}, 0.2, 0.03, "BIG POND");
    pages[1].elements[1] = generateElement(PAGESET, 0, 10, 1, vec2{0.0,-0.25}, 0.2, 0.01, "Start Game");

    pages[2].state = MAINMENU;
    tLoad->loadTexture("images/menuPage4.png", pages[2].background);
    pages[2].alpha = 1.0;
    pages[2].totalElements = 0;

    pages[3].state = LEVELSELECT;
    //tLoad->loadTexture("images/menuPage4.png", pages[3].background);
    pages[3].alpha = 1.0;
    pages[3].totalElements = 0;

    pages[4].state = HELPPAGE;
    tLoad->loadTexture("images/helpPage2.png", pages[4].background);
    pages[4].alpha = 1.0;
    pages[4].totalElements = 0;

    pages[5].state = CREDITPAGE;
    tLoad->loadTexture("images/creditsPage.png", pages[5].background);
    pages[5].alpha = 1.0;
    pages[5].totalElements = 0;

    pages[6].state = PAUSE;
    tLoad->loadTexture("images/pausePage2.png", pages[6].background);
    pages[6].alpha = 0.5;
    pages[6].totalElements = 0;

    pages[7].state = LEVELEND;
    //tLoad->loadTexture("images/pausePage2.png", pages[7].background);
    pages[7].alpha = 0.5;
    pages[7].totalElements = 0;

}

GLvoid menu::drawPage(GLfloat w , GLfloat h)
{
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, pages[currentState].background);
        glColor4f(1.0,1.0,1.0,pages[currentState].alpha);
        glBegin(GL_QUADS);
        glTexCoord2f(xMin,yMin);
        glVertex2f(-w/(2.0*h),0.5);
        glTexCoord2f(xMax,yMin);
        glVertex2f(w/(2.0*h),0.5);
        glTexCoord2f(xMax,yMax);
        glVertex2f(w/(2.0*h),-0.5);
        glTexCoord2f(xMin,yMax);
        glVertex2f(-w/(2.0*h),-0.5);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        for(int i = 0; i < pages[currentState].totalElements; i++){
            drawElement(pages[currentState].elements[i]);
        }
    glPopMatrix();
}

void menu::drawElement(menuElement& m)
{

    if(m.elemType != NOACTION){
        if(m.selected){
            glColor4f(0.0,0.8,0.9,0.5);
        }else{
            glColor4f(1.0,1.0,1.0, 0.2);
        }
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.0); glVertex2f(m.bounds.bound0.x , m.bounds.bound1.y);
            glTexCoord2f(1.0, 1.0); glVertex2f(m.bounds.bound1.x , m.bounds.bound1.y);
            glTexCoord2f(1.0, 0.0); glVertex2f(m.bounds.bound1.x , m.bounds.bound0.y);
            glTexCoord2f(0.0, 0.0); glVertex2f(m.bounds.bound0.x , m.bounds.bound0.y);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }

    glColor4f(m.textHue.x,m.textHue.y,m.textHue.z, 1.0);
    float yOffset = (m.lineCount - 1) * 0.5 * m.fontSize;
    for(int i = 0; i < m.lineCount; i++){
        writer->drawLineCentered(m.text, m.fontSize, m.bounds.center.x, m.bounds.center.y + yOffset, i*m.lineLength, m.lineLength);
        yOffset -= m.fontSize;
    }
    glColor3f(1.0,1.0,1.0);

}

menuElement menu::generateElement(e_type etype, int index, int lineLength, int lineCount, vec2 center_c, float fontSize, float padding, char* str)
{
    menuElement newElement;
    newElement.elemType = etype;
    newElement.fontSize = fontSize;
    newElement.index = index;
    newElement.lineCount = lineCount;
    newElement.lineLength = lineLength;
    newElement.text = str;
    newElement.textHue = vec3{0.0,0.0,0.0};
    newElement.selected = false;

    v_bounds newBounds;
    newBounds.center = center_c;

    float widthMod = padding + 0.5 * ((float(lineLength) * fontSize) + (float(lineLength - 1) * fontSize * (writer->kerning)));
    float heightMod = padding + 0.5 * (float(lineCount) * fontSize);
    newBounds.bound0 = {center_c.x + widthMod, center_c.y + heightMod};
    newBounds.bound1 = {center_c.x - widthMod, center_c.y - heightMod};
    newElement.bounds = newBounds;
    return newElement;
}

void menu::hover(float mouseX, float mouseY)
{
    mousePos.x = mouseX; mousePos.y = mouseY;
    for(int i = 0; i < pages[currentState].totalElements; i++){
        menuElement *temp = &pages[currentState].elements[i];
        if(temp->elemType != NOACTION && mousePos.x <= temp->bounds.bound0.x && mousePos.x >= temp->bounds.bound1.x     &&      mousePos.y <= temp->bounds.bound0.y && mousePos.y >= temp->bounds.bound1.y){
            temp->selected = true;
        }else{
            temp->selected = false;
        }
    }
}

void menu::click()
{
    for(int i = 0; i < pages[currentState].totalElements; i++){
        if(pages[currentState].elements[i].selected && pages[currentState].elements[i].elemType != NOACTION){
            pages[currentState].elements[i].selected = false;
            currentState = pages[currentState].elements[i].index;
        }
    }
}

void menu::escPressed()
{

}
