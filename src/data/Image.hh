//
// Created by dotty on 12/6/20.
//

#ifndef TELE_IMAGE_HH
#define TELE_IMAGE_HH

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <boost/filesystem.hpp>
#include <iostream>

class Image {
public:
    explicit Image(const boost::filesystem::path &imgPath) : path_(imgPath),
                                                             SDLSurface_(IMG_Load(imgPath.c_str()), SDL_FreeSurface),
                                                             rect_(),
                                                             zoom_() {
        if (!SDLSurface_.get()) {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
            exit(1);
        }
    }

    std::shared_ptr<SDL_Surface> SDLSurface_;
    boost::filesystem::path path_;
    SDL_Rect rect_;
    SDL_Rect zoom_;
};


#endif //TELE_IMAGE_HH
