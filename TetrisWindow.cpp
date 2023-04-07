#include "TetrisWindow.h"
#include <SDL.h>
#include <iostream>
#include <random>
#include <utility>


TetrisWindow::TetrisWindow(int BLOCK_SIZE, int BLOCKS_X, int BLOCKS_Y, int* points,
                           SDL_Renderer *renderer, std::shared_ptr<ResourceManager> resourceManager)
        : BLOCK_SIZE(BLOCK_SIZE), BLOCKS_X(BLOCKS_X), BLOCKS_Y(BLOCKS_Y), renderer(renderer), points(points), resourceManager(std::move(resourceManager)),
          WIDTH(BLOCK_SIZE * BLOCKS_X + BLOCKS_X - 2),
          HEIGHT(BLOCK_SIZE * BLOCKS_Y + BLOCKS_Y - 2){

    // Initialize grid
    grid = std::vector<std::vector<GridElement>>(BLOCKS_Y, std::vector<GridElement>(BLOCKS_X, {TetrominoType::EMPTY}));

    const int PREVIEW_DIMENSIONS = 4; // Biggest block, n x n grid

    nextBlockGrid = std::vector<std::vector<GridElement>>(PREVIEW_DIMENSIONS, std::vector<GridElement>(PREVIEW_DIMENSIONS, {TetrominoType::EMPTY})); // Biggest block is 4

    NEXT_PREVIEW_WIDTH = BLOCK_SIZE * PREVIEW_DIMENSIONS + PREVIEW_DIMENSIONS-2;
    NEXT_PREVIEW_HEIGHT = BLOCK_SIZE * PREVIEW_DIMENSIONS + PREVIEW_DIMENSIONS-2;

    newBlock();

    // Create texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
    if (texture == nullptr){
        throw std::runtime_error("Could not create texture: " + std::string(SDL_GetError()));
    }

    nextBlockPreviewTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, NEXT_PREVIEW_WIDTH, NEXT_PREVIEW_HEIGHT);
    if (nextBlockPreviewTexture == nullptr){
        throw std::runtime_error("Could not create texture: " + std::string(SDL_GetError()));
    }
}


void TetrisWindow::renderLoop() {
    if (gameOver) return;

    renderGrid(texture, grid, currentBlock.get());
    renderGrid(nextBlockPreviewTexture, nextBlockGrid, nextBlock.get());
}

void TetrisWindow::gameLoop() {
    if (!currentBlock)return;

    CollisionType collisionType = currentBlock->tryMove(grid, 0, 1);
    if (collisionType == COLLISION_BLOCKS) newBlock();

}

void TetrisWindow::onKeyPress(SDL_Keycode key) {

    if (!currentBlock)return;
    if (key == SDLK_UP){
        currentBlock->tryRotation(grid, 1);
    }
    else if (key == SDLK_DOWN){
        currentBlock->tryRotation(grid, -1);
    }
    else if (key == SDLK_LEFT){
        currentBlock->tryMove(grid, -1, 0);
    }
    else if (key == SDLK_RIGHT){
        currentBlock->tryMove(grid, 1, 0);
    }
    else if (key == SDLK_SPACE){ // SLAM
        while (currentBlock->tryMove(grid, 0, 1) == NO_COLLISION){}
        resourceManager->playSound(Sound::DROP);
        newBlock();
    }
}

void TetrisWindow::onKeyRelease(SDL_Keycode key) {

}

void TetrisWindow::newBlock() {
    if (currentBlock){
        currentBlock->drawToGrid(grid, true);
        checkRows();
    }

    if (!nextBlock){
        nextBlock = getRandomBlock();
        nextBlock->forceMove(0, 1); // Center
    }

    currentBlock = std::move(nextBlock);
    currentBlock->forceMove( BLOCKS_X/2-currentBlock->getMatrixSizeX()/2 ,-1);

    nextBlock = getRandomBlock();
    nextBlock->forceMove(0, 1); // Center


    if (currentBlock->tryMove(grid, 0, 0) != CollisionType::NO_COLLISION){
        // Game over
        gameOver = true;
        resourceManager->playSound(Sound::GAME_OVER);
    }


}

