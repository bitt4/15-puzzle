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
    SDL_Color fontColor;
    int *tiles;
    bool endGame;

    /* Swap target tile with the empty one */
    void swapTiles(int target_x, int target_y);
    void renderValue();
    bool isGameOver();
    bool isSolvable();
public:
    puzzle(SDL_Renderer* renderer);
    ~puzzle();
    void render();
    void click(int x, int y);
    bool getEndGame();
    void quit();
};
