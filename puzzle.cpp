#include "puzzle.hpp"

puzzle::puzzle(SDL_Renderer *renderer)
    :renderer(renderer),
     winColor({.r = 0, .g = 255, .b = 0}),
     endGame(false),
     gameOver(false)
{
    this->defaultFont = TTF_OpenFont("Arial.ttf", 72);
    this->fontColor = {255, 255, 255, 255};
    this->tiles = (int*)calloc(16, sizeof(int));

    /* Initialize tiles */
    for(int i = 0; i < 16; i++){
        this->tiles[i] = i;
    }

    /* Shuffle tiles (looks random)*/
    shuffle();
}

void puzzle::shuffle(){
    int currentPos, lastPos = 0;
    srand(time(0));

    /* Initialize tiles again, because when shuffling completed game, it's already solvable */
    for(int i = 0; i < 16; i++){
        this->tiles[i] = i;
    }

    while(!isSolvable()) {
        for(int i = 0; i < 100; i++){
            currentPos = rand()%16;
            int tmp = this->tiles[currentPos];
            this->tiles[currentPos] = this->tiles[lastPos];
            this->tiles[lastPos] = tmp;
            lastPos = currentPos;
        }
    }

}

puzzle::~puzzle(){
    free(this->tiles);
}

void puzzle::render(){
    SDL_Color white = {255, 255, 255, 255};
    render(white);
}

void puzzle::render(SDL_Color color){
    /* Render black empty board */
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Render lines */
    SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
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

    renderValue(color);
}

void puzzle::renderValue(){
    renderValue(this->fontColor);
}

void puzzle::renderValue(SDL_Color color){
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
                                                  color);

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
    if(this->gameOver){
        restart();
    }
    else {
        swapTiles(x, y);
        render();
        if(isGameOver()){
            render(this->winColor);
            this->gameOver = true;
        }
    }
    SDL_RenderPresent(renderer);
}

bool puzzle::isGameOver(){
    for(int i = 0; i < 16; i++){
        if(this->tiles[i] != (i+1)%16)
            return false;
    }

    return true;
}

bool puzzle::getEndGame(){
    return this->endGame;
}

void puzzle::quit(){
    this->endGame = true;
}

bool puzzle::isSolvable(){
    /* This site has very good explanation */
    /* www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html */

    int inversions = 0;
    for(int i = 0; i < 15; i++){
        int currentValue = this->tiles[i];
        for(int j = i + 1; j < 16; j++){
            /* Skip blank tile */
            if(currentValue && tiles[j] && currentValue > this->tiles[j])
                inversions++;
        }
    }

    bool evenBlankPositionFromBottom;

    for(int i = 3; i >= 0; i--){
        for(int j = 0; j < 4; j++){
            if(this->tiles[i * 4 + j] == 0){
                evenBlankPositionFromBottom = (i % 2 == 0);
                goto exitNestedForLoops; /* lmao, this is the first time I used goto, I hope it's okay in this situation */
            }
        }
    }

 exitNestedForLoops:
    if(evenBlankPositionFromBottom)
        return inversions % 2 == 1;
    else
        return inversions % 2 == 0;
}

void puzzle::restart(){
    this->gameOver = false;
    this->endGame = false;
    shuffle();
    render();
}
