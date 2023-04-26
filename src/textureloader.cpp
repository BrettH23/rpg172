#include "textureloader.h"

textureLoader::textureLoader()
{
    //ctor
}

textureLoader::~textureLoader()
{
    //dtor
}
void textureLoader::loadTexture(char* filename, GLuint& tID)
{
    glGenTextures(1, &tID);

    glBindTexture(GL_TEXTURE_2D, tID);

    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

    if(!image){
        std::cout << "No image found." << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void textureLoader::binder(GLuint tID)
{
    glBindTexture(GL_TEXTURE_2D, tID);
}

void textureLoader::loadTexture1(char* fileName)
{
    glGenTextures(1,&tex);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


    glBindTexture(GL_TEXTURE_2D, tex);
    image = SOIL_load_image(fileName,&width,&height, 0,SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width, height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void textureLoader::TextureBinder()
{
    glBindTexture(GL_TEXTURE_2D,tex);
}

