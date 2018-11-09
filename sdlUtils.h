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


extern GLuint meshIndexCount;
extern GLuint toonIndexCount;
extern GLuint meshObjects[];

extern GLuint skyboxProgram;
extern GLuint textureProgram;
extern GLuint phongShaderProgram;
extern GLuint toonShaderProgram;
extern GLuint blendingProgram;
extern GLuint gouraudShaderProgram;

extern GLuint *currentBunnyShader;
extern GLuint *blendingBaseTexture;

extern Player player;

// TEXTURE STUFF
extern GLuint textures[];
extern GLuint skybox[];

extern rt3d::lightStruct light0;
extern glm::vec4 lightPos;

extern rt3d::materialStruct material0;
extern rt3d::materialStruct material1;

// light attenuation
extern float attConstant;
extern float attLinear;
extern float attQuadratic;

extern float theta;


GLuint initProgramWithLight(const char *vertexShader, const char *fragmentShader);

void init();

void draw(SDL_Window * window);


#endif //SDL_OPENGL_BASICS_SDLUTILS_H
