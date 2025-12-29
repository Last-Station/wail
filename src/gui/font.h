#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#ifndef SD_font_h
#define SD_font_h

    // Must be initialized before use!
extern TTF_Font* font;
extern SDL_Texture *TextTexture;
extern SDL_IOStream* fontIO;

    // resources/font/Default_Font.ttf
extern char ttf_font_data[] asm("_binary_build_resources_font_Noto_Sans_Default_Font_ttf_start");
extern char ttf_font_end[] asm("_binary_build_resources_font_Noto_Sans_Default_Font_ttf_end");
extern char ttf_font_size[] asm("_binary_build_resources_font_Noto_Sans_Default_Font_ttf_size");
extern size_t font_size;

    // Text color
//extern SDL_Color TextBg;
//extern SDL_Color TextFg;

#endif
