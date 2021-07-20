#include <iostream>
#include <getopt.h>
#include <SDL2/SDL.h>

#include "../include/puzzle.hpp"

void display_help();
int parse_option(const char* value, int l, int r, const char* option);

int main(int argc, char *argv[]){

    int game_size = 4;
    int tile_width = 150;
    std::string font_file = "assets/font/UbuntuMono-R.ttf";

    static struct option long_options[] = {
                                           {"font", required_argument, NULL, 'f'},
                                           {"help", no_argument, NULL, 'h'},
                                           {"size", required_argument, NULL, 's'},
                                           {"tile-width", required_argument, NULL, 'w'},
                                           {NULL, 0, NULL, 0}
    };

    int c;

    while ((c = getopt_long(argc, argv, "f:s:w:h", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'f':
                font_file = optarg;
                break;
            case 's':
                game_size = parse_option(optarg, 2, 25, argv[optind - 2]);
                break;
            case 'w':
                tile_width = parse_option(optarg, 25, 250, argv[optind - 2]);
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

    Puzzle puzzle(game_size, tile_width, font_file);

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

    while(!puzzle.get_end_game() && SDL_WaitEvent(&e)){
        switch(e.type){
        case SDL_QUIT:
            puzzle.quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(e.button.button == SDL_BUTTON_LEFT){
                puzzle.click(e.button.x / (tile_width + 1), e.button.y / (tile_width + 1));
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
            "  -s NUM,  --size NUM       | Set size of game (number of tiles in one row)\n"
            "  -w NUM,  --tile-width NUM | Set width of one tile\n"
            "  -h,      --help           | Print this help message\n"
            "  -f PATH, --font PATH      | Specify the path to the font file\n"
            "\n"
            );
}

int parse_option(const char* value, int l, int r, const char* option){
    char* end;
    long int output = strtol(value, &end, 10);

    if(value == end){
        fprintf(stderr, "Invalid value '%s' for option '%s'\n", value, option);
        exit(1);
    }
    if(l <= output && output <= r){
        return output;
    } else {
        fprintf(stderr, "Value '%s' for option '%s' is out of range, expected value between '%d' and '%d'\n", value, option, l, r);
        exit(1);
    }
}
