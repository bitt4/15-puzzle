#include "puzzle.hpp"

Puzzle::Puzzle(const char* font)
    :m_tiles((int*)calloc(16, sizeof(int))),
     m_end_game(false),
     m_game_over(false),
     m_base_path(SDL_GetBasePath()),
     m_current_file_path((char*)malloc(0)) /* malloc(0) is used so it can be reallocated and resized later */
{
    m_font_color.r = 255;
    m_font_color.g = 255;
    m_font_color.b = 255;
    m_font_color.a = 255;

    m_win_color.r = 0;
    m_win_color.g = 255;
    m_win_color.b = 0;

    /* Select font */
    m_font = TTF_OpenFont(font, 72);

    /* Catch errors */
    if(!m_font){
        print_format_error("An error occured while loading font '%s': %s", font, TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* Initialize tiles */
    for(int i = 0; i < 16; i++){
        m_tiles[i] = i;
    }

    /* Shuffle tiles (looks random)*/
    shuffle();
}

Puzzle::~Puzzle(){
    free(m_tiles);
    free(m_current_file_path);
}

void Puzzle::swap_tiles(int x, int y){
    /* return immediately if user clicks on empty tile */
    if(m_tiles[y * 4 +x] == 0)
        return;

    /* Search for empty tile */
    Point empty = get_empty_tile();
    int emptyTilePosX = empty.x,
        emptyTilePosY = empty.y;

    /* Check if target tile is next to empty tile */
    if(abs(emptyTilePosX - x) + abs(emptyTilePosY - y) == 1){
        int temp = m_tiles[y * 4 + x];
        m_tiles[y * 4 + x] = 0;
        m_tiles[emptyTilePosY * 4 + emptyTilePosX] = temp;
    }
}

void Puzzle::render_value(){
    render_value(m_font_color);
}

void Puzzle::render_value(SDL_Color color){
    SDL_Surface* cellText = 0;
    SDL_Texture* cellTexture = 0;

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(m_tiles[y * 4 + x]){
                int textWidth, textHeight;

                TTF_SizeText(m_font,
                             std::to_string(m_tiles[y * 4 + x]).c_str(),
                             &textWidth,
                             &textHeight);

                cellText = TTF_RenderText_Blended(m_font,
                                                  std::to_string(m_tiles[y * 4 + x]).c_str(),
                                                  color);

                cellTexture = SDL_CreateTextureFromSurface(m_renderer, cellText);

                SDL_Rect cellRectangle;
                cellRectangle.x = x * 150 + x + (150-textWidth)/2;
                cellRectangle.y = y * 150 + y + (150-textHeight)/2;
                cellRectangle.w = textWidth;
                cellRectangle.h = textHeight;

                SDL_RenderCopy(m_renderer, cellTexture, NULL, &cellRectangle);
            }
        }
    }

    SDL_FreeSurface(cellText);
    SDL_DestroyTexture(cellTexture);
}

bool Puzzle::is_game_over(){
    for(int i = 0; i < 16; i++){
        if(m_tiles[i] != (i+1)%16)
            return false;
    }

    return true;
}

bool Puzzle::is_solvable(){
    /* This site has very good explanation */
    /* www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html */

    int inversions = 0;
    for(int i = 0; i < 15; i++){
        int currentValue = m_tiles[i];
        for(int j = i + 1; j < 16; j++){
            /* Skip blank tile */
            if(currentValue && m_tiles[j] && currentValue > m_tiles[j])
                inversions++;
        }
    }

    bool evenBlankPositionFromBottom;

    Point empty = get_empty_tile();

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
            int tmp = m_tiles[currentPos];
            m_tiles[currentPos] = m_tiles[lastPos];
            m_tiles[lastPos] = tmp;
            lastPos = currentPos;
        }
    }
    while(!is_solvable());
}

void Puzzle::restart(){
    m_game_over = false;
    shuffle();
    render();
}

Point Puzzle::get_empty_tile(){
    Point empty;

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(!m_tiles[y * 4 + x]){
                empty.x = x;
                empty.y = y;
                goto getEmptyTileExit;
            }
        }
    }
 getEmptyTileExit:
    return empty;
}

void Puzzle::print_format_error(const char* format_string, ...){

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

const char* Puzzle::get_path(const char* filename) {
    int size = (strlen(m_base_path) + strlen(filename) + 1) * sizeof(char);
    m_current_file_path = (char*)realloc(m_current_file_path, size);
    memset(m_current_file_path, 0, size);
    strcpy(m_current_file_path, m_base_path);
    strcat(m_current_file_path, filename);
    return m_current_file_path;
}

void Puzzle::set_renderer(SDL_Renderer* renderer){
    m_renderer = renderer;
}

void Puzzle::render(){
    SDL_Color white = {255, 255, 255, 255};
    render(white);
}

void Puzzle::render(SDL_Color color){
    /* Render black empty board */
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);

    /* Render lines */
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    for(int i = 1; i < 4; i++){
        SDL_RenderDrawLine(m_renderer,
                           150 * i + i,
                           0,
                           150 * i + i,
                           150 * 4 + 3);

        SDL_RenderDrawLine(m_renderer,
                           0,
                           150 * i + i,
                           150 * 4 + 3,
                           150 * i + i);
    }

    render_value(color);
}

void Puzzle::click(int x, int y){
    if(m_game_over){
        restart();
    }
    else {
        swap_tiles(x, y);
        render();
        if(is_game_over()){
            render(m_win_color);
            m_game_over = true;
        }
    }
    SDL_RenderPresent(m_renderer);
}

void Puzzle::keydown(SDL_Keycode key){
    if(m_game_over){
        restart();
        SDL_RenderPresent(m_renderer);
    }
    else {
        switch(key){
        case SDLK_DOWN: {
            Point empty = get_empty_tile();
            if(empty.y - 1 >= 0){
                swap_tiles(empty.x, empty.y-1);
                goto exit;
            }
            break;
        }
        case SDLK_UP: {
            Point empty = get_empty_tile();
            if(empty.y + 1 <= 3){
                swap_tiles(empty.x, empty.y+1);
                goto exit;
            }
            break;
        }
        case SDLK_LEFT: {
            Point empty = get_empty_tile();
            if(empty.x + 1 <= 3){
                swap_tiles(empty.x+1, empty.y);
                goto exit;
            }
            break;
        }
        case SDLK_RIGHT: {
            Point empty = get_empty_tile();
            if(empty.x - 1 >= 0){
                swap_tiles(empty.x-1, empty.y);
                goto exit;
            }
            break;
        }
        default: {}
        }

    exit:
        if(is_game_over()){
            render(m_win_color);
            m_game_over = true;
        }
        else {
            render();
        }
        SDL_RenderPresent(m_renderer);
    }
}

bool Puzzle::get_end_game(){
    return m_end_game;
}

void Puzzle::quit(){
    m_end_game = true;
}
