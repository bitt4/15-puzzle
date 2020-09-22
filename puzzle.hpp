#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdlib>
#include <random>
#include <ctime>

typedef struct {
    int x;
    int y;
} Point;

class puzzle{
private:
    SDL_Renderer *renderer;
    TTF_Font* defaultFont;
    SDL_Color fontColor, winColor;
    int *tiles;
    bool endGame, gameOver;

    /* Swap target tile with the empty one */
    void swapTiles(int target_x, int target_y);

    /*
     * Renders numbers in tiles by `fontColor`,
     * which is white by default.
     * To render numbers by arbitrary color,
     * use puzzle::renderValue(SDL_Color color)
     */
    void renderValue();

    /*
     * Renders numbers in tiles by arbitrary
     * color specified by `color` argument
     */
    void renderValue(SDL_Color color);

    /*
     * Returns true, if the puzzle is completed,
     * otherwise returns false
     */
    bool isGameOver();

    /*
     * Returns true, if current array `tiles`
     * contains solvable configuration of tiles,
     * source: www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html
     */
    bool isSolvable();

    /*
     * Shuffles `tiles` array (swaps random tiles)
     */
    void shuffle();
    void restart();
    Point getEmptyTile();
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
