#include "Game.h"

#include <GL/glew.h>
#include <iostream>
#ifdef _WIN32
#include <SDL.h>
#endif
#ifndef _WIN32
#include <SDL2/SDL.h>
#endif

#include "assetsLoader.h"
#include "assetsPaths.h"
#include "constants.h"
#include "Mesh.h"
#include "rt3dObjLoader.h"
#include "sdlUtils.h"
#include "Shader.h"


Game::Game() {
    // Create window and render context
    this->mainWindowWidth = Constants::BaseWindowSize::width;
    this->mainWindowHeight = Constants::BaseWindowSize::height;
    this->setupRenderingContext();
    this->initializeGlew();
    this->init();
}

Game::~Game() {
    SDL_GL_DeleteContext(this->mainContext);
    SDL_DestroyWindow(this->mainWindow);
    SDL_Quit();
}

void Game::runEventLoop() {
    SDL_Event sdlEvent;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_WINDOWEVENT:
                    this->handleWindowEvent(sdlEvent.window);
                    break;

                default:
                    break;
            }
        }
        this->handleUserInput();
        this->draw(); // call the draw function
    }
}

void Game::initializeGlew() {
#ifdef _WIN32
    // Required on Windows *only* init GLEW to access OpenGL beyond 1.1
    glewExperimental = GL_TRUE;
#endif
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "glewInit failed, aborting." << std::endl;
        exit(1);
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
}

void Game::setupRenderingContext() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
        throw std::runtime_error("Unable to initialize SDL : "
                                 + std::string(SDL_GetError()));

    // Request an OpenGL 3.0 context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // Turn on x4 multisampling anti-aliasing (MSAA)
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    this->mainWindow = SDL_CreateWindow("SDL/GLM/OpenGL Demo",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        this->mainWindowWidth,
                                        this->mainWindowHeight,
                                        SDL_WINDOW_OPENGL
                                        | SDL_WINDOW_RESIZABLE
                                        | SDL_WINDOW_SHOWN);

    if (!this->mainWindow) {
        throw std::runtime_error("Unable to create window : "
                                 + std::string(SDL_GetError()));
    }

    // Create openGL context and attach to window
    this->mainContext = SDL_GL_CreateContext(this->mainWindow);
    // set swap buffers to sync with monitor's vertical refresh rate
    SDL_GL_SetSwapInterval(1);
}

