#include "Tetromino.h"
#include <iostream>


Tetromino::Tetromino(int x, int y, TetrominoType type)
    : X_LOC(x), Y_LOC(y), type(type){
}

std::map<TetrominoType, Texture> Tetromino::tetrominoTextures = {
        {TetrominoType::L, Texture::BLOCK_L},
        {TetrominoType::J, Texture::BLOCK_J},
        {TetrominoType::I, Texture::BLOCK_I},
        {TetrominoType::O, Texture::BLOCK_O},
        {TetrominoType::S, Texture::BLOCK_S},
        {TetrominoType::T, Texture::BLOCK_T},
        {TetrominoType::Z, Texture::BLOCK_Z},
};


Color Tetromino::tetrominoToColor(TetrominoType type) {
    switch (type){
        case TetrominoType::EMPTY:
            return {0, 0, 0, 0};
        case TetrominoType::I:
            return {0, 255, 255, 255};
        case TetrominoType::J:
            return {0, 0, 255, 255};
        case TetrominoType::L:
            return {255, 165, 0, 255};
        case TetrominoType::O:
            return {255, 255, 0, 255};
        case TetrominoType::S:
            return {0, 255, 0, 255};
        case TetrominoType::T:
            return {128, 0, 128, 255};
        case TetrominoType::Z:
            return {255, 0, 0, 255};
        default:
            return {255, 255, 255, 255};
    }
}

void Tetromino::rotate(int rotation){
    int newRotation = (rotation + rotationStatus) % rotationMatrix.size();

    if (newRotation < 0) newRotation += rotationMatrix.size();

    rotationStatus = newRotation;
}

void Tetromino::move(int x, int y) {
    this->X_LOC += x;
    this->Y_LOC += y;
}


int Tetromino::getMatrixSizeX() const {
    return getBlockMatrix()[0].size();
}

int Tetromino::getMatrixSizeY() const {
    return getBlockMatrix().size();
}

CollisionType Tetromino::checkCollisions(std::vector<std::vector<GridElement>> &grid) {
    for (int y = 0; y < getMatrixSizeY(); y++){
        for (int x = 0; x < getMatrixSizeX(); x++){

            if (getBlockMatrix()[y][x] == 0)continue;

            int coordY = y+Y_LOC;
            int coordX = x+X_LOC;

            if (coordY < 0 || coordY >= grid.size()) return COLLISION_BLOCKS;
            if (coordX < 0 || coordX >= grid[y].size())return COLLISION_SIDES;

            GridElement el = grid[coordY][coordX];

            if (el.isStatic && el.type != TetrominoType::EMPTY) return COLLISION_BLOCKS;

        }

    }

    return NO_COLLISION;
}

void Tetromino::drawToGrid(std::vector<std::vector<GridElement>> &grid, bool setStatic) {
    for (int y = 0; y < getMatrixSizeY(); y++){
        for (int x = 0; x < getMatrixSizeX(); x++){

            if (getBlockMatrix()[y][x] == 0)continue;

            int coordY = y+Y_LOC;
            int coordX = x+X_LOC;

            grid[coordY][coordX] = {getType(), setStatic};

        }

    }
}

CollisionType Tetromino::tryMove(std::vector<std::vector<GridElement>> &grid, int x, int y) {
    move(x, y);

    CollisionType colType = checkCollisions(grid);

    if (colType != CollisionType::NO_COLLISION){
        move(-x, -y);
    }

    return colType;
}

CollisionType Tetromino::tryRotation(std::vector<std::vector<GridElement>> &grid, int rotation) {
    rotate(rotation);
    CollisionType colType = checkCollisions(grid);

    if (colType != CollisionType::NO_COLLISION){
        rotate(-rotation);
    }

    return colType;
}

void Tetromino::forceMove(int x, int y) {
    move(x, y);
}
