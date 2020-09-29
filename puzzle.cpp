#include "puzzle.hpp"

puzzle::puzzle()
    /* TODO: Make font selectable */
    :fontColor({.r = 255, .g = 255, .b = 255, .a = 255}),
     winColor({.r = 0, .g = 255, .b = 0}),
     tiles((int*)calloc(16, sizeof(int))),
     endGame(false),
     gameOver(false)
{
    /* Select font */
    this->defaultFont = TTF_OpenFont("font/Arial.ttf", 72);
    if(!this->defaultFont){            /* Catch errors */
        printFormatError("An error occured while loading font: %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* Initialize tiles */
    for(int i = 0; i < 16; i++){
        this->tiles[i] = i;
    }

    /* Shuffle tiles (looks random)*/
    shuffle();
}

void puzzle::setRenderer(SDL_Renderer* renderer){
    this->renderer = renderer;
}

void puzzle::shuffle(){
    int currentPos, lastPos = 0;
    srand(time(0));

    do {
        for(int i = 0; i < 100; i++){
            currentPos = rand()%16;
            int tmp = this->tiles[currentPos];
            this->tiles[currentPos] = this->tiles[lastPos];
            this->tiles[lastPos] = tmp;
            lastPos = currentPos;
        }
    }
    while(!isSolvable());
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
    SDL_Surface* cellText = 0;
    SDL_Texture* cellTexture = 0;

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
    Point empty = getEmptyTile();
    int emptyTilePosX = empty.x,
        emptyTilePosY = empty.y;

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

void puzzle::keydown(SDL_Keycode key){
    if(this->gameOver){
        restart();
        SDL_RenderPresent(renderer);
    }
    else {
        switch(key){
        case SDLK_DOWN: {
            Point empty = getEmptyTile();
            if(empty.y - 1 >= 0){
                swapTiles(empty.x, empty.y-1);
                goto exit;
            }
            break;
        }
        case SDLK_UP: {
            Point empty = getEmptyTile();
            if(empty.y + 1 <= 3){
                swapTiles(empty.x, empty.y+1);
                goto exit;
            }
            break;
        }
        case SDLK_LEFT: {
            Point empty = getEmptyTile();
            if(empty.x + 1 <= 3){
                swapTiles(empty.x+1, empty.y);
                goto exit;
            }
            break;
        }
        case SDLK_RIGHT: {
            Point empty = getEmptyTile();
            if(empty.x - 1 >= 0){
                swapTiles(empty.x-1, empty.y);
                goto exit;
            }
            break;
        }
        default: {}
        }

    exit:
        if(isGameOver()){
            render(this->winColor);
            this->gameOver = true;
        }
        else {
            render();
        }
        SDL_RenderPresent(renderer);
    }
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

    Point empty = getEmptyTile();

    evenBlankPositionFromBottom = (empty.y % 2 == 0);

    if(evenBlankPositionFromBottom)
        return inversions % 2 == 1;
    else
        return inversions % 2 == 0;
}

void puzzle::restart(){
    this->gameOver = false;
    shuffle();
    render();
}

Point puzzle::getEmptyTile(){
    Point empty;

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(!this->tiles[y * 4 + x]){
                empty.x = x;
                empty.y = y;
                goto getEmptyTileExit;
            }
        }
    }
 getEmptyTileExit:
    return empty;
}

void puzzle::printFormatError(const char* format_string, ...){

    char error_message[256];
    va_list args;
    va_start(args, format_string);

    int buffer_size = vsnprintf(error_message, 256, format_string, args);
    if (buffer_size < 0) {
        fprintf(stderr, "An error occured\n");
    }

    va_end(args);

#ifdef _WIN32
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             error_message,
                             NULL);
#else
    fprintf(stderr, "%s\n", error_message);
#endif
}
