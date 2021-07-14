#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <cstdarg>

typedef struct {
    int x;
    int y;
} Point;

class Puzzle {
private:
    SDL_Renderer *m_renderer;
    int *m_tiles;
    TTF_Font* m_font;
    SDL_Color m_font_color, m_win_color;
    bool m_end_game;
    bool m_game_over;
    const char* m_base_path;
    char* m_current_file_path;

    void swap_tiles(int target_x, int target_y);
    void render_value();
    void render_value(SDL_Color color);
    bool is_game_over();

    /*
     * source: www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html
     */
    bool is_solvable();
    void shuffle();
    void restart();
    Point get_empty_tile();
    void print_format_error(const char* format_string, ...);

public:
    Puzzle(const char* font);
    ~Puzzle();
    const char* get_path(const char* filename);
    void set_renderer(SDL_Renderer* renderer);
    void render();
    void render(SDL_Color color);
    void click(int x, int y);
    void keydown(SDL_Keycode key);
    bool get_end_game();
    void quit();
};
