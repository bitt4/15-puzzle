#include "puzzle.hpp"

Puzzle::Puzzle(const std::string &font)
    :m_font { TTF_OpenFont(font.c_str(), 72) },
     m_base_path { SDL_GetBasePath() }
{
    /* Catch errors */
    if(!m_font){
        print_format_error("An error occured while loading font '%s': %s", font.c_str(), TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* Initialize tiles */
    const int number_of_tiles = m_size * m_size;
    m_tiles.resize(number_of_tiles);
    for(int i = 0; i < number_of_tiles; ++i){
        m_tiles[i] = i;
    }

    /* Shuffle tiles (looks random)*/
    shuffle();
}

Puzzle::~Puzzle(){
    // TTF_CloseFont(m_font);
    // I got segfault when calling TTF_CloseFont(m_font),
    // but when I removed TTF_Quit() in main.cpp, the segfault disappeared
}

void Puzzle::swap_tiles(const int x, const int y){
    const int current_position = y * m_size + x;

    /* return immediately if user clicks on empty tile */
    if(m_tiles[current_position] == 0)
        return;

    /* Search for empty tile */
    Point empty = get_empty_tile();
    int emptyTilePosX = empty.x,
        emptyTilePosY = empty.y;

    /* Check if target tile is next to empty tile */
    if(abs(emptyTilePosX - x) + abs(emptyTilePosY - y) == 1){
        int temp = m_tiles[current_position];
        m_tiles[current_position] = 0;
        m_tiles[emptyTilePosY * m_size + emptyTilePosX] = temp;
    }
}

void Puzzle::render_value() const {
    render_value(m_font_color);
}

void Puzzle::render_value(const SDL_Color &color) const {
    SDL_Surface* cellText = 0;
    SDL_Texture* cellTexture = 0;

    for(int y = 0; y < m_size; ++y){
        for(int x = 0; x < m_size; ++x){
            const int current_cell_position = y * m_size + x;
            if(m_tiles[current_cell_position]){
                int textWidth, textHeight;

                TTF_SizeText(m_font,
                             std::to_string(m_tiles[current_cell_position]).c_str(),
                             &textWidth,
                             &textHeight);

                cellText = TTF_RenderText_Blended(m_font,
                                                  std::to_string(m_tiles[current_cell_position]).c_str(),
                                                  color);

                cellTexture = SDL_CreateTextureFromSurface(m_renderer, cellText);

                SDL_Rect cellRectangle;
                cellRectangle.x = x * m_tile_width + x + (m_tile_width - textWidth) / 2;
                cellRectangle.y = y * m_tile_width + y + (m_tile_width - textHeight) / 2;
                cellRectangle.w = textWidth;
                cellRectangle.h = textHeight;

                SDL_RenderCopy(m_renderer, cellTexture, NULL, &cellRectangle);
            }
        }
    }

    SDL_FreeSurface(cellText);
    SDL_DestroyTexture(cellTexture);
}

bool Puzzle::is_game_over() const {
    const int number_of_tiles = m_size * m_size;
    for(int i = 0; i < number_of_tiles; ++i){
        if(m_tiles[i] != (i + 1) % number_of_tiles)
            return false;
    }

    return true;
}

bool Puzzle::is_solvable() const {
    /* This site has very good explanation */
    /* https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/ */

    const int number_of_tiles = m_size * m_size;
    int inversions = 0;
    for(int i = 0; i < number_of_tiles - 1; ++i){
        int currentValue = m_tiles[i];
        for(int j = i + 1; j < number_of_tiles; ++j){
            /* Skip blank tile */
            if(currentValue && m_tiles[j] && currentValue > m_tiles[j])
                inversions++;
        }
    }

    bool evenBlankPositionFromBottom;

    Point empty = get_empty_tile();

    evenBlankPositionFromBottom = (empty.y % 2 == 0);

    if(m_size & 1){
        return !(inversions & 1);
    } else {
        if(evenBlankPositionFromBottom)
            return inversions & 1;
        else
            return !(inversions & 1);
    }
}

void Puzzle::shuffle(){
    int currentPos, lastPos = 0;
    srand(time(0));
    const int number_of_tiles = m_size * m_size;

    do {
        for(int i = 0; i < 100; ++i){
            currentPos = rand() % number_of_tiles;
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

Point Puzzle::get_empty_tile() const {
    Point empty;

    for(int y = 0; y < m_size; ++y){
        for(int x = 0; x < m_size; ++x){
            if(!m_tiles[y * m_size + x]){
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

std::string Puzzle::get_path(const std::string &filename) const {
    return m_base_path + filename;
}

void Puzzle::set_renderer(SDL_Renderer* renderer){
    m_renderer = renderer;
}

void Puzzle::render() const {
    SDL_Color white = {255, 255, 255, 255};
    render(white);
}

void Puzzle::render(const SDL_Color &color) const {
    /* Render black empty board */
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);

    /* Render lines */
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    for(int i = 1; i < m_size; ++i){
        SDL_RenderDrawLine(m_renderer,
                           m_tile_width * i + i,
                           0,
                           m_tile_width * i + i,
                           m_tile_width * m_size + m_size - 1);

        SDL_RenderDrawLine(m_renderer,
                           0,
                           m_tile_width * i + i,
                           m_tile_width * m_size + m_size - 1,
                           m_tile_width * i + i);
    }

    render_value(color);
}

void Puzzle::click(const int x, const int y){
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

void Puzzle::keydown(const SDL_Keycode key){
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
            if(empty.y + 1 < m_size){
                swap_tiles(empty.x, empty.y+1);
                goto exit;
            }
            break;
        }
        case SDLK_LEFT: {
            Point empty = get_empty_tile();
            if(empty.x + 1 < m_size){
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

int Puzzle::get_window_width() const {
    return m_size * (m_tile_width + 1) - 1;
    // width of one tile is 'm_tile_width', number of tiles in one row/column is 'm_size', and there are 'm_size - 1' lines
    // between tiles in one row/column, which simplifies to the expression above
}

bool Puzzle::get_end_game() const {
    return m_end_game;
}

void Puzzle::quit(){
    m_end_game = true;
}
