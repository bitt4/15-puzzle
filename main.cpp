#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]){

    /* Initialize SDL */

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL video did not initialize successfully: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("15-puzzle",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          603, 603,
                                          SDL_WINDOW_SHOWN);

    if(window == NULL){
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_SOFTWARE);

    if(renderer == NULL){
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Event e;
    int quit = 0;

    while(!quit){
        while(SDL_PollEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                break;
            default: {}
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
