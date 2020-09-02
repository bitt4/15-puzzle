#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdlib>

class puzzle{
private:
    SDL_Renderer *renderer;
    TTF_Font* defaultFont;
    SDL_Color fontColor;
    int *tiles;

    /* Swap target tile with the empty one */
    void swapTiles(int target_x, int target_y);
public:
    puzzle(SDL_Renderer* renderer);
    ~puzzle();
    void render();
    void renderValue();
    void click(int x, int y);
};
