#include <iostream>
#include "TetrisWindow.h"
#include <SDL.h>
#include "ResourceManager.h"

constexpr int WIDTH = 800, HEIGHT = 720;
constexpr int BLOCK_SIZE = 30, BLOCKS_X = 10, BLOCKS_Y = 20;

int points = 0;

enum GameState{
    STOPPED,
    PLAYING,
    PAUSED,
};

std::shared_ptr<ResourceManager> resourceManager;
std::unique_ptr<TetrisWindow> gameWindow;

GameState gameState = GameState::STOPPED;
bool everStarted = false;

SDL_Renderer* renderer;

bool onKeyPress(SDL_Keycode keyCode);
void respawnGame();
void renderOverlays();

int main() {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        throw std::runtime_error("Could not init SDL");
    }

    SDL_Window* window = SDL_CreateWindow("TetrisSDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    if (window == nullptr){
        throw std::runtime_error("Could not create window: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr){
        throw std::runtime_error("Could not create renderer: " + std::string(SDL_GetError()));
    }

    resourceManager = std::make_shared<ResourceManager>(renderer);
    if (!resourceManager->isInitialized()){
        throw std::runtime_error("Failed to initialize ResourceManager");
    }

    respawnGame();


    float frameTime = 1.0f / 3;

    const int BOARD_X = WIDTH/2 - gameWindow->getWidth()/2;
    const int BOARD_Y = HEIGHT/2 - gameWindow->getHeight()/2;


    auto lastTime = std::chrono::system_clock::now();
    while(true){
        frameTime = 1.0f / (points/1000.0f + 3);

        SDL_Event event;
        if (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                break;
            }
            else if (event.type == SDL_KEYDOWN){

                if (!onKeyPress(event.key.keysym.sym)) continue;

                if (gameState == GameState::PLAYING){ // PLAY
                    gameWindow->onKeyPress(event.key.keysym.sym);
                }


            }
            else if (event.type == SDL_KEYUP){
                if (gameState == GameState::PLAYING){
                    gameWindow->onKeyRelease(event.key.keysym.sym);
                }
            }
        }

        //

        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_ms = currentTime - lastTime;
        if (elapsed_ms.count() > frameTime){
            if (gameState == GameState::PLAYING){
                gameWindow->gameLoop();

                if (gameWindow->isGameOver()){
                    gameState = GameState::STOPPED;
                }
            }
            lastTime = currentTime;
        }

        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 23, 66, 255);

        SDL_RenderClear(renderer);

        resourceManager->drawImage(0, 0, WIDTH, HEIGHT, Texture::BACKGROUND);

        // Rendering
        gameWindow->renderLoop();

        // Game board
        SDL_Rect boardLoc = { BOARD_X,
                             BOARD_Y,
                              gameWindow->getWidth(),
                              gameWindow->getHeight()};


        SDL_RenderCopy(renderer, gameWindow->getTexture(), NULL, &boardLoc);

        // Next block preview
        SDL_Rect previewLoc = {
                BOARD_X + gameWindow->getWidth() + 20,
                BOARD_Y + BLOCK_SIZE,
                gameWindow->getBlockPreviewWidth(),
                gameWindow->getBlockPreviewHeight()
        };
        SDL_RenderCopy(renderer, gameWindow->getBlockPreviewTexture(), NULL, &previewLoc);

        resourceManager->drawText(BOARD_X + gameWindow->getWidth() + 20, BOARD_Y, "Next block:", FontSize::SMALL,
                                  {255, 255, 255, 255});

        // Score
        resourceManager->drawText(10, 10, "Score: " + std::to_string(points),
                                  FontSize::SMALL, {255,255,255,255});

        renderOverlays();


        // Finish rest
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);


    SDL_Quit();
    return 0;
}

bool onKeyPress(SDL_Keycode keyCode){
    if (gameState == GameState::PLAYING){

        if (keyCode == SDLK_ESCAPE){ // PAUSE
            gameState = GameState::PAUSED;
        }


    }else{
        if (gameState == GameState::PAUSED){

        }else if (gameState == GameState::STOPPED){
            if (!everStarted){
                everStarted = true;
            }
            else{
                if (keyCode == SDLK_SPACE && gameWindow->isGameOver()){
                    respawnGame();
                }
            }
        }
        // Both
        if (keyCode == SDLK_SPACE){
            gameState = GameState::PLAYING;
            return false;
        }
    }

    return true;
}

void renderOverlays(){
    if (gameState != GameState::PLAYING){
        resourceManager->drawText(WIDTH/2, HEIGHT/2 + 50, "Press SPACE to continue...",
                                  FontSize::MEDIUM, {255, 255, 255, 255}, true);

        if (gameState == GameState::PAUSED){
            resourceManager->drawText(WIDTH/2, HEIGHT/2 - 50, "PAUSED",
                                      FontSize::LARGE, {255, 255, 255, 255}, true);
        }
        else if (gameState == GameState::STOPPED && everStarted){
            resourceManager->drawText(WIDTH/2, HEIGHT/2 - 50, "GAME OVER",
                                      FontSize::LARGE, {255, 255, 255, 255}, true);
        }
        else if (gameState == GameState::STOPPED && !everStarted){
            resourceManager->drawImage(WIDTH/2, HEIGHT/2 - 100, Texture::LOGO, true);
        }
    }

    resourceManager->drawText(WIDTH-130, HEIGHT - 20, "Copyright (C) gronnmann",  FontSize::X_SMALL, {255, 255, 255, 255});
}

void respawnGame(){ // Just respawn the game window
    points = 0;
    gameWindow = std::make_unique<TetrisWindow>(BLOCK_SIZE, BLOCKS_X, BLOCKS_Y, &points, renderer, resourceManager);
}