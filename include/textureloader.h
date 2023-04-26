#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <iostream>
#include <SOIL.H>
#include <gl/gl.h>


class textureLoader
{
    public:
        textureLoader();            //constructor
        virtual ~textureLoader();   //destructor

        void loadTexture(char*, GLuint&);   //image loader
        void binder(GLuint);                //image binder

        unsigned char* image;   //image data storage
        int width,height;       //image size info
        void loadTexture1(char *);
        void TextureBinder();
        GLuint tex;

    protected:

    private:
};

#endif // TEXTURELOADER_H
