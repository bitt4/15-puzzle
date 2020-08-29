#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class puzzle{
private:
    SDL_Renderer *renderer;
    TTF_Font* defaultFont;
    SDL_Color fontColor;
public:
    puzzle(SDL_Renderer* renderer);
    void renderInit();
    void renderValue();
};
