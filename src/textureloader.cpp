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
        std::cout << 'No image found.' << std::endl;
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
