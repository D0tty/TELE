#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "csv.hpp"

int main()
{
    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if((initted & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        // handle error
    }
    // load sample.png into image
    SDL_Surface *image;
    image = IMG_Load("sample.png");
    if(!image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    std::cout<<"Hello, World!"<<std::endl;
    return 0;
}