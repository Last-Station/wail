#ifndef SD_window_h
#define SD_window_h


#include <stdint.h>

    // Window title info
#define PROGRAM_NAME "Wail"
#define PROGRAM_VERSION "v0.0"

    // Startup window dimentions
#define WINDOW_WIDTH 670
#define WINDOW_HEIGHT 330

    // Must be initialized before use!
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Surface *surface;
extern SDL_IOStream* StarupImgIO;

    // resources/startup.bmp
//extern char startup_img_data[] asm("_binary_resources_startup_bmp_start");
//extern char startup_img_end[] asm("_binary_resources_startup_bmp_end");
//extern char startup_img_size[] asm("_binary_resources_startup_bmp_size");
//extern size_t img_size;

    // Stores root beer floats :p
struct RBFloat {
    float x;
    float y;
    uint32_t MouseFlag;
};


#endif
