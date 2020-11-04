# 15-puzzle

15-puzzle, also called 'Tiles Game' or 'Puzzle of Fifteen' is sliding puzzle game, that consists of 15 numbered square tiles and one empty tile. The goal of the puzzle is to order the tiles by sliding, swapping with the empty tile.

## Installation

### Dependencies

 - [SDL2](https://www.libsdl.org/)
 - [SDL2 ttf](https://www.libsdl.org/projects/SDL_ttf/)

### Setup

```bash
git clone https://github.com/bitt4/15-puzzle.git
cd 15-puzzle
make
./15-puzzle
```

## Usage

### Options
Option                    | Effect
--------------------------|--------------------------
`--font PATH`, `-f PATH`  | set font file
`--help`, `-h`            | display help

### Controls
You can either click on tile to move it to empty space, or use the arrow keys.

|Key             |Description|
|----------------|-----------|
|<kbd>🠉</kbd>    |Swipe up   |
|<kbd>🠋</kbd>    |Swipe down |
|<kbd>🠊</kbd>    |Swipe right|
|<kbd>🠈</kbd>    |Swipe left |

The arrow controls might be a little difficult to use from the beginning.
If that's the case, imagine it as using your finger to move the tiles.

## Example

### Unsolved
![](demos/1.png)
### Solved
![](demos/2.png)
