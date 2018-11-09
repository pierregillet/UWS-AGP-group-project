#include "assetsLoader.h"

#include <iostream>
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#endif
#ifndef _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <string>

#include "sdlUtils.h" // TODO: Get rid of the globals to get rid of this ugly import

// A simple texture loading function
// lots of room for improvement - and better error checking!
GLuint loadTexture(const char *const fileName) {
    GLuint texID;
    glGenTextures(1, &texID); // generate texture ID

    SDL_Surface *tmpSurface = IMG_Load(fileName);
    if (nullptr == tmpSurface) {
        throw std::runtime_error("Error loading the " + std::string(fileName) + " image : " + IMG_GetError());
    }

    // bind texture and set parameters
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_PixelFormat *format = tmpSurface->format;

    GLuint externalFormat, internalFormat;
    if (format->Amask) {
        internalFormat = GL_RGBA;
        externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
    } else {
        internalFormat = GL_RGB;
        externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
                 externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
    return texID;	// return value of texture ID
}


// A simple cubemap loading function
// lots of room for improvement - and better error checking!
void loadCubeMap(const char * const fileName[6], GLuint *texID) {
    glGenTextures(1, texID); // generate texture ID
    GLenum sides[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };

    glBindTexture(GL_TEXTURE_CUBE_MAP, *texID); // bind texture and set parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        SDL_Surface *tmpSurface = IMG_Load(fileName[i]);
        if (nullptr == tmpSurface) {
            std::cout << "Error loading bitmap" << std::endl;
            throw std::runtime_error("Error loading the " + std::string(fileName[i]) + " image : " + IMG_GetError());
        }

        // skybox textures should not have alpha (assuming this is true!)
        SDL_PixelFormat *format = tmpSurface->format;
        GLuint externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;

        glTexImage2D(sides[i], 0, GL_RGB, tmpSurface->w, tmpSurface->h, 0,
                     externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
        // texture loaded, free the temporary buffer
        SDL_FreeSurface(tmpSurface);
    }
}
