#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <SDL2/SDL.h>

#include "puzzle.hpp"

void display_help();

int main(int argc, char *argv[]){

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL video did not initialize successfully: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Initialize TTF */
    if(TTF_Init() == -1){
        fprintf(stderr, "SDL TTF initialization error: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    std::string font_file = "assets/font/UbuntuMono-R.ttf";

    static struct option long_options[] = {
                                           {"font", required_argument, NULL, 'f'},
                                           {"help", no_argument, NULL, 'h'},
                                           {NULL, 0, NULL, 0}
    };

    int c;

    while ((c = getopt_long(argc, argv, "f:h", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'f':
                font_file = optarg;
                break;
            case 'h':
                display_help();
                exit(EXIT_SUCCESS);
                break;
            case '?':
                fprintf(stderr,
                        "%s: Use -h or --help to display list of arguments.\n", argv[0]);
                exit(EXIT_FAILURE);
            default: {}
            }
    }

    Puzzle puzzle(font_file);

    /* Create window */
    SDL_Window *window = SDL_CreateWindow("15-puzzle",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          puzzle.get_window_width(),
                                          puzzle.get_window_width(), // height is same as width
                                          SDL_WINDOW_SHOWN);

    if(window == nullptr){
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Set window icon */
    SDL_Surface *icon = SDL_LoadBMP(puzzle.get_path("assets/icon.bmp").c_str());
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    /* Create renderer */
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_SOFTWARE);

    if(renderer == nullptr){
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    puzzle.set_renderer(renderer);
    puzzle.render();
    SDL_RenderPresent(renderer);

    SDL_Event e;

    while(!puzzle.get_end_game()){
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    puzzle.quit();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(e.button.button == SDL_BUTTON_LEFT){
                        puzzle.click(e.button.x / (150 + e.button.x / 150), e.button.y / (150 + e.button.y / 150));
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_UP:
                        case SDLK_DOWN:
                        case SDLK_LEFT:
                        case SDLK_RIGHT:
                            puzzle.keydown(e.key.keysym.sym);
                            break;
                    }
                    break;
                default: {}
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

void display_help(){
    fprintf(stderr,
            "15 puzzle game\n"
            "Usage: 15-puzzle [OPTION]...\n"
            "Example: 15-puzzle -h\n"
            "\n"
            "  -h,       --help       |  Print this help message\n"
            "  -f PATH,  --font PATH  |  Specify the path to the font file\n"
            "\n"
            );
}
