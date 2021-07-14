#include "puzzle.hpp"

Puzzle::Puzzle(const char* font)
    :m_tiles((int*)calloc(16, sizeof(int))),
     m_endGame(false),
     m_gameOver(false),
     m_basePath(SDL_GetBasePath()),
     m_currentFilePath((char*)malloc(0)) /* malloc(0) is used so it can be reallocated and resized later */
{
    this->m_fontColor.r = 255;
    this->m_fontColor.g = 255;
    this->m_fontColor.b = 255;
    this->m_fontColor.a = 255;

    this->m_winColor.r = 0;
    this->m_winColor.g = 255;
    this->m_winColor.b = 0;

    /* Select font */
    this->m_font = TTF_OpenFont(font, 72);

    /* Catch errors */
    if(!this->m_font){
        printFormatError("An error occured while loading font '%s': %s", font, TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* Initialize tiles */
    for(int i = 0; i < 16; i++){
        this->m_tiles[i] = i;
    }

    /* Shuffle tiles (looks random)*/
    shuffle();
}

Puzzle::~Puzzle(){
    free(this->m_tiles);
    free(this->m_currentFilePath);
}

void Puzzle::swapTiles(int x, int y){
    /* return immediately if user clicks on empty tile */
    if(this->m_tiles[y * 4 +x] == 0)
        return;

    /* Search for empty tile */
    Point empty = getEmptyTile();
    int emptyTilePosX = empty.x,
        emptyTilePosY = empty.y;

    /* Check if target tile is next to empty tile */
    if(abs(emptyTilePosX - x) + abs(emptyTilePosY - y) == 1){
        int temp = this->m_tiles[y * 4 + x];
        this->m_tiles[y * 4 + x] = 0;
        this->m_tiles[emptyTilePosY * 4 + emptyTilePosX] = temp;
    }
}

void Puzzle::renderValue(){
    renderValue(this->m_fontColor);
}

void Puzzle::renderValue(SDL_Color color){
    SDL_Surface* cellText = 0;
    SDL_Texture* cellTexture = 0;

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(this->m_tiles[y * 4 + x]){
                int textWidth, textHeight;

                TTF_SizeText(this->m_font,
                             std::to_string(this->m_tiles[y * 4 + x]).c_str(),
                             &textWidth,
                             &textHeight);

                cellText = TTF_RenderText_Blended(this->m_font,
                                                  std::to_string(this->m_tiles[y * 4 + x]).c_str(),
                                                  color);

                cellTexture = SDL_CreateTextureFromSurface(this->m_renderer, cellText);

                SDL_Rect cellRectangle;
                cellRectangle.x = x * 150 + x + (150-textWidth)/2;
                cellRectangle.y = y * 150 + y + (150-textHeight)/2;
                cellRectangle.w = textWidth;
                cellRectangle.h = textHeight;

                SDL_RenderCopy(this->m_renderer, cellTexture, NULL, &cellRectangle);
            }
        }
    }

    SDL_FreeSurface(cellText);
    SDL_DestroyTexture(cellTexture);
}

bool Puzzle::isGameOver(){
    for(int i = 0; i < 16; i++){
        if(this->m_tiles[i] != (i+1)%16)
            return false;
    }

    return true;
}

bool Puzzle::isSolvable(){
    /* This site has very good explanation */
    /* www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html */

    int inversions = 0;
    for(int i = 0; i < 15; i++){
        int currentValue = this->m_tiles[i];
        for(int j = i + 1; j < 16; j++){
            /* Skip blank tile */
            if(currentValue && m_tiles[j] && currentValue > this->m_tiles[j])
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

void Puzzle::shuffle(){
    int currentPos, lastPos = 0;
    srand(time(0));

    do {
        for(int i = 0; i < 100; i++){
            currentPos = rand()%16;
            int tmp = this->m_tiles[currentPos];
            this->m_tiles[currentPos] = this->m_tiles[lastPos];
            this->m_tiles[lastPos] = tmp;
            lastPos = currentPos;
        }
    }
    while(!isSolvable());
}

void Puzzle::restart(){
    this->m_gameOver = false;
    shuffle();
    render();
}

Point Puzzle::getEmptyTile(){
    Point empty;

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(!this->m_tiles[y * 4 + x]){
                empty.x = x;
                empty.y = y;
                goto getEmptyTileExit;
            }
        }
    }
 getEmptyTileExit:
    return empty;
}

void Puzzle::printFormatError(const char* format_string, ...){

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

const char* Puzzle::getPath(const char* filename) {
    int size = (strlen(this->m_basePath) + strlen(filename) + 1) * sizeof(char);
    this->m_currentFilePath = (char*)realloc(this->m_currentFilePath, size);
    memset(this->m_currentFilePath, 0, size);
    strcpy(this->m_currentFilePath, this->m_basePath);
    strcat(this->m_currentFilePath, filename);
    return this->m_currentFilePath;
}

void Puzzle::setRenderer(SDL_Renderer* renderer){
    this->m_renderer = renderer;
}

void Puzzle::render(){
    SDL_Color white = {255, 255, 255, 255};
    render(white);
}

void Puzzle::render(SDL_Color color){
    /* Render black empty board */
    SDL_SetRenderDrawColor(this->m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);

    /* Render lines */
    SDL_SetRenderDrawColor(this->m_renderer, color.r, color.g, color.b, color.a);
    for(int i = 1; i < 4; i++){
        SDL_RenderDrawLine(this->m_renderer,
                           150 * i + i,
                           0,
                           150 * i + i,
                           150 * 4 + 3);

        SDL_RenderDrawLine(this->m_renderer,
                           0,
                           150 * i + i,
                           150 * 4 + 3,
                           150 * i + i);
    }

    renderValue(color);
}

void Puzzle::click(int x, int y){
    if(this->m_gameOver){
        restart();
    }
    else {
        swapTiles(x, y);
        render();
        if(isGameOver()){
            render(this->m_winColor);
            this->m_gameOver = true;
        }
    }
    SDL_RenderPresent(m_renderer);
}

void Puzzle::keydown(SDL_Keycode key){
    if(this->m_gameOver){
        restart();
        SDL_RenderPresent(m_renderer);
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
            render(this->m_winColor);
            this->m_gameOver = true;
        }
        else {
            render();
        }
        SDL_RenderPresent(m_renderer);
    }
}

bool Puzzle::getEndGame(){
    return this->m_endGame;
}

void Puzzle::quit(){
    this->m_endGame = true;
}
