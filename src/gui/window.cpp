/*
 *  This file contains everything related to managing graphics.
 *
 *
 *  Functions:
 *
 *  SmartPowerMgmt()
 *  SmartRendering()
 *  StartUpWindow()
 *  MainWindow()
 *
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "font.h"
#include "window.h"


    // Main SDL pointers
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_Surface* surface = nullptr;
SDL_IOStream* StarupImgIO = nullptr;
TTF_Font* font = nullptr;
SDL_Texture* TextTexture = nullptr;
SDL_IOStream* fontIO = nullptr;


    // Compute size of IO(s):
    // src/resources/font/Noto_Sans/NotoSans-VariableFont_wdth,wght.ttf
size_t font_size = (size_t)(ttf_font_size);


    // Set text color(s)
//SDL_Color TextBg {0, 100, 0, 200};
//SDL_Color TextFg {255, 255, 255, 255};


    // Reduce CPU when not needed, allow muti-threads when necessary or normal operation (TODO)
int SmartPowerMgmt() {
    return 0;
}


    // Render only when necessary, like when window size changes or
    // mouse hovers over an object causing it to change color and etc. (TODO)
int SmartRendering() {
    return 0;
}


    // Display and manage main window components, such as button presses,
    // window resizing, other window events, mouse events etc. (TODO)
int MainWindow(void (*on_loop)(void *data)) {
    bool SDL_tasks_done {0};

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize TTF: %s", SDL_GetError());
        return 1;
    }

    // TODO (create window with paramaters from config file)
    if (!SDL_CreateWindowAndRenderer (
        PROGRAM_NAME " " PROGRAM_VERSION,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE,
        &window, &renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return 1;
    }

    // Main program loop
    while (!SDL_tasks_done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SDL_tasks_done = 1;
            }
        }

        on_loop(NULL);

        // Clear the window making the background transparent
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10); // temporary CPU limiter WARNING
    }

    return 0;
}
