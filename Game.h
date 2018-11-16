#ifndef AGP_INDIVIDUAL_PROJECT_GAME_H
#define AGP_INDIVIDUAL_PROJECT_GAME_H

#include <GL/glew.h>
#include <vector>
#ifdef _WIN32
#include <SDL.h>
#endif
#ifndef _WIN32
#include <SDL2/SDL.h>
#endif

#include "Mesh.h"
#include "Light.h"


class Game {
public:
    Game();
    ~Game();
    void runEventLoop();

private:
    SDL_Window *mainWindow; // window handle
    SDL_GLContext mainContext; // OpenGL context handle
    GLuint blendingShader;
    GLuint gouraudShader;
    GLuint phongShader;
    GLuint skyboxShader;
    GLuint textureShader;
    GLuint toonShader;
    GLuint *currentBunnyShader;
    GLuint *blendingBaseTexture;
    std::vector<Light> lights;
    std::vector<Mesh> meshObjects;
    std::vector<GLuint> textures;
    std::vector<GLuint> skybox = std::vector<GLuint>(6);

    void initializeGlew();
    void setupRenderingContext();
    void init();
    void loadShaders();
    void handleWindowEvent(const SDL_WindowEvent & windowEvent);
    void handleUserInput();
    void draw();
};


#endif //AGP_INDIVIDUAL_PROJECT_GAME_H
