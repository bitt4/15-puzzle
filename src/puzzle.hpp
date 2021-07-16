#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <cstdarg>
#include <vector>

typedef struct {
    int x;
    int y;
} Point;

class Puzzle {
private:
    const int m_size { 4 };
    const int m_tile_width { 150 };
    SDL_Renderer *m_renderer;
    std::vector<int> m_tiles;
    TTF_Font* m_font;
    const SDL_Color m_font_color { 255, 255, 255, 255 };
    const SDL_Color m_win_color { 0, 255, 0, 0 };
    bool m_end_game { false };
    bool m_game_over { false };
    const std::string m_base_path;

    void swap_tile_with_empty(const int target_x, const int target_y);
    void render_value() const;
    void render_value(const SDL_Color &color) const;
    bool is_game_over() const;
    bool is_solvable() const;
    void shuffle();
    void restart();
    Point get_empty_tile() const;
    void show_error(const std::string &error) const;

public:
    Puzzle(const std::string &font);
    ~Puzzle();
    std::string get_path(const std::string& filename) const;
    int get_window_width() const;
    void set_renderer(SDL_Renderer* renderer);
    void render() const;
    void render(const SDL_Color &color) const;
    void click(const int x, const int y);
    void keydown(const SDL_Keycode key);
    bool get_end_game() const;
    void quit();
};
