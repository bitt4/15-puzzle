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

class puzzle{
private:
    SDL_Renderer *renderer;

    /* Font by which numbers are rendered */
    TTF_Font* defaultFont;

    /* Colors for rendering numbers in tiles.
     * fontColor: defines default font color of numbers in tiles,
     *            used in puzzle::renderValue(SDL_Color)
     *            as default value
     * winColor: used as argument in puzzle::render(SDL_Color),
     *           when the puzzle is completed
     */
    SDL_Color fontColor, winColor;

    /*
     * Contains tiles
     */
    int *tiles;

    /*
     * Used to stop the game loop.
     * When user closes the window, `endGame` is set
     * to true, terminating the game loop.
     * Returned by puzzle::getEndGame() (getter).
     */
    bool endGame;

    /*
     * Specifies state of puzzle.
     * True, if the puzzle is completed, otherwise false.
     * Used to restart the puzzle.
     */
    bool gameOver;

    const char* basePath;
    char* currentFilePath;

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

    /*
     * Restarts state of game:
     * sets `gameOver` and `endGame` to false,
     * shuffles tiles with puzzle::shuffle(),
     * and calls puzzle::render() to render game
     */
    void restart();

    /*
     * Searches for empty tile. Returns `Point`
     * structure, which contains x (column)
     * and y (row) array coordinates in `tiles` array
     */
    Point getEmptyTile();

    void printFormatError(const char* format_string, ...);

public:
    puzzle();
    ~puzzle();
    const char* getPath(const char* filename);
    void setRenderer(SDL_Renderer* renderer);
    void render();
    void render(SDL_Color color);
    void click(int x, int y);
    void keydown(SDL_Keycode key);
    bool getEndGame();
    void quit();
};
