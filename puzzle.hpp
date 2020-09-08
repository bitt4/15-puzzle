#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdlib>
#include <random>
#include <ctime>

class puzzle{
private:
    SDL_Renderer *renderer;
    TTF_Font* defaultFont;
    SDL_Color fontColor, winColor;
    int *tiles;
    bool endGame, gameOver;

    /* Swap target tile with the empty one */
    void swapTiles(int target_x, int target_y);
    void renderValue();
    void renderValue(SDL_Color color);
    bool isGameOver();
    bool isSolvable();
    void shuffle();
    void restart();
public:
    puzzle(SDL_Renderer* renderer);
    ~puzzle();
    void render();
    void render(SDL_Color color);
    void click(int x, int y);
    void keydown(SDL_Keycode key);
    bool getEndGame();
    void quit();
};
