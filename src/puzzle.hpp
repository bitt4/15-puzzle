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
    TTF_Font* m_font;
    SDL_Color m_fontColor, m_winColor;
    int *m_tiles;
    bool m_endGame;
    bool m_gameOver;
    const char* m_basePath;
    char* m_currentFilePath;

    void swapTiles(int target_x, int target_y);
    void renderValue();
    void renderValue(SDL_Color color);
    bool isGameOver();

    /*
     * source: www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html
     */
    bool isSolvable();
    void shuffle();
    void restart();
    Point getEmptyTile();
    void printFormatError(const char* format_string, ...);

public:
    Puzzle(const char* font);
    ~Puzzle();
    const char* getPath(const char* filename);
    void setRenderer(SDL_Renderer* renderer);
    void render();
    void render(SDL_Color color);
    void click(int x, int y);
    void keydown(SDL_Keycode key);
    bool getEndGame();
    void quit();
};
