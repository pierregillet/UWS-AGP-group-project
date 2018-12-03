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

#include "constants.h"
#include "Mesh.h"
#include "Light.h"
#include "Player.h"


class Game {
public:
    Game();
    ~Game();
    void runEventLoop();

private:
    SDL_Window *mainWindow; // window handle
    SDL_GLContext mainContext; // OpenGL context handle
    unsigned int mainWindowWidth;
    unsigned int mainWindowHeight;
    GLuint blendingShader;
    GLuint gouraudShader;
    GLuint phongShader;
    GLuint skyboxShader;
    GLuint textureShader;
    GLuint toonShader;
    GLuint motionBlurShader;
    GLuint *currentBunnyShader;
    GLuint *blendingBaseTexture;
    std::vector<Light> lights;
    std::vector<Mesh> meshObjects;
    GLuint screenQuadVao;
    GLuint screenQuadVbo;
    GLuint colorBufferTextures[Constants::motionBlurFramesKept];
    int currentColorBufferTexture;
    std::vector<GLuint> textures;
    std::vector<GLuint> skybox = std::vector<GLuint>(6);
    Player player = Player(glm::vec3(-2.0f, 1.0f, 8.0f),
                           glm::vec3(0.0f, 1.0f, -1.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    float rotatingCubeAngle = 0.0f;
    GLuint motionBlurFrameBuffer;

    void initializeGlew();
    void setupRenderingContext();
    void init();
    void loadShaders();
    void initMotionBlurTextures();
    void handleWindowEvent(const SDL_WindowEvent & windowEvent);
    void handleUserInput();
    void draw();
};


#endif //AGP_INDIVIDUAL_PROJECT_GAME_H
