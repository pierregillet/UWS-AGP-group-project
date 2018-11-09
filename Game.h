#ifndef AGP_INDIVIDUAL_PROJECT_GAME_H
#define AGP_INDIVIDUAL_PROJECT_GAME_H

#ifdef _WIN32
#include <SDL.h>
#endif
#ifndef _WIN32
#include <SDL2/SDL.h>
#endif


class Game {
public:
    Game();
    ~Game();
    void runEventLoop();

private:
    SDL_Window *mainWindow; // window handle
    SDL_GLContext mainContext; // OpenGL context handle

    void initializeGlew();
    void setupRenderingContext();
    void handleUserInput();
    void draw();
};


#endif //AGP_INDIVIDUAL_PROJECT_GAME_H