void Game::init() {
    this->lights = {Light(Constants::initialLightPosition)};

    this->loadShaders();

    currentBunnyShader = &this->toonShader;
    loadCubeMap(assetsPaths::skyboxTextures, this->skybox);

    textures = {loadTexture(assetsPaths::fabricTexture),
                loadTexture(assetsPaths::studdedMetalTexture),
                loadTexture(assetsPaths::transparentWindowTexture),
                loadTexture(assetsPaths::mossTexture)};

    blendingBaseTexture = &textures[0];

    meshObjects = {Mesh(assetsPaths::cubeObject),
                   Mesh(assetsPaths::bunnyObject)};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::loadShaders() {
    this->blendingShader = rt3d::initShaders(
            assetsPaths::blendingShader.vertex.c_str(),
            assetsPaths::blendingShader.fragment.c_str()
    );

    this->gouraudShader = rt3d::initShaders(
            assetsPaths::gouraudShader.vertex.c_str(),
            assetsPaths::gouraudShader.fragment.c_str()
    );
    Shader::setLight(this->gouraudShader, Constants::light0,
                     Constants::material0);

    this->phongShader = rt3d::initShaders(
            assetsPaths::phongShader.vertex.c_str(),
            assetsPaths::phongShader.fragment.c_str()
    );
    Shader::setLight(this->phongShader, Constants::light0,
                     Constants::material0);

    this->skyboxShader = rt3d::initShaders(
            assetsPaths::cubeMapShader.vertex.c_str(),
            assetsPaths::cubeMapShader.fragment.c_str()
    );

    this->textureShader = rt3d::initShaders(
            assetsPaths::texturedShader.vertex.c_str(),
            assetsPaths::texturedShader.fragment.c_str()
    );

    this->toonShader = rt3d::initShaders(
            assetsPaths::toonShader.vertex.c_str(),
            assetsPaths::toonShader.fragment.c_str()
    );
    Shader::setLight(this->toonShader, Constants::light0, Constants::material0);
}

void Game::handleWindowEvent(const SDL_WindowEvent & windowEvent) {
    if (SDL_WINDOWEVENT_SIZE_CHANGED == windowEvent.event) {
        this->mainWindowWidth = (unsigned int) windowEvent.data1;
        this->mainWindowHeight = (unsigned int) windowEvent.data2;
        glViewport(0, 0, this->mainWindowWidth, this->mainWindowHeight);
    }
}

void Game::handleUserInput() {
    // Todo : Handle KeyPresses events instead of checking if the key is
    //  pressed each loop, or find a better way to structure this function
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_W]) player.moveForward(0.1f);
    if (keys[SDL_SCANCODE_S]) player.moveForward(-0.1f);
    if (keys[SDL_SCANCODE_A]) player.moveRight(-0.1f);
    if (keys[SDL_SCANCODE_D]) player.moveRight(0.1f);
    if (keys[SDL_SCANCODE_R]) player.moveUp(0.1f);
    if (keys[SDL_SCANCODE_F]) player.moveUp(-0.1f);

    if (keys[SDL_SCANCODE_Q]) player.lookRight(-1.0f);
    if (keys[SDL_SCANCODE_E]) player.lookRight(1.0f);

    if (keys[SDL_SCANCODE_I]) lights[0].moveForward(-0.1f);
    if (keys[SDL_SCANCODE_J]) lights[0].moveRight(-0.1f);
    if (keys[SDL_SCANCODE_K]) lights[0].moveForward(0.1f);
    if (keys[SDL_SCANCODE_L]) lights[0].moveRight(0.1f);
    if (keys[SDL_SCANCODE_U]) lights[0].moveUp(0.1f);
    if (keys[SDL_SCANCODE_H]) lights[0].moveUp(-0.1f);

    if (keys[SDL_SCANCODE_1]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    if (keys[SDL_SCANCODE_2]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

    if (keys[SDL_SCANCODE_3]) currentBunnyShader = &this->textureShader;
    if (keys[SDL_SCANCODE_4]) currentBunnyShader = &this->phongShader;
    if (keys[SDL_SCANCODE_5]) currentBunnyShader = &this->toonShader;
    if (keys[SDL_SCANCODE_7]) blendingBaseTexture = &textures[0];
    if (keys[SDL_SCANCODE_8]) blendingBaseTexture = &textures[1];
}

void Game::draw() {
    std::stack<glm::mat4> mvStack;
    // clear the screen
    glEnable(GL_CULL_FACE);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection(1.0);
    projection = glm::perspective(float(60.0f * Constants::degreeToRadian),
                                  float(this->mainWindowWidth) / float(this->mainWindowHeight),
                                  1.0f, 150.0f);


    GLfloat scale(1.0f); // Just to allow easy scaling of complete scene

    glm::mat4 modelview(1.0); // Set base position for scene
    mvStack.push(modelview);

    mvStack.top() = player.getCameraDirection();

    // Skybox as single cube using cube map
    glUseProgram(this->skyboxShader);
    rt3d::setUniformMatrix4fv(this->skyboxShader, "projection",
                              glm::value_ptr(projection));

    glDepthMask(GL_FALSE); // Make sure writing to update depth test is off
    auto mvRotOnlyMat3 = glm::mat3(mvStack.top());
    mvStack.push(glm::mat4(mvRotOnlyMat3));

    glCullFace(GL_FRONT); // Drawing inside of cube!
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox[0]);
    mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.5f, 1.5f, 1.5f));
    rt3d::setUniformMatrix4fv(this->skyboxShader, "modelview",
                              glm::value_ptr(mvStack.top()));
    rt3d::drawIndexedMesh(meshObjects[0].getMeshObject(),
                          meshObjects[0].getMeshIndexCount(), GL_TRIANGLES);
    mvStack.pop();
    glCullFace(GL_BACK); // Drawing inside of cube!


    // Back to remainder of rendering
    glDepthMask(GL_TRUE); // Make sure depth test is on

    glm::vec4 tmp = mvStack.top() * Constants::initialLightPosition;
