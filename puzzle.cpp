#include "puzzle.hpp"

puzzle::puzzle(SDL_Renderer *renderer)
    :renderer(renderer)
{

}

void puzzle::renderInit(){
    /* Render black empty board */
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Render lines */
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    for(int i = 1; i < 4; i++){
        SDL_RenderDrawLine(this->renderer,
                           150 * i + i,
                           0,
                           150 * i + i,
                           150 * 4 + 3);

        SDL_RenderDrawLine(this->renderer,
                           0,
                           150 * i + i,
                           150 * 4 + 3,
                           150 * i + i);
    }

    SDL_RenderPresent(renderer);
}
