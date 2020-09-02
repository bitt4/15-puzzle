#include "puzzle.hpp"

puzzle::puzzle(SDL_Renderer *renderer)
    :renderer(renderer)
{
    this->defaultFont = TTF_OpenFont("Arial.ttf", 72);
    this->fontColor = {255, 255, 255, 255};
    this->tiles = (int*)calloc(16, sizeof(int));

    /* Initialize tiles */
    /* TODO: This will be randomized later */
    for(int i = 0; i < 16; i++){
        this->tiles[i] = i;
    }
}

puzzle::~puzzle(){
    free(this->tiles);
}

void puzzle::render(){
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

    renderValue();
    SDL_RenderPresent(renderer);
}

void puzzle::renderValue(){
    SDL_Surface* cellText;
    SDL_Texture* cellTexture;

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(this->tiles[y * 4 + x]){
                int textWidth, textHeight;

                TTF_SizeText(this->defaultFont,
                             std::to_string(this->tiles[y * 4 + x]).c_str(),
                             &textWidth,
                             &textHeight);

                cellText = TTF_RenderText_Blended(this->defaultFont,
                                                  std::to_string(this->tiles[y * 4 + x]).c_str(),
                                                  this->fontColor);

                cellTexture = SDL_CreateTextureFromSurface(this->renderer, cellText);

                SDL_Rect cellRectangle;
                cellRectangle.x = x * 150 + x + (150-textWidth)/2;
                cellRectangle.y = y * 150 + y + (150-textHeight)/2;
                cellRectangle.w = textWidth;
                cellRectangle.h = textHeight;

                SDL_RenderCopy(this->renderer, cellTexture, NULL, &cellRectangle);
            }
        }
    }

    SDL_FreeSurface(cellText);
    SDL_DestroyTexture(cellTexture);
}

void puzzle::swapTiles(int x, int y){
    /* return immediately if user clicks on empty tile */
    if(this->tiles[y * 4 +x] == 0)
        return;

    /* Search for empty tile */
    int emptyTilePosX, emptyTilePosY;
    for(int emptyY = 0; emptyY < 4; emptyY++){
        for(int emptyX = 0; emptyX < 4; emptyX++){
            if(!this->tiles[emptyY * 4 + emptyX]){
                emptyTilePosX = emptyX;
                emptyTilePosY = emptyY;
                break; /* Since there is only one empty tile */
            }
        }
    }

    /* Check if target tile is next to empty tile */
    if(abs(emptyTilePosX - x) + abs(emptyTilePosY - y) == 1){
        int temp = this->tiles[y * 4 + x];
        this->tiles[y * 4 + x] = 0;
        this->tiles[emptyTilePosY * 4 + emptyTilePosX] = temp;
    }
}

void puzzle::click(int x, int y){
    swapTiles(x, y);
    render();
}
