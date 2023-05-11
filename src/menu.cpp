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
GLvoid menu::init(font *f, level *l, sound* soundGuy)
{
    quit = false;
    checkQuit = false;
    currentState = 1;
    writer = f;
    mousePos = vec2{0.0,0.0};
    pages = new page[8];
    paused = true;
    vec3 black = vec3{0.0,0.0,0.0};
    vec3 white = vec3{1.0,1.0,1.0};
    levels = l;
    sfxEngine = soundGuy;

    quitOverlay.alpha = 0.0;
    quitOverlay.state = QUITMENU;
    quitOverlay.totalElements = 3;
    quitOverlay.elements = new menuElement[3];
    quitOverlay.elements[2] = generateElement(QUITGAME, 0, 3, 1, vec2{-0.12,-0.05}, black, 0.08, 0.01, "YES");
    quitOverlay.elements[1] = generateElement(CLOSEQUIT, 0, 3, 1, vec2{0.12,-0.05}, black, 0.08, 0.01, "NO");
    quitOverlay.elements[1].lineLength = 2;
    quitOverlay.elements[0] = generateElement(NOACTION, 0, 11, 3, vec2{0.0,0.0}, white, 0.08, 0.02, "   QUIT?                         ");
    quitOverlay.elements[0].bgHue = black;
    quitOverlay.elements[0].bgAlpha = 1.0;


    pages[0].state = GAMEPAGE;
    pages[0].alpha = 0.0;
    pages[0].totalElements = 0;

    pages[1].state = LANDINGPAGE;
    tLoad->loadTexture("images/p3.jpeg", pages[1].background);
    pages[1].alpha = 1.0;
    pages[1].totalElements = 2;
    pages[1].elements = new menuElement[2];
    pages[1].elements[0] = generateElement(HEADER, 0, 8, 1, vec2{0.0,0.25}, white, 0.2, 0.03, "BIG POND");
    pages[1].elements[0].outlines = true;
    pages[1].elements[1] = generateElement(PAGESET, 2, 10, 1, vec2{0.0,-0.25}, black, 0.2, 0.01, "Start Game");

    pages[2].state = MAINMENU;
    tLoad->loadTexture("images/multi-colored-psychedelic-background.jpeg", pages[2].background);
    pages[2].alpha = 1.0;
    pages[2].totalElements = 5;
    pages[2].elements = new menuElement[5];
    pages[2].elements[0] = generateElement(HEADER, 0, 8, 1, vec2{0.0,0.25}, white, 0.2, 0.03, "BIG POND");
    pages[2].elements[0].outlines = true;
    pages[2].elements[1] = generateElement(PAGESET, 3, 7, 1 ,vec2{0.0,0.10},black, 0.08, 0.02, "PLAY");
    pages[2].elements[1].lineLength = 4;
    pages[2].elements[2] = generateElement(PAGESET, 4, 7, 1 ,vec2{0.0,-0.05},black, 0.08, 0.02, "HELP");
    pages[2].elements[2].lineLength = 4;
    pages[2].elements[3] = generateElement(PAGESET, 5, 7, 1 ,vec2{0.0,-0.2},black, 0.08, 0.02, "CREDITS");
    pages[2].elements[4] = generateElement(OPENQUIT, 0, 7, 1 ,vec2{0.0,-0.35},black, 0.08, 0.02, "QUIT");
    pages[2].elements[4].lineLength = 4;


    pages[3].state = LEVELSELECT;
    tLoad->loadTexture("images/p2.jpeg", pages[3].background);
    pages[3].alpha = 1.0;
    pages[3].totalElements = 6;
    pages[3].elements = new menuElement[6];
    pages[3].elements[0] = generateElement(LEVELSET, 0, 9, 1 ,vec2{-0.5,0.2},black, 0.05, 0.02, "LEVEL 1  ");
    pages[3].elements[1] = generateElement(LEVELSET, 1, 9, 1 ,vec2{-0.5,0.1},black, 0.05, 0.02, "LEVEL 2  ");
    pages[3].elements[2] = generateElement(LEVELSET, 2, 9, 1 ,vec2{-0.5,0.0},black, 0.05, 0.02, "LEVEL 3  ");
    pages[3].elements[3] = generateElement(LEVELSET, 3, 9, 1 ,vec2{-0.5,-0.1},black, 0.05, 0.02, "LEVEL 4  ");
    pages[3].elements[4] = generateElement(LEVELSET, 4, 9, 1 ,vec2{-0.5,-0.2},black, 0.05, 0.02, "LEVEL 5  ");
    pages[3].elements[5] = generateElement(PAGESET, 2, 9, 1 ,vec2{-0.5,-0.4},black, 0.05, 0.02, "GO BACK  ");


    pages[4].state = HELPPAGE;
    tLoad->loadTexture("images/p1.jpeg", pages[4].background);
    pages[4].alpha = 1.0;
    pages[4].totalElements = 2;
    pages[4].elements = new menuElement[2];
    char* helpString = "THIS GAME CAN BE PLAYED ENTIRELY     WITH THE MOUSE. The mouse controls   movement while left mouse button     and spacebar fire your weapon.       Dodge enemy attacks to survive and   kill enemies to finish the level.    Gain points by killing enemies, by   avoiding damage, and clearing the    level quickly.                       You can buy upgrades between levels. MOUTHS give alternate attacks, and   SCALES give abilities.               Hit escape at any time to pause or   go back.                                  ";
    pages[4].elements[0] = generateElement(NOACTION, 0, 37, 14, vec2{0.0,0.043} , black,0.055, 0.03,helpString);
    pages[4].elements[1] = generateElement(PAGESET, 2, 9, 1 ,vec2{0.0,-0.438},black, 0.05, 0.02, " GO BACK ");

    pages[5].state = CREDITPAGE;
    tLoad->loadTexture("images/optical-fiber-background.jpeg", pages[5].background);
    pages[5].alpha = 1.0;
    pages[5].totalElements = 2;
    pages[5].elements = new menuElement[2];
    char* nameString = "Robert Mawhinney  Brett Harris      Juan Pacheco      Aakash Sharma      ";
    pages[5].elements[0] = generateElement(NOACTION, 0, 18, 4, vec2{0.0,0.043} , black,0.09, 0.03,nameString);
    pages[5].elements[1] = generateElement(PAGESET, 2, 9, 1 ,vec2{0.0,-0.438},black, 0.05, 0.02, " GO BACK ");

    pages[6].state = PAUSE;
    //tLoad->loadTexture("images/pausePage2.png", pages[6].background);
    pages[6].alpha = 0.5;
    pages[6].totalElements = 5;
    pages[6].elements = new menuElement[5];
    pages[6].elements[0] = generateElement(NOACTION, 0, 2, 1, vec2{0.0,0.0} , white,0.055, 0.2,"  ");
    pages[6].elements[0].bgHue = vec3{0.0,0.0,0.0};
    pages[6].elements[1] = generateElement(PAGESET, 0, 6, 1 ,vec2{0.0,0.15},black, 0.055, 0.02, "RETURN");
    pages[6].elements[2] = generateElement(LEVELSET, -1, 6, 1 ,vec2{0.0,0.05},black, 0.055, 0.02, "RETRY");
    pages[6].elements[2].lineLength = 5;
    pages[6].elements[3] = generateElement(PAGESET, 3, 6, 1 ,vec2{0.0,-0.05},black, 0.055, 0.02, "MENU");
    pages[6].elements[3].lineLength = 4;
    pages[6].elements[4] = generateElement(OPENQUIT, 0, 6, 1 ,vec2{0.0,-0.15},black, 0.055, 0.02, "QUIT");
    pages[6].elements[4].lineLength = 4;


    pages[7].state = LEVELEND;
    //tLoad->loadTexture("images/pausePage2.png", pages[7].background);
    pages[7].alpha = 0.0;
    pages[7].totalElements = 3;
    pages[7].elements = new menuElement[3];
    pages[7].elements[0] = generateElement(LEVELSET, -1, 6, 1 ,vec2{-0.2,-0.4},black, 0.055, 0.02, "RETRY");
    pages[7].elements[0].lineLength = 5;
    pages[7].elements[1] = generateElement(PAGESET, 3, 6, 1 ,vec2{0.2,-0.4},black, 0.055, 0.02, "MENU");
    pages[7].elements[1].lineLength = 4;
    pages[7].elements[2] = generateElement(RESULTSCREEN, 0, 30, 9, vec2{0.0,0.05},black, 0.055, 0.05, "");

    failure = new char*[9];
    failure[0] = "TRY NOT GETTING HIT NEXT TIME";
    failure[1] = "THAT WAS DEFINITELY YOUR FAULT";
    failure[2] = "DEAD LOL";
    failure[3] = "THE KEY IS TO NOT GET HIT";
    failure[4] = "THIS GAME CHEATS";
    failure[5] = "RESPAWN?";
    failure[6] = "DEAD";
    failure[7] = "OOPS";
    failure[8] = "POOR THING";
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
        glColor4f(1.0,1.0,1.0, 1.0);
    glPopMatrix();

    glPushMatrix();
        for(int i = 0; i < pages[currentState].totalElements; i++){
            drawElement(pages[currentState].elements[i]);
        }
        if(checkQuit){
            for(int i = 0; i < quitOverlay.totalElements; i++){
                drawElement(quitOverlay.elements[i]);
            }
        }
    glPopMatrix();
}
int randVal;
void menu::drawElement(menuElement& m)
{

    if(m.elemType != HEADER){
        if(m.selected && m.elemType!=NOACTION){
            glColor4f(0.0,0.6,1.0,1.0);
        }else{
            glColor4f(m.bgHue.x, m.bgHue.y, m.bgHue.z, m.bgAlpha);
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
    if(m.elemType == LEVELSELECT && m.index >=0){
        setRankColor(levels->levelList[m.index].scoreRank);
        writer->drawLine(&levels->levelList[m.index].scoreRank, m.fontSize, m.bounds.bound0.x - m.fontSize, m.bounds.center.y, 1, true );
        glColor4f(1.0,1.0,1.0,1.0);
    }

    glColor4f(m.textHue.x,m.textHue.y,m.textHue.z, 1.0);
    float yOffset = (m.lineCount - 1) * 0.5 * m.fontSize;
    if(m.elemType == RESULTSCREEN){
        yOffset+=m.fontSize;
        if(levels->victory){

            char buffer[33];
            char *buf2;
            float xOffset = m.bounds.center.x - 0.5 * float(m.lineLength - 1) * (m.fontSize * (1 + writer->kerning));

            writer->drawLine("Killscore:", m.fontSize, xOffset, yOffset, 10, false );
            buf2 = itoa(levels->currentScore, buffer, 10);
            writer->drawLine(buf2, m.fontSize, xOffset + m.fontSize*11 + m.fontSize*writer->kerning*(10), yOffset, strlen(buf2), false );
            yOffset -= m.fontSize*2;

            writer->drawLine("Hit Bonus:", m.fontSize, xOffset, yOffset, 10, false );
            buf2 = itoa(levels->finalHitBonus, buffer, 10);
            writer->drawLine(buf2, m.fontSize, xOffset + m.fontSize*11 + m.fontSize*writer->kerning*(10), yOffset, strlen(buf2), false );
            yOffset -= m.fontSize*2;

            writer->drawLine("Time Bonus:", m.fontSize, xOffset, yOffset, 11, false );
            buf2 = itoa(levels->finalTimeBonus, buffer, 10);
            writer->drawLine(buf2, m.fontSize, xOffset + m.fontSize*11 + m.fontSize*writer->kerning*(10), yOffset, strlen(buf2), false );
            yOffset -= m.fontSize*2;

            writer->drawLine("Total:", m.fontSize, xOffset, yOffset, 6, false );
            buf2 = itoa(levels->finalScore, buffer, 10);
            writer->drawLine(buf2, m.fontSize, xOffset + m.fontSize*11 + m.fontSize*writer->kerning*(10), yOffset, strlen(buf2), false );
            yOffset -= m.fontSize*2;

            writer->drawLine("Rank:", m.fontSize, xOffset, yOffset, 6, false );
            setRankColor(levels->finalRank);
            writer->drawLine(&levels->finalRank, m.fontSize, xOffset + m.fontSize*11 + m.fontSize*writer->kerning*(10), yOffset, 1, true );
        }else{

            writer->drawLineCentered(failure[randVal],m.fontSize, m.bounds.center.x, m.bounds.center.y,0,strlen(failure[randVal]), false);
        }

    }else{
        for(int i = 0; i < m.lineCount; i++){
        writer->drawLineCentered(m.text, m.fontSize, m.bounds.center.x, m.bounds.center.y + yOffset, i*m.lineLength, m.lineLength, m.outlines);
        yOffset -= m.fontSize;
    }
    }

    glColor4f(1.0,1.0,1.0, 1.0);

}

void menu::tick()
{
    if(currentState == 0 && !levels->levelLive){
        paused = true;
        randVal = rand() % 9;
        currentState = 7;
    }
}

void menu::setRankColor(char c)
{
    switch(c){
    case 'P':
        glColor4f(1.0,1.0,0.3,1.0);
        break;
    case 'S':
        glColor4f(1.0,1.0,0.0,1.0);
        break;
    case 'A':
        glColor4f(0.3,1.0,0.3,1.0);
        break;
    case 'B':
        glColor4f(0.3,1.0,1.0,1.0);
        break;
    case 'C':
        glColor4f(0.4,0.4,0.4,1.0);
        break;
    case 'D':
        glColor4f(0.8,0.0,0.0,1.0);
        break;
    case 'F':
        glColor4f(0.8,0.0,0.0,1.0);
        break;
    }
}



menuElement menu::generateElement(e_type etype, int index, int lineLength, int lineCount, vec2 center_c, vec3 color, float fontSize, float padding, char* str)
{
    menuElement newElement;
    newElement.elemType = etype;
    newElement.fontSize = fontSize;
    newElement.index = index;
    newElement.lineCount = lineCount;
    newElement.lineLength = lineLength;
    newElement.text = str;
    newElement.textHue = color;
    newElement.selected = false;
    newElement.bgHue = vec3{1.0,1.0,1.0};
    newElement.bgAlpha = 0.6;
    newElement.outlines = false;

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
    page *whichOverlay;
    if(!checkQuit){
        whichOverlay = &pages[currentState];
    }else{
        whichOverlay = &quitOverlay;
    }
    for(int i = 0; i < whichOverlay->totalElements; i++){
        menuElement *temp = &whichOverlay->elements[i];
        if(temp->elemType != NOACTION && temp->elemType!=RESULTSCREEN && mousePos.x <= temp->bounds.bound0.x && mousePos.x >= temp->bounds.bound1.x     &&      mousePos.y <= temp->bounds.bound0.y && mousePos.y >= temp->bounds.bound1.y){
            temp->selected = true;
        }else{
            temp->selected = false;
        }
    }
}

void menu::click()
{
    if(!paused) return;
    page *whichOverlay;
    if(!checkQuit){
        whichOverlay = &pages[currentState];
    }else{
        whichOverlay = &quitOverlay;
    }
    for(int i = 0; i < whichOverlay->totalElements; i++){
        if(whichOverlay->elements[i].selected && whichOverlay->elements[i].elemType != NOACTION){
            sfxEngine->playSound("sounds/clickSound2.mp3");
            whichOverlay->elements[i].selected = false;
            switch(whichOverlay->elements[i].elemType){
            case PAGESET:
                currentState = whichOverlay->elements[i].index;
                if(currentState == 0){
                    paused = false;
                }else{
                    sfxEngine->setTrack(5);
                }
                break;
            case OPENQUIT:
                checkQuit = true;
                break;
            case CLOSEQUIT:
                checkQuit = false;
                break;
            case QUITGAME:
                quit = true;
                break;
            case LEVELSET:
                currentState = 0;
                paused = false;
                int whichLevel = whichOverlay->elements[i].index;
                if(whichLevel == -1)whichLevel = levels->currentLevel;
                levels->loadLevel(whichLevel);
                sfxEngine->setTrack(levels->currentLevel);
                break;
            }


        }
    }
}

void menu::escPressed()
{
    paused = true;
    if(!checkQuit){
        switch(pages[currentState].state){
        case GAMEPAGE:
            currentState = 6;
            break;
        case LEVELSELECT:
        case HELPPAGE:
        case CREDITPAGE:
            currentState = 2;
            break;
        case LEVELEND:
            currentState = 3;
            break;
        case MAINMENU:
            currentState = 1;
            break;

        case LANDINGPAGE:
        case PAUSE:
            checkQuit = true;
            break;
        case QUITMENU:
            break;
        }




    }
}
