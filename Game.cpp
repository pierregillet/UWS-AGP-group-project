#include "Game.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stack>
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

    meshObjects = {
            Mesh(assetsPaths::cubeObject), // [0]
            Mesh(assetsPaths::bunnyObject), // [1]
    };

    this->player = Player(glm::vec3(-2.0f, 1.0f, 8.0f),
                          glm::vec3(0.0f, 1.0f, -1.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);

    glGenFramebuffers(1, &this->motionBlurFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->motionBlurFrameBuffer);

    // Rendering to a texture
    glGenTextures(Constants::motionBlurFramesKept, this->colorBufferTextures);
    this->currentColorBufferTexture = 0;

    this->initMotionBlurTextures();

    glBindTexture(GL_TEXTURE_2D, 0);

    this->checkFrameBufferCompleteness();

    // screen quad VAO
    glGenVertexArrays(1, &this->screenQuadVao);
    glGenBuffers(1, &this->screenQuadVbo);
    glBindVertexArray(this->screenQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, this->screenQuadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Constants::screenQuadVertices),
                    &Constants::screenQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                            (void*) nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                            (void*)(2 * sizeof(GLfloat)));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenFramebuffers(1, &this->depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->SHADOW_WIDTH,
                 this->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMap, 0);
	glDrawBuffer(GL_NONE);

	this->checkFrameBufferCompleteness();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

    this->motionBlurShader = rt3d::initShaders(
            assetsPaths::motionBlurShader.vertex.c_str(),
            assetsPaths::motionBlurShader.fragment.c_str()
    );

    this->shadowProgram = rt3d::initShaders(
            assetsPaths::shadowMapShader.vertex.c_str(),
            assetsPaths::shadowMapShader.fragment.c_str()
    );

	this->depthProgram = rt3d::initShaders(
            assetsPaths::depthShader.vertex.c_str(),
            assetsPaths::depthShader.fragment.c_str()
	);

	this->quad_programID = rt3d::initShaders(
            assetsPaths::quadShader.vertex.c_str(),
            assetsPaths::quadShader.fragment.c_str()
	);
}

void Game::initMotionBlurTextures() {
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mainWindowWidth,
                 this->mainWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->colorBufferTextures[0], 0);

    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mainWindowWidth,
                 this->mainWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           this->colorBufferTextures[1], 0);

    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mainWindowWidth,
                 this->mainWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           this->colorBufferTextures[2], 0);

    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mainWindowWidth,
                 this->mainWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                           this->colorBufferTextures[3], 0);

    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mainWindowWidth,
                 this->mainWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D,
                           this->colorBufferTextures[4], 0);
}

void Game::handleWindowEvent(const SDL_WindowEvent & windowEvent) {
    if (SDL_WINDOWEVENT_SIZE_CHANGED == windowEvent.event) {
        this->mainWindowWidth = (unsigned int) windowEvent.data1;
        this->mainWindowHeight = (unsigned int) windowEvent.data2;
        glViewport(0, 0, this->mainWindowWidth, this->mainWindowHeight);

        this->initMotionBlurTextures();
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


void Game::checkFrameBufferCompleteness() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                throw std::runtime_error(
                        "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");

            case GL_FRAMEBUFFER_UNDEFINED:
                throw std::runtime_error("GL_FRAMEBUFFER_UNDEFINED");

            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                throw std::runtime_error(
                        "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");

            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                throw std::runtime_error(
                        "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");

            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                throw std::runtime_error(
                        "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");

            default:
                throw std::runtime_error("Framebuffer is not complete");
        }
    }
}


void Game::draw() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->motionBlurFrameBuffer);

    if (++this->currentColorBufferTexture >= Constants::motionBlurFramesKept) {
        this->currentColorBufferTexture = 0;
    }

    switch (this->currentColorBufferTexture) {
        case 0:
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            break;

        case 1:
            glDrawBuffer(GL_COLOR_ATTACHMENT1);
            break;

        case 2:
            glDrawBuffer(GL_COLOR_ATTACHMENT2);
            break;

        case 3:
            glDrawBuffer(GL_COLOR_ATTACHMENT3);
            break;

        case 4:
            glDrawBuffer(GL_COLOR_ATTACHMENT4);
            break;

        default:
            break;
    }

    std::stack<glm::mat4> mvStack;
    // clear the screen
    glEnable(GL_CULL_FACE);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(float(60.0f * Constants::degreeToRadian),
                                  float(this->mainWindowWidth) / float(this->mainWindowHeight),
                                  1.0f, 150.0f);

    mvStack.push(player.getCameraDirection());

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
    rt3d::setLightPos(this->phongShader, glm::value_ptr(tmp));
    rt3d::setLightPos(this->toonShader, glm::value_ptr(tmp));
    rt3d::setLightPos(this->gouraudShader, glm::value_ptr(tmp));

    glUseProgram(this->phongShader);
    glActiveTexture(GL_TEXTURE0);
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
    mvStack.top() = glm::rotate(mvStack.top(),
                                float(this->rotatingCubeAngle * Constants::degreeToRadian),
                                glm::vec3(1.0f, 1.0f, 1.0f));
    this->rotatingCubeAngle += 1.0f;
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

	mvStack.top() = glm::rotate(mvStack.top(),
		20.0f*float(this->rotatingCubeAngle * Constants::degreeToRadian),
		glm::vec3(1.0f, 1.0f, 1.0f));

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


    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

    // Post processing effects

    // Motion blur processing
     glUseProgram(this->motionBlurShader);

	GLint texturesUniformLocation = glGetUniformLocation(this->motionBlurShader,
		"bufferedTextures");

    // TODO: check the size of this array, and/or make a loop using an iterator
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[0]);
    glUniform1i(texturesUniformLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[1]);
    glUniform1i(texturesUniformLocation, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[2]);
    glUniform1i(texturesUniformLocation, 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[3]);
    glUniform1i(texturesUniformLocation, 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[4]);
    glUniform1i(texturesUniformLocation, 4);

    // disable depth test so screen-space quad isn't discarded due to depth test.
    glDisable(GL_DEPTH_TEST);
    // clear all relevant buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(this->screenQuadVao);
    // use the color attachment texture as the texture of the quad plane
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->colorBufferTextures[this->currentColorBufferTexture]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
    // End of motion blur processing

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_GL_SwapWindow(this->mainWindow); // swap buffers
}
