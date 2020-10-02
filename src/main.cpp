#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>

#include "puzzle.hpp"

int main(int argc, char *argv[]){

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL video did not initialize successfully: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    /* Initialize TTF */
    if(TTF_Init() == -1){
        std::cerr << "SDL TTF initialization error: " << TTF_GetError() << "\n";
        return EXIT_FAILURE;
    }

    puzzle puzzle;

    /* Create window */
    SDL_Window *window = SDL_CreateWindow("15-puzzle",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          603, 603,
                                          SDL_WINDOW_SHOWN);

    if(window == NULL){
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    /* Set window icon */
    SDL_Surface *icon = SDL_LoadBMP(puzzle.getPath("assets/icon.bmp"));
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    /* Create renderer */
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_SOFTWARE);

    if(renderer == NULL){
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    puzzle.setRenderer(renderer);
    puzzle.render();
    SDL_RenderPresent(renderer);

    SDL_Event e;

    while(!puzzle.getEndGame()){
        while(SDL_PollEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                puzzle.quit();
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(e.button.button == SDL_BUTTON_LEFT)
                    puzzle.click(e.button.x / (150 + e.button.x / 150), e.button.y / (150 + e.button.y / 150));
                break;
            case SDL_KEYDOWN:
                if(0x4000004f <= e.key.keysym.sym && e.key.keysym.sym <= 0x40000052) /* if arrow keys are pressed */
                    puzzle.keydown(e.key.keysym.sym);
                break;
            default: {}
            }
        }
    }

    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
