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
//#include "font.h"
//#include "window.h"

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
    // src/resources/font/LibertinusSerif_Bold.ttf
//size_t img_size = (size_t)(startup_img_size);
//size_t font_size = (size_t)(ttf_font_size);

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
int MainWindow() {
    return 0;
}
