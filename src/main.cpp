#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

    // src/gui/MainWindow.cpp
int MainWindow();

void cleanup() {
    TTF_Quit();
    SDL_Quit();
}

int main() { // for testing only (TODO)

    if(MainWindow() != 0) {
        cleanup();
        return 1;
    }

    return 0;
}
