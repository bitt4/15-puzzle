#include <SDL2/SDL.h>

class puzzle{
private:
    SDL_Renderer *renderer;
public:
    puzzle(SDL_Renderer* renderer);
    void renderInit();
};
