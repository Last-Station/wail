/*
 *
 * Must-have UI related things TODO
 *
 *
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "font.h"
#include "window.h"


struct HorizontallySnappedButton { //TODO
    int HBx;
    int HBy;
    int count;
    bool MHover;

    char* BText;
    char* BDescription;
    SDL_Rect BParameter;

    char ButtonTask(bool resize, int Bnumber, int ButtonW, int ButtonH, int MouseX, int MouseY) {

        // Set button parameters
        if(resize == 1) {
            BParameter.x = (Bnumber + 1) * HBx;
            BParameter.y = HBy;
            BParameter.w = ButtonW;
            BParameter.h = ButtonH;
        }

        // Check if mouse is over button area
        if( (MouseX > BParameter.x) &&
            (MouseX < BParameter.x + ButtonW) &&
            (MouseY > BParameter.y) &&
            (MouseY < BParameter.y + ButtonH)) {
                MHover = 1;
        } else {
                MHover = 0;
        }

        return 0;
    }
};
