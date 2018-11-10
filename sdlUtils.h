#ifndef SDL_OPENGL_BASICS_SDLUTILS_H
#define SDL_OPENGL_BASICS_SDLUTILS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stack>
#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#endif
#ifndef _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "md2model.h"
#include "Player.h"
#include "rt3d.h"


extern Player player;
extern float theta;


#endif //SDL_OPENGL_BASICS_SDLUTILS_H
