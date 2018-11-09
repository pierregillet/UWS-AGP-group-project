#include "sdlUtils.h"

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

#include "rt3d.h"

#define DEG_TO_RADIAN 0.017453293

// Globals
// Real programs don't use globals :-D

//GLuint meshIndexCount = 0;
//GLuint toonIndexCount = 0;
//GLuint meshObjects[2];

Player player(glm::vec3(-2.0f, 1.0f, 8.0f), glm::vec3(0.0f, 1.0f, -1.0f),
              glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);

// TEXTURE STUFF
GLuint textures[4];
GLuint skybox[5];

rt3d::lightStruct light0 = {
        {0.4f, 0.4f, 0.4f, 1.0f}, // ambient
        {1.0f, 1.0f, 1.0f, 1.0f}, // diffuse
        {1.0f, 1.0f, 1.0f, 1.0f}, // specular
        {-5.0f, 2.0f, 2.0f, 1.0f}  // position
};
glm::vec4 lightPos(-5.0f, 2.0f, 2.0f, 1.0f); //light position

rt3d::materialStruct material0 = {
        {0.2f, 0.4f, 0.2f, 1.0f}, // ambient
        {0.5f, 1.0f, 0.5f, 1.0f}, // diffuse
        {0.0f, 0.1f, 0.0f, 1.0f}, // specular
        2.0f  // shininess
};
rt3d::materialStruct material1 = {
        {0.4f, 0.4f, 1.0f, 1.0f}, // ambient
        {0.8f, 0.8f, 1.0f, 1.0f}, // diffuse
        {0.8f, 0.8f, 0.8f, 1.0f}, // specular
        1.0f  // shininess
};

// light attenuation
float attConstant = 1.0f;
float attLinear = 0.0f;
float attQuadratic = 0.0f;

float theta = 0.0f;


GLuint initProgramWithLight(const char *vertexShader, const char *fragmentShader) {
    GLuint program = rt3d::initShaders(vertexShader, fragmentShader);
    rt3d::setLight(program, light0);
    rt3d::setMaterial(program, material0);
    // set light attenuation shader uniforms
    GLint uniformIndex = glGetUniformLocation(program, "attConst");
    glUniform1f(uniformIndex, attConstant);
    uniformIndex = glGetUniformLocation(program, "attLinear");
    glUniform1f(uniformIndex, attLinear);
    uniformIndex = glGetUniformLocation(program, "attQuadratic");
    glUniform1f(uniformIndex, attQuadratic);
    return program;
}
