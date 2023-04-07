#pragma once
#include <SDL.h>
#include <vector>
#include "Tetromino.h"
#include "ResourceManager.h"

enum class TetrominoType;
class Tetromino;

struct Color{
    int r, g, b, a;
};

struct GridElement{
    TetrominoType type;
    bool isStatic = true;
};

class TetrisWindow{
public:
    TetrisWindow(int BLOCK_SIZE, int BLOCKS_X, int BLOCKS_Y, int* points, SDL_Renderer* renderer,
                 std::shared_ptr<ResourceManager> resourceManager);

    void renderLoop();
    void gameLoop();

    void onKeyPress(SDL_Keycode key);
    void onKeyRelease(SDL_Keycode key);

    int getWidth() const { return WIDTH; }
    int getHeight() const { return HEIGHT; }

    int getBlockPreviewWidth() const { return NEXT_PREVIEW_WIDTH;};
    int getBlockPreviewHeight() const { return NEXT_PREVIEW_HEIGHT;};

    SDL_Texture* getTexture() const { return texture; }
    SDL_Texture* getBlockPreviewTexture() const { return nextBlockPreviewTexture; }

    bool isGameOver() const { return gameOver;};

private:
    const int WIDTH, HEIGHT;

    const int BLOCK_SIZE, BLOCKS_X, BLOCKS_Y;
    int NEXT_PREVIEW_HEIGHT;
    int NEXT_PREVIEW_WIDTH;

    std::vector<std::vector<GridElement>> grid, nextBlockGrid;

    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* nextBlockPreviewTexture;

    std::unique_ptr<Tetromino> currentBlock, nextBlock;

    std::shared_ptr<ResourceManager> resourceManager;

    void newBlock();
    void checkRows();

    int* points;

    bool gameOver = false;

    static std::unique_ptr<Tetromino> getRandomBlock();
    void renderGrid(SDL_Texture* texture, std::vector<std::vector<GridElement>> grid, Tetromino* dynamicBlock);
};