//    this->lights[0].setPosition(tmp);
    rt3d::setLightPos(this->phongShader, glm::value_ptr(tmp));
    rt3d::setLightPos(this->toonShader, glm::value_ptr(tmp));
    rt3d::setLightPos(this->gouraudShader, glm::value_ptr(tmp));

    glUseProgram(this->phongShader);
    rt3d::setUniformMatrix4fv(this->phongShader, "projection",
                              glm::value_ptr(projection));

    // Draw a small cube block at lightPos
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    mvStack.push(mvStack.top());
    glm::vec4 lightPosition = lights[0].getPosition();
    mvStack.top() = glm::translate(mvStack.top(), glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
    mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.25f, 0.25f, 0.25f));
    rt3d::setUniformMatrix4fv(this->phongShader, "modelview", glm::value_ptr(mvStack.top()));
    rt3d::setMaterial(this->phongShader, Constants::material0);
    rt3d::drawIndexedMesh(meshObjects[0].getMeshObject(), meshObjects[0].getMeshIndexCount(), GL_TRIANGLES);
    mvStack.pop();

    // Draw a cube for ground plane
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    mvStack.push(mvStack.top());
    mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10.0f, -0.1f, -10.0f));
    mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0f, 0.1f, 20.0f));
    rt3d::setUniformMatrix4fv(this->phongShader, "modelview", glm::value_ptr(mvStack.top()));
    rt3d::setMaterial(this->phongShader, Constants::material0);
    rt3d::drawIndexedMesh(meshObjects[0].getMeshObject(), meshObjects[0].getMeshIndexCount(), GL_TRIANGLES);
    mvStack.pop();

    // Draw a bunch of cube blocks on top of ground plane
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    for (int b = 0; b < 5; b++) {
        mvStack.push(mvStack.top());
        mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10.0f + b * 2, 1.0f, -16.0f + b * 2));
        mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5f, 1.0f + b, 0.5f));
        rt3d::setUniformMatrix4fv(this->phongShader, "modelview", glm::value_ptr(mvStack.top()));
        rt3d::setMaterial(this->phongShader, Constants::material0);
        rt3d::drawIndexedMesh(meshObjects[0].getMeshObject(), meshObjects[0].getMeshIndexCount(), GL_TRIANGLES);
        mvStack.pop();
    }

    // Draw a rotating cube to be environment/reflection mapped
    glUseProgram(this->phongShader);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    mvStack.push(mvStack.top());
    mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-2.0f, 1.0f, -3.0f));
    mvStack.top() = glm::rotate(mvStack.top(), float(theta * Constants::degreeToRadian), glm::vec3(1.0f, 1.0f, 1.0f));
    theta += 1.0f;
    mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5f, 0.5f, 0.5f));
    rt3d::setUniformMatrix4fv(this->phongShader, "modelview", glm::value_ptr(mvStack.top()));
    rt3d::setMaterial(this->phongShader, Constants::material1);
    rt3d::drawIndexedMesh(meshObjects[0].getMeshObject(), meshObjects[0].getMeshIndexCount(), GL_TRIANGLES);
    mvStack.pop();

    // Draw the shaded bunny
    glUseProgram(*this->currentBunnyShader);
    rt3d::setUniformMatrix4fv(*this->currentBunnyShader, "projection", glm::value_ptr(projection));
    mvStack.push(mvStack.top());
    mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-4.0f, 0.1f, -2.0f));
    mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0, 20.0, 20.0));
    rt3d::setUniformMatrix4fv(*this->currentBunnyShader, "modelview", glm::value_ptr(mvStack.top()));
    rt3d::setMaterial(*this->currentBunnyShader, Constants::material0);
    rt3d::drawIndexedMesh(meshObjects[1].getMeshObject(), meshObjects[1].getMeshIndexCount(), GL_TRIANGLES);
    mvStack.pop();

    // Draw a small cube with blending effect
    glUseProgram(this->blendingShader);
    rt3d::setUniformMatrix4fv(this->blendingShader, "projection", glm::value_ptr(projection));
    // Copy brick texture to OpenGL
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *blendingBaseTexture);
    // Set the baseTex sampler uniform to texture unit 0
    int uniformLocation = glGetUniformLocation(this->blendingShader, "baseTex");
    if (uniformLocation >= 0) {
        glUniform1i(uniformLocation, 0);
    }
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    // Set the effectTex sampler uniform to texture unit 1
    uniformLocation = glGetUniformLocation(this->blendingShader, "effectTex");
    if (uniformLocation >= 0) {
        glUniform1i(uniformLocation, 1);
    }
    mvStack.push(mvStack.top());
    mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10.0f, 1.5f, -3.0f));
    rt3d::setUniformMatrix4fv(this->phongShader, "modelview", glm::value_ptr(mvStack.top()));
    rt3d::drawIndexedMesh(meshObjects[0].getMeshObject(), meshObjects[0].getMeshIndexCount(), GL_TRIANGLES);
    glActiveTexture(GL_TEXTURE0);
    mvStack.pop();

    // Remember to use at least one pop operation per push...
    mvStack.pop(); // initial matrix
    glDepthMask(GL_TRUE);

    SDL_GL_SwapWindow(this->mainWindow); // swap buffers
}
