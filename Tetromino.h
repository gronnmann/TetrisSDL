#pragma once
#include <vector>
#include "TetrisWindow.h"
#include "ResourceManager.h"

struct Color;
struct GridElement;

enum class TetrominoType{
    EMPTY,
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
    P,
};

enum CollisionType{
    NO_COLLISION,
    COLLISION_SIDES,
    COLLISION_BLOCKS,
};

class Tetromino {
public:
    Tetromino(int x, int y, TetrominoType type);

    TetrominoType getType() const {return type;};

    std::vector<std::vector<int>> getBlockMatrix() const {return rotationMatrix[rotationStatus];};

    CollisionType tryRotation(std::vector<std::vector<GridElement>>& grid, int rotation);
    CollisionType tryMove(std::vector<std::vector<GridElement>>& grid, int x, int y);
    void forceMove(int x, int y); // Used for spawning from one grid to another

    void drawToGrid(std::vector<std::vector<GridElement>>& grid, bool setStatic = false);

    static Color tetrominoToColor(TetrominoType type);
    static std::map<TetrominoType, Texture> tetrominoTextures;


    int getMatrixSizeX() const;
    int getMatrixSizeY() const;
private:
    int rotationStatus = 0;

    void rotate(int rotation);

    void move(int x, int y);

    int X_LOC, Y_LOC;

    TetrominoType type;

    CollisionType checkCollisions(std::vector<std::vector<GridElement>>& grid);

protected:

    std::vector<std::vector<std::vector<int>>> rotationMatrix;
};

// Rotation matrixes from https://tetris.fandom.com/wiki/SRS?file=SRS-pieces.png

class TetrominoI : public Tetromino{
public:
    TetrominoI(int x, int y) : Tetromino(x, y, TetrominoType::I) {
        rotationMatrix = { // Rotation 1
                {
                        {0,0,0,0},
                        {1,1,1,1},
                        {0,0,0,0},
                        {0,0,0,0},
                    },

                { // Rotation 2
                        {0,0,1,0},
                        {0,0,1,0},
                        {0,0,1,0},
                        {0,0,1,0}
                    },

                { // Rotation 3
                        {0,0,0,0},
                        {0,0,0,0},
                        {1,1,1,1},
                        {0,0,0,0},
                    },

                { // Rotation 4
                        {0,1,0,0},
                        {0,1,0,0},
                        {0,1,0,0},
                        {0,1,0,0},
                    },
        };
    };
};

class TetrominoO : public Tetromino{
public:
    TetrominoO(int x, int y) : Tetromino(x, y, TetrominoType::O) {
        rotationMatrix = { // Rotation 1
                {
                        {1,1},
                        {1,1},
                },
        };
    };
};

class TetrominoT : public Tetromino{
public:
    TetrominoT(int x, int y) : Tetromino(x, y, TetrominoType::T) {
        rotationMatrix = { // Rotation 1
                {
                        {0,1,0},
                        {1,1,1},
                        {0,0,0},
                },

                { // Rotation 2
                        {0,1,0},
                        {0,1,1},
                        {0,1,0},
                },

                { // Rotation 3
                        {0,0,0},
                        {1,1,1},
                        {0,1,0},
                },

                { // Rotation 4
                        {0,1,0},
                        {1,1,0},
                        {0,1,0},
                },
        };
    };
};

class TetrominoL : public Tetromino{
public:
    TetrominoL(int x, int y) : Tetromino(x, y, TetrominoType::L) {
        rotationMatrix = { // Rotation 1
                {
                        {0,0,1},
                        {1,1,1},
                        {0,0,0},
                },

                { // Rotation 2
                        {0,1,0},
                        {0,1,0},
                        {0,1,1},
                },

                { // Rotation 3
                        {0,0,0},
                        {1,1,1},
                        {1,0,0},
                },

                { // Rotation 4
                        {1,1,0},
                        {0,1,0},
                        {0,1,0},
                },
        };
    };
};

class TetrominoJ : public Tetromino{
public:
    TetrominoJ(int x, int y) : Tetromino(x, y, TetrominoType::J) {
        rotationMatrix = { // Rotation 1
                {
                        {1,0,0},
                        {1,1,1},
                        {0,0,0},
                },

                { // Rotation 2
                        {0,1,1},
                        {0,1,0},
                        {0,1,0},
                },

                { // Rotation 3
                        {0,0,0},
                        {1,1,1},
                        {0,0,1},
                },

                { // Rotation 4
                        {0,1,0},
                        {0,1,0},
                        {1,1,0},
                },
        };
    };
};

class TetrominoS : public Tetromino{
public:
    TetrominoS(int x, int y) : Tetromino(x, y, TetrominoType::S) {
        rotationMatrix = { // Rotation 1
                {
                        {0,1,1},
                        {1,1,0},
                        {0,0,0},
                },

                { // Rotation 2
                        {0,1,0},
                        {0,1,1},
                        {0,0,1},
                },

                { // Rotation 3
                        {0,0,0},
                        {0,1,1},
                        {1,1,0},
                },

                { // Rotation 4
                        {1,0,0},
                        {1,1,0},
                        {0,1,0},
                },
        };
    };
};

class TetrominoZ : public Tetromino{
public:
    TetrominoZ(int x, int y) : Tetromino(x, y, TetrominoType::Z) {
        rotationMatrix = { // Rotation 1
                {
                        {1,1,0},
                        {0,1,1},
                        {0,0,0},
                },

                { // Rotation 2
                        {0,0,1},
                        {0,1,1},
                        {0,1,0},
                },

                { // Rotation 3
                        {0,0,0},
                        {1,1,0},
                        {0,1,1},
                },

                { // Rotation 4
                        {0,0,1},
                        {0,1,1},
                        {0,1,0},
                },
        };
    };
};

class TetrominoP : public Tetromino{
public:
    TetrominoP(int x, int y) : Tetromino(x, y, TetrominoType::P) {
        rotationMatrix = { // Rotation 1
                {
                        {0,1,0},
                        {0,1,0},
                        {1,0,1},
                },

                { // Rotation 2
                        {1,0,0},
                        {0,1,1},
                        {1,0,0},
                },

                { // Rotation 3
                        {1,0,1},
                        {0,1,0},
                        {0,1,0},
                },

                { // Rotation 4
                        {0,0,1},
                        {1,1,0},
                        {0,0,1},
                },
        };
    };
};