void TetrisWindow::checkRows() {

    std::vector<int> indexes;

    for (int y = 0; y < BLOCKS_Y; y++){

        int blockCount = 0;
        for (int x = 0; x < BLOCKS_X; x++){
            if (grid[y][x].type != TetrominoType::EMPTY && grid[y][x].isStatic)blockCount++;
        }

        if (blockCount == BLOCKS_X){
            // Clear row
            for (int x = 0; x < BLOCKS_X; x++){
                grid[y][x] = {TetrominoType::EMPTY};
            }
            indexes.push_back(y);
        }
    }

    if (!indexes.empty()){
        resourceManager->playSound(Sound::CLEAR_ROW);
    }
    switch(indexes.size()){
        case 0:
            break;
        case 1:
            *points += 40;
            break;
        case 2:
            *points += 100;
            break;
        case 3:
            *points += 300;
            break;
        case 4:
            *points += 1200;
            break;
        default:
            *points += 1000000;
            break;
    }



    // Indexes are from top down
    for (int moveLoc : indexes){
        for (int i = 0; i < moveLoc-1; i++){
            int y = moveLoc - i;
            std::swap(grid[y], grid[y-1]);

        }
    }

}

std::unique_ptr<Tetromino> TetrisWindow::getRandomBlock() {

    std::unique_ptr<Tetromino> randomBlock;

    static std::random_device r;
    static std::default_random_engine rand(r());

    int blockRand = std::uniform_int_distribution<>{0, 6}(rand);

    switch(blockRand){
        case 0:
            randomBlock = std::make_unique<TetrominoI>(0, 0);
            break;
        case 1:
            randomBlock = std::make_unique<TetrominoO>(0, 0);
            break;
        case 2:
            randomBlock = std::make_unique<TetrominoT>(0, 0);
            break;
        case 3:
            randomBlock = std::make_unique<TetrominoL>(0, 0);
            break;
        case 4:
            randomBlock = std::make_unique<TetrominoJ>(0, 0);
            break;
        case 5:
            randomBlock = std::make_unique<TetrominoS>(0, 0);
            break;
        case 6:
            randomBlock = std::make_unique<TetrominoZ>(0, 0);
            break;
        case 7:
            //currentBlock = std::make_unique<TetrominoP>(4, 0);
            break;
    }

    return std::move(randomBlock);
}

void TetrisWindow::renderGrid(SDL_Texture *texture, std::vector<std::vector<GridElement>> grid, Tetromino* dynamicBlock) {
    int SIZE_Y = grid.size();
    int SIZE_X = grid[0].size();

    SDL_SetRenderTarget(renderer, texture);

    // Background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Clear dynamic blocks
    for (int y = 0; y < SIZE_Y; y++){
        for (int x = 0; x < SIZE_X; x++){
            if (!grid[y][x].isStatic){
                grid[y][x] = {TetrominoType::EMPTY};
            }
        }
    }

    // Draw current dynamic block
    dynamicBlock->drawToGrid(grid);

    // Grid
    for (int y = 0; y < SIZE_Y; y++){
        for (int x = 0; x < SIZE_X; x++){
            int yPos = y*BLOCK_SIZE + y;
            int xPos = x*BLOCK_SIZE + x;

            if (Tetromino::tetrominoTextures.count(grid[y][x].type)){
                resourceManager->drawImage(xPos, yPos, BLOCK_SIZE, BLOCK_SIZE, Tetromino::tetrominoTextures[grid[y][x].type]);
            }else{
                SDL_Rect rect = {xPos, yPos, BLOCK_SIZE, BLOCK_SIZE};
                Color gottenColor = Tetromino::tetrominoToColor(grid[y][x].type);
                SDL_SetRenderDrawColor(renderer, gottenColor.r, gottenColor.g, gottenColor.b, gottenColor.a);
                SDL_RenderFillRect(renderer, &rect);
            }

        }
    }

    // Background lines
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for (int i = 1; i < SIZE_X; i++){
        SDL_RenderDrawLine(renderer, BLOCK_SIZE*i + (i-1), 0, BLOCK_SIZE*i + (i-1), HEIGHT);
    }
    for (int i = 1; i < SIZE_Y; i++){
        SDL_RenderDrawLine(renderer, 0, BLOCK_SIZE*i + (i-1), WIDTH, BLOCK_SIZE*i + (i-1));
    }

    SDL_SetRenderTarget(renderer, NULL);
}